/* strSystem class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#include "strSystem.h"
#include "jEngine.h"

#include "log.h"
#include "jProfiler.h"
#include "jText.h"

#include <malloc.h>
#include <stdio.h>
#include <string.h>

#define isNum(a) ((a==L'0')||(a==L'1')||(a==L'2')||(a==L'3')||(a==L'4')||(a==L'5')||(a==L'6')||(a==L'7')||(a==L'8')||(a==L'9'))
#define getNum(a) (((int)a)-48)

#pragma warning( disable : 4311 )

strSystem* strSystem::p = 0;

strSystem::strSystem(jEngine* j)
{
	// constructor code for strSystem
	e=j;

	strName = 0;
	strContent = 0;
	numStrings = 0;

#ifdef _DEBUG
	LogWriteLine(L"\x25CB\x25CB\x25CB strSystem constructor complete");
#endif
}

strSystem::strSystem(const strSystem& a)
{
	// copy constructor code for strSystem
}

strSystem& strSystem::operator=(const strSystem& a)
{
	// since we only want a single instance always return this 
	return (*this);
}

strSystem::~strSystem()
{
	// destructor code for strSystem
	cleanupStringTable();

#ifdef _DEBUG
	LogWriteLine(L"\x25CB\x25CB\x25CB strSystem destructor complete");
#endif
}

void strSystem::cleanupStringTable()
{
	for(int i=0;i<numStrings;i++)
	{
		if(strName[i]) free(strName[i]);
		if(strContent[i]) free(strContent[i]);
	}

	if(strName) free(strName);
	if(strContent) free(strContent);
	
	numStrings=0;
	strName=0;
	strContent=0;
}

wchar_t* strSystem::strFillParams(wchar_t* ptr, wchar_t* str, va_list parg)
{
	if(!str) return 0;

	wchar_t*	vars[10];
	// work out length + some spare room (from string identifiers)
	int len = (int)wcslen(str)+1;

	int			numVars = 0;

	// find number of variables needed
	wchar_t* k = (wchar_t*)str;
	while(k=wcschr(k, L'%'))
	{
		if(isNum(k[1])) 
		{
			int tmp = getNum(k[1]);
			tmp = tmp ? tmp : 10;
			numVars = max(numVars, tmp);
		}
		k++;
	}

	// make array from argument list
	for(int j=0;j<numVars;j++) 
	{
		vars[j] = va_arg(parg, wchar_t*);
		len += (int)wcslen(vars[j]);
	}
	
	// realloc ptr
	wchar_t* ret = (wchar_t*)realloc(ptr,sizeof(wchar_t)*len);

	// iterate through string
	wchar_t* lastPos = (wchar_t*)str;
	wchar_t* pos = wcschr(str, L'%');
	int i = 0;
	while(pos)
	{
		wcscpy(&(ret[i]),lastPos);
		i+=(int)(wcslen(lastPos)-wcslen(pos));
		
		if(isNum(pos[1]))
		{
			int n = getNum(pos[1]);
			n = n ? n-1 : 9;
			if(vars[n])
			{
				wcscpy(&(ret[i]), vars[n]);
				i+=(int)wcslen(vars[n]);
			}
		} else if (pos[1]=='%')
		{
			wcscpy(&(ret[i]), L"%%");
			i+=2;
		}
		pos++;
		pos++;

		lastPos = pos;
		pos = wcschr(lastPos, L'%');
	}
	
	wcscpy(&(ret[i]),lastPos);

	ret = (wchar_t*)realloc(ret,sizeof(wchar_t)*(wcslen(ret)+1));

	return ret;
}

strSystem* strSystem::getPointer(jEngine* j)
{
	// return our instance of strSystem if its created 
	if(!p) p = new strSystem(j);
	return p;
}

int strSystem::compStr(const void* s1, const void* s2)
{
	//TODO::write special string comparison
	return 1;
}

bool strSystem::loadLanguage(const wchar_t* languageName)
{
#ifdef _DEBUG
	jProfiler profiler = jProfiler(L"strSystem::loadLanguage");
#endif

	if(!languageName) return false;

	wchar_t* filename = (wchar_t*)malloc((wcslen(languageName)+13)*sizeof(wchar_t));
	swprintf(filename, L"strings\\%s.txt", languageName);

	//FILE* f = _wfopen(filename, L"rb");
    jTextFile f;
    bool r = f.Load(filename);
	free(filename);

    if(!r) return false;

	cleanupStringTable();

	/*
    // skip bom
	fseek(f, 2L, SEEK_CUR);
    */

	while(true)
	{
		wchar_t buf1[32768];
		wchar_t buf2[32768];

        /*
		int ret = fwscanf(f,L"%[^=\r\n]=%[^\r\n]\r\n", buf1, buf2);
		
		if(ret<=0) break;
        */

        if(!f.ReadFormat(L"%[^=\r\n]=%[^\r\n]\r\n", buf1, buf2)) break;

		// add both strings to our table
		numStrings++;

		strName = (wchar_t**)realloc(strName,sizeof(wchar_t*)*numStrings);
		strContent = (wchar_t**)realloc(strContent,sizeof(wchar_t*)*numStrings);
			
		strName[numStrings-1] = (wchar_t*)malloc((wcslen(buf1)+1)*sizeof(wchar_t));
		strName[numStrings-1] = wcscpy(strName[numStrings-1], buf1);

		strContent[numStrings-1] = (wchar_t*)malloc((wcslen(buf2)+1)*sizeof(wchar_t));
		strContent[numStrings-1] = wcscpy(strContent[numStrings-1], buf2);
	}

	//fclose(f);

	//TODO::check in mod dir and find and load that strings\lang.txt
	return true;
}

wchar_t* strSystem::getString(wchar_t* stringName, ...)
{
	if(!stringName) return 0;

	// atm lets just brute force it...
	int i=0;
	va_list parg;
	va_start(parg, stringName);
	while(i<numStrings)
	{
		if(!wcscmp(stringName,strName[i])) 
		{
			
			wchar_t* ret = strFillParams(0, strContent[i], parg);
			va_end(parg);
			return ret;
		}
		i++;
	}
	
	// string not found
	if(i>=numStrings) 
	{
		wchar_t* ret = strFillParams(0, stringName, parg);
		va_end(parg);
		return ret;
	}

	return 0;
}

void strSystem::setUILanguage(void** ptr, const wchar_t* val)
{
	if(val)
	{
		if(p->loadLanguage(val))
		{
			*ptr = realloc(*ptr,sizeof(wchar_t)*(wcslen(val)+1));
			*ptr = (void*)wcscpy((wchar_t*)(*ptr),val);
		}
	}
}
