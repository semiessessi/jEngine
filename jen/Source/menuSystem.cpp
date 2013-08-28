/* menuSystem class by Semi Essessi
 *
 * UI and menu system
 *
 */

#include "menuSystem.h"
#include "jEngine.h"

#include "menuCom.h"
#include "menuItem.h"
#include "menuVar.h"

#include "console.h"
#include "glSystem.h"
#include "glTextRender.h"
#include "inMouse.h"
#include "inSystem.h"
#include "jProfiler.h"
#include "jText.h"
#include "log.h"
#include "strSystem.h"

#include <io.h>
#include <stdio.h>
#include <wchar.h>

#ifdef _RDEBUG
#include <windows.h>
#endif

menuSystem* menuSystem::p = 0;

menuSystem::menuSystem(jEngine* j) : glRenderable()
{
	// constructor code for menuSystem
	e=j;
	mouse = e->in->getMouse();

	main = 0;
	numComs = 0;
	comList = 0;

	textRenderLarge = 0;
	textRenderDefault = 0;
	textRenderMono = 0;
	textRenderSerif = 0;
	textRenderSmall = 0;

	numMFL = 0;
	menuFileLinks = 0;

	rMouse = true;

#ifdef _DEBUG
	LogWriteLine(L"\x25CB\x25CB\x25CB menuSystem constructor complete");
#endif
}

menuSystem::menuSystem(const menuSystem& a)
{
	// copy constructor code for menuSystem
}

menuSystem& menuSystem::operator=(const menuSystem& a)
{
	// since we only want a single instance always return this 
	return (*this);
}

menuSystem::~menuSystem()
{
	// destructor code for menuSystem
	// if(main) delete main;

	for(unsigned int i=0;i<numComs;++i)
	{
		delete comList[i];
	}
	if(comList) free(comList);

	for(unsigned int i=0;i<numMFL;++i)
	{
		delete menuFileLinks[i];
	}
	if(menuFileLinks) free(menuFileLinks);

#ifdef _DEBUG
	LogWriteLine(L"\x25CB\x25CB\x25CB menuSystem destructor complete");
#endif
}

menuSystem* menuSystem::getPointer(jEngine* j)
{
	// return our instance of menuSystem if its created 
	if(!p) p = new menuSystem(j);
	return p;
}

float menuSystem::getLastDT()
{
	return e->timer->getLastDT();
}

void menuSystem::addCom(const wchar_t* name, const wchar_t* help, void (*func)(menuItem*, jStack<menuVar>*, jStack<menuVar>*, menuCom*&, menuVar*&), const char* typeList, unsigned int precedence, bool isOp)
{
	if(name&&func&&help)
	{
		menuCom* com = new menuCom(this,name,help,func, typeList, precedence, isOp);
		numComs++;
		
		comList = (menuCom**)realloc(comList, sizeof(menuCom*)*numComs);
		comList[numComs-1]=com;
	}
}

bool menuSystem::isCommand(const wchar_t* name)
{
	for(unsigned int i=0;i<numComs;i++)
	{
		if(comList[i]->isCommand(name)) return true;
	}
	return false;
}

menuCom* menuSystem::getCommand(const wchar_t* name)
{
	for(unsigned int i=0;i<numComs;i++)
	{
		if(comList[i]->isCommand(name)) return comList[i];
	}
	return 0;
}

// builds a list of all menus in error free files and remembers which file each one is in for later

void menuSystem::buildMenuFileList()
{
#ifdef _DEBUG
	jProfiler profiler = jProfiler(L"menuSystem::buildMenuFileList");
#endif

	// init menusystem stuff here why not...
	menuVar::strInitialise(e->str);
	initComs();

	_wfinddata_t f;
	intptr_t hFile;

	int count = 0;
	int mCount = 0;

	wchar_t** cacheNames = 0;
	int	numCache = 0;

	//TODO::look in mod directory
	hFile = _wfindfirst(L"menus\\*.menu", &f);

    jTextFile file;

	while(hFile!=-1L)
	{
		count++;
		bool err = false;
		
		wchar_t* path = (wchar_t*)malloc(2048*sizeof(wchar_t));
		int fl = swprintf(path, L"menus\\%s", f.name) + 1;
		path = (wchar_t*)realloc(path, fl*sizeof(wchar_t));
		
		//FILE* file = _wfopen(path,L"rb");
        file.Load(path);

		// get length of file
		//fseek(file, 0L, SEEK_END);
		//int lenData = (ftell(file))/sizeof(wchar_t);
        unsigned int lenData = file.GetStringLength();

		// dont read bom
		//fseek(file, 2L, SEEK_SET);

        unsigned int i = 0;
		wchar_t* data = (wchar_t*)malloc(sizeof(wchar_t)*(lenData));
		//fread(data, sizeof(wchar_t), lenData, file);
        for(i = 0; i < (unsigned int)lenData; ++i) data[i] = file.GetString()[i];
		data[lenData - 1] = 0;
		//fclose(file);

		// go through the script, ignoring comments and stuff in braces looking for 'menu XXX'
		i = 0;

		//TODO::handle quotes in here...

		// strip line comments
		while(i<lenData)
		{
			if((data[i] == L'/') && (data[i+1] == L'/'))
			{
				unsigned int k = i;
				while(data[i] != L'\n') i++;
				int d = i - k + 1;
				i = k;

				while(k<(lenData-d))
				{
					data[k]=data[k+d];
					k++;
				}
				while(k<lenData)
				{
					data[k]=0;
					k++;
				}
			}
			i++;
			if(data[i]==0) break;
		}

		lenData = i;

		// TODO::parse #defines first
		// TODO::allow #cache instead of cache as it is at the moment
		// strip preprocessor lines
		while(i<lenData)
		{
			if((data[i] == L'#'))
			{
				unsigned int k = i;
				while(data[i] != L'\n') i++;
				int d = i - k + 1;
				i = k;

				while(k<(lenData-d))
				{
					data[k]=data[k+d];
					k++;
				}
				while(k<lenData)
				{
					data[k]=0;
					k++;
				}
			}
			i++;
			if(data[i]==0) break;
		}

		lenData = i;

		// strip block comments
		i = 0;
		while(i<lenData)
		{
			if((data[i] == L'/') && (data[i+1] == L'*'))
			{
				unsigned int k = i;
				while(!((data[i-1] == L'*') && (data[i] == L'/'))) i++;
				unsigned int d = i - k + 1;
				i = k;

				while(k<(lenData-d))
				{
					data[k]=data[k+d];
					k++;
				}
				while(k<lenData)
				{
					data[k]=0;
					k++;
				}
			}
			i++;
			if(data[i]==0) break;
		}

		lenData = i;

		// remove braces and contents
		i = 0;
		while(i<lenData)
		{
			if(data[i] == L'{')
			{
				unsigned int braceCount = 1;
				unsigned int k = i;
				i++;
				while(braceCount != 0)
				{
					if(data[i]==L'{') braceCount++;
					if(data[i]==L'}') braceCount--;
					i++;
					// no end brace...
					if(i==lenData)
					{
						err = true;
						i--;
						break;
					}
				}
				unsigned int d = i - k + 1;
				i = k;

				while(k<(lenData-d))
				{
					data[k]=data[k+d];
					k++;
				}
				while(k<lenData)
				{
					data[k]=0;
					k++;
				}
			}
			// illegal end brace...
			else if(data[i] == L'}')
			{
				err = true;
				break;
			}
			i++;
			if(data[i]==0) break;
		}

		lenData = i;

		// reduce all consecutive white space to single spaces
		i = 0;
		while(i<lenData)
		{
			if((data[i]==L'\t') || (data[i]==L'\r') || (data[i]==L'\n')) data[i] = L' ';
			i++;
			if(data[i]==0) break;
		}

		lenData = i;

		i = 0;
		while(i<lenData)
		{
			if((data[i] == L' ') && (data[i+1] == L' '))
			{
				unsigned int k = i;
				while(data[i+2] == L' ') i++;
				unsigned int d = i - k + 1;
				i = k;

				while(k<(lenData-d))
				{
					data[k]=data[k+d];
					k++;
				}
				while(k<lenData)
				{
					data[k]=0;
					k++;
				}
			}
			i++;
			if(data[i]==0) break;
		}

		// remove first and last space if present
		if(data[i-1]==L' ') data[i-1]=0;
		if(data[0]==L' ')
		{
			for(unsigned int j=0;j<i;j++) data[j]=data[j+1];
		}

		lenData = (unsigned int)wcslen(data) + 1;

		if(err)
		{
			count--;
			wchar_t* estr = e->str->getString(L"Error(s) found in .menu file: %1", path);
			e->con->writeLine(estr);
			free(estr);
		} else
		{
			unsigned int cnt = 0;
			unsigned int offset = 0;

			while(true)
			{
				wchar_t		buf1[1024];
				wchar_t		buf2[1024];

				memset(buf1,0,sizeof(wchar_t)*1024);
				memset(buf2,0,sizeof(wchar_t)*1024);

				int c = swscanf(data+offset,L"%s %s", buf1, buf2);

				if(c<2)
				{
					break;
				}
				else if(!_wcsicmp(L"menu", buf1))
				{
					if(isMenuScriptKeyword(buf2))
					{
						// keywords can't be menu names
						count--;
						wchar_t* estr = e->str->getString(L"Error(s) found in .menu file: %1", path);
						e->con->writeLine(estr);
						free(estr);
						for(unsigned int j=0;j<cnt;j++)
						{
							removeLastMenuFileLink();
							mCount--;
						}
						break;
					}
					else
					{
						addMenuFileLink(buf2, path);
						cnt++;
						mCount++;
						offset+=(unsigned int)wcslen(buf1);
						offset+=(unsigned int)wcslen(buf2);
						offset+=2;
						if(offset>=lenData) break;
					}
				}
				else if(!_wcsicmp(L"cache", buf1))
				{
					numCache++;
					cacheNames = (wchar_t**)realloc(cacheNames, sizeof(wchar_t*)*numCache);
					wchar_t* newName = (wchar_t*)malloc(sizeof(wchar_t)*(wcslen(buf2)+1));
					wcscpy(newName, buf2);
					cacheNames[numCache-1]=newName;
					offset+=(unsigned int)wcslen(buf1);
					offset+=(unsigned int)wcslen(buf2);
					offset+=2;
					if(offset>=lenData) break;
				}
				else
				{
					// not a menu declaration
					offset+=(unsigned int)wcslen(buf1);
					offset+=(unsigned int)wcslen(buf2);
					offset+=2;
					if(offset>=lenData) break; 
				}
			}
		}

		if(path) free(path);
		if(data) free(data);

		int retval = _wfindnext(hFile, &f);
		if(retval == -1) break;
	}

	// build cache
	int nc = 0;
	menuItem* buff = main;
	for(int it=0;it<numCache;it++) if(loadMenu(cacheNames[it])) nc++;
	main = buff;

	wchar_t* ret = (wchar_t*)malloc(16*sizeof(wchar_t));
	int len = swprintf(ret, L"%i", count) + 1;
	ret = (wchar_t*)realloc(ret, len*sizeof(wchar_t));

	wchar_t* mret = (wchar_t*)malloc(16*sizeof(wchar_t));
	len = swprintf(mret, L"%i", mCount) + 1;
	mret = (wchar_t*)realloc(mret, len*sizeof(wchar_t));

	wchar_t* cret = (wchar_t*)malloc(16*sizeof(wchar_t));
	len = swprintf(cret, L"%i", nc) + 1;
	cret = (wchar_t*)realloc(cret, len*sizeof(wchar_t));

	wchar_t* rstr = e->str->getString(L"%1 menu file(s) succesfully loaded, %2 menu(s) found and %3 cached", ret, mret, cret);
	e->con->writeLine(rstr);
	free(rstr);
	free(ret);
    free(mret);
    free(cret);
	_findclose(hFile);

	for(int it=0;it<numCache;it++) delete[] cacheNames[it];
	if(cacheNames) free(cacheNames);
}

void menuSystem::addMenuFileLink(const wchar_t* name, const wchar_t* path)
{
	if(name&&path)
	{
		numMFL++;
		menuFileLinks = (menuFileLink**)realloc(menuFileLinks, sizeof(menuFileLink*)*numMFL);
		menuFileLinks[numMFL-1] = new menuFileLink(name, path);
	}
}

void menuSystem::removeLastMenuFileLink()
{
	if(numMFL>0)
	{
		numMFL--;
		delete menuFileLinks[numMFL];
		menuFileLinks = (menuFileLink**)realloc(menuFileLinks, sizeof(menuFileLink*)*numMFL);
	}
}

bool menuSystem::loadMenu(const wchar_t* name)
{
#ifdef _DEBUG
	jProfiler profiler = jProfiler(L"menuSystem::loadMenu");
#endif
	for(unsigned int i=0;i<numMFL;i++)
	{
		if(!_wcsicmp(menuFileLinks[i]->menuName, name))
		{
			// is it cached already?
			if(menuFileLinks[i]->ptr)
			{
				if(main) main->eventOnShutdown();
				if(main) main->shutdownGraphics();
				main = menuFileLinks[i]->ptr;
				main->initGraphics();
				main->eventOnInit();
				e->in->setMouseReciever(main);
				return true;
			}
			else
			{
				// load menu
				//FILE* file = _wfopen(menuFileLinks[i]->fileName,L"rb");
                jTextFile file;
                file.Load(menuFileLinks[i]->fileName);

				// get length of file
				//fseek(file, 0L, SEEK_END);
				//unsigned int lenData = (ftell(file))/sizeof(wchar_t);

                unsigned int lenData = file.GetStringLength();
				// dont read bom
				//fseek(file, 2L, SEEK_SET);

				wchar_t* data = (wchar_t*)malloc(sizeof(wchar_t)*(lenData));
		        //fread(data, sizeof(wchar_t), lenData, file);
                for(unsigned int j = 0; j < (unsigned int)lenData; ++j) data[j] = file.GetString()[j];
		        data[lenData - 1] = 0;
		        //fclose(file);

				unsigned int it = 0;

				bool inQuotes = false;

				// strip line comments
				while(it<lenData)
				{
					if(data[it]==L'"') inQuotes = !inQuotes; // ignore comments inside of quotes
					else if((data[it] == L'/') && (data[it+1] == L'/') && !inQuotes)
					{
						unsigned int k = it;
						while(data[it] != L'\n') it++;
						unsigned int d = it - k + 1;
						it = k;

						while(k<(lenData-d))
						{
							data[k]=data[k+d];
							++k;
						}
						while(k<lenData)
						{
							data[k]=0;
							++k;
						}
					}
					it++;
				}

				lenData = (unsigned int)wcslen(data)+1;

				// strip block comments
				it = 0;
				inQuotes = false;
				while(it<lenData)
				{
					if(data[it]==L'"') inQuotes = !inQuotes; // ignore comments inside of quotes
					else if((data[it] == L'/') && (data[it+1] == L'*') && !inQuotes)
					{
						unsigned int k = it;
						while(!((data[it-1] == L'*') && (data[it] == L'/'))) it++;
						unsigned int d = it - k + 1;
						it = k;

						while(k<(lenData-d))
						{
							data[k]=data[k+d];
							++k;
						}
						while(k<lenData)
						{
							data[k]=0;
							++k;
						}
					}
					++it;
					if(data[it]==0) break;
				}

				lenData = it;

				// count number of braces, brackets, commas and some operators for padding with spaces
				it = 0;
				inQuotes = false;
				unsigned int bCount = 0;
				while(it<lenData)
				{
					if(data[it]==L'"') inQuotes = !inQuotes; // ignore stuff inside of quotes
					else if(( ((data[it] == L'&') && (data[it+1] == L'&')) || ((data[it] == L'|') && (data[it+1] == L'|'))
								|| ((data[it] == L'^') && (data[it+1] == L'^')) || ((data[it] == L'=') && (data[it+1] == L'='))
								|| ((data[it] == L'>') && (data[it+1] == L'=')) || ((data[it] == L'<') && (data[it+1] == L'='))
								|| ((data[it] == L'!') && (data[it+1] == L'=')) || ((data[it] == L'+') && (data[it+1] == L'='))
								|| ((data[it] == L'-') && (data[it+1] == L'=')) || ((data[it] == L'*') && (data[it+1] == L'='))
								|| ((data[it] == L'/') && (data[it+1] == L'=')) || ((data[it] == L'&') && (data[it+1] == L'='))
								|| ((data[it] == L'^') && (data[it+1] == L'=')) || ((data[it] == L'|') && (data[it+1] == L'='))
								|| ((data[it] == L'+') && (data[it+1] == L'+')) || ((data[it] == L'-') && (data[it+1] == L'-')) )
							&& !inQuotes)
					{
						++bCount;
						++it;
					}
					else if(((data[it] == L'{') || (data[it] == L'}') || (data[it] == L'(') || (data[it] == L')') || (data[it] == L'[') || (data[it] == L']')
							|| (data[it] == L',') || (data[it] == L';') || (data[it] == L'*') || (data[it] == L'/') || (data[it] == L'+') || (data[it] == L'-')
							|| (data[it] == L'<') || (data[it] == L'>') || (data[it] == L'!') || (data[it] == L'&') || (data[it] == L'|') || (data[it] == L'^')
							|| (data[it] == L'=') || (data[it] == L'?') || (data[it] == L':') || (data[it] == L'<') || (data[it] == L'>'))
							&& !inQuotes)
						++bCount;
					++it;
					if(data[it]==0) break;
				}

				lenData = it + 2*bCount;

				data = (wchar_t*)realloc(data,sizeof(wchar_t)*(lenData+1));
				data[lenData]=0;

				unsigned int ic = 2*bCount;
				while(ic>0)
				{
					data[lenData-ic]=0;
					--ic;
				}

				// pad braces, brackets, commas and some operators with spaces
				// TODO:: update this to do more operators
				it = 0;
				inQuotes = false;
				bCount = 0;
				while(it<lenData)
				{
					if(data[it]==L'"') inQuotes = !inQuotes; // ignore stuff inside of quotes
					else if(( ((data[it] == L'&') && (data[it+1] == L'&')) || ((data[it] == L'|') && (data[it+1] == L'|'))
								|| ((data[it] == L'^') && (data[it+1] == L'^')) || ((data[it] == L'=') && (data[it+1] == L'='))
								|| ((data[it] == L'>') && (data[it+1] == L'=')) || ((data[it] == L'<') && (data[it+1] == L'='))
								|| ((data[it] == L'!') && (data[it+1] == L'=')) || ((data[it] == L'+') && (data[it+1] == L'='))
								|| ((data[it] == L'-') && (data[it+1] == L'=')) || ((data[it] == L'*') && (data[it+1] == L'='))
								|| ((data[it] == L'/') && (data[it+1] == L'=')) || ((data[it] == L'&') && (data[it+1] == L'='))
								|| ((data[it] == L'^') && (data[it+1] == L'=')) || ((data[it] == L'|') && (data[it+1] == L'='))
								|| ((data[it] == L'+') && (data[it+1] == L'+')) || ((data[it] == L'-') && (data[it+1] == L'-')) )
							&& !inQuotes)
					{
						// move everything along by two chars
						unsigned int j = lenData-1;
						it+=3;
						while(j>it)
						{
							data[j]=data[j-2];
							--j;
						}
						data[it-1]=data[it-2];
						data[it-2]=data[it-3];
						data[it]=L' ';
						data[it-3]=L' ';
					}
					else if(((data[it] == L'{') || (data[it] == L'}') || (data[it] == L'(') || (data[it] == L')') || (data[it] == L'[') || (data[it] == L']')
							|| (data[it] == L',') || (data[it] == L';') || (data[it] == L'*') || (data[it] == L'/') || (data[it] == L'+') || (data[it] == L'-')
							|| (data[it] == L'<') || (data[it] == L'>') || (data[it] == L'!') || (data[it] == L'&') || (data[it] == L'|') || (data[it] == L'^')
							|| (data[it] == L'=') || (data[it] == L'?') || (data[it] == L':') || (data[it] == L'<') || (data[it] == L'>'))
							&& !inQuotes)
					{
						// move everything along by two chars
						unsigned int j = lenData-1;
						it+=2;
						while(j>it)
						{
							data[j]=data[j-2];
							--j;
						}
						data[it-1]=data[it-2];
						data[it]=L' ';
						data[it-2]=L' ';
					}
					++it;
					if(data[it]==0) break;
				}

				data[lenData] = 0;

				// reduce all consecutive white space to single spaces
				it = 0;
				while(it<lenData)
				{
					if((data[it]==L'\t') || (data[it]==L'\r')) data[it] = L' '; // || (data[it]==L'\n') use \n to work out line numbers...
					++it;
					if(data[it]==0) break;
				}

				// TODO::uses spaces, add line numbers...
				it = 0;
				while(it<lenData)
				{
					if(data[it]==L'\n') data[it] = L' ';
					++it;
					if(data[it]==0) break;
				}

				lenData = it;

				it = 0;
				while(it<lenData)
				{
					if((data[it] == L' ') && (data[it+1] == L' '))
					{
						unsigned int k = it;
						while(data[it+2] == L' ') it++;
						unsigned int d = it - k + 1;
						it = k;

						while(k<(lenData-d))
						{
							data[k]=data[k+d];
							++k;
						}
						while(k<lenData)
						{
							data[k++]=0;
						}
					}
					++it;
					if(data[it]==0) break;
				}

				// remove first and last space if present
				if(data[it-1]==L' ') data[it-1]=0;
				if(data[0]==L' ')
				{
					for(unsigned int j=0;j<it;j++) data[j]=data[j+1];
				}

				lenData = (unsigned int)wcslen(data);
				data = (wchar_t*)realloc(data,sizeof(wchar_t)*(lenData+1));
				data[lenData]=0;

				// TODO::replace \n with line numbers

				// look for the menu in the top level scope
				it = 0;

				unsigned int size = (unsigned int)wcslen(name) + 5;
				unsigned int k = lenData-size;

				wchar_t nameStr[1024];

				swprintf(nameStr, L"menu %s", name);

				bool ret = false;

				while(it<k)
				{
					wchar_t buf = data[it+size];
					data[it+size]=0;
					if(!_wcsicmp(nameStr, &(data[it])))
					{
						data[it+size]=buf;
						
						unsigned int startPos = it;

						while(it<lenData)
						{
							if(data[it] == L'{')
							{
								int braceCount = 1;
								++it;
								while(braceCount != 0)
								{
									if(data[it]==L'{') ++braceCount;
									if(data[it]==L'}') --braceCount;
									if(data[it]==0)
									{
										break;
									}
									++it;
								}
								
								// create new menu item(s) out of its code block
								ret = true;
								data[it]=0;
								menuItem* newMenuItem = new menuItem(this, &(data[startPos]));
								menuFileLinks[i]->ptr = newMenuItem;
								if(main) main->eventOnShutdown();
								if(main) main->shutdownGraphics();
								main = newMenuItem;
								main->eventOnInit();
								main->initGraphics();
								e->in->setMouseReciever(main);
								break;
							}
							++it;
						}

						break;
					}
					else data[it+size]=buf;
					++it;
				}

				if(data) free(data);
				return ret;
			}
		}
	}

	return false;
}

bool menuSystem::isMenuScriptKeyword(const wchar_t* str)
{
	if(!_wcsicmp(L"menu", str)) return true;
	
	if(!_wcsicmp(L"window", str)) return true;
	
	if(!_wcsicmp(L"item", str)) return true;
	
	if(!_wcsicmp(L"label", str)) return true;
	if(!_wcsicmp(L"button", str)) return true;
	if(!_wcsicmp(L"inputbox", str)) return true;
	
	if(!_wcsicmp(L"oninit", str)) return true;
	if(!_wcsicmp(L"onshutdown", str)) return true;
	if(!_wcsicmp(L"ontimer", str)) return true;
	if(!_wcsicmp(L"onclick", str)) return true;
	if(!_wcsicmp(L"onmousemove", str)) return true;

	if(!_wcsicmp(L"text", str)) return true;
	if(!_wcsicmp(L"borderstyle", str)) return true;
	if(!_wcsicmp(L"resizestyle", str)) return true;
	if(!_wcsicmp(L"shapestyle", str)) return true;

	if(!_wcsicmp(L"show", str)) return true;
	if(!_wcsicmp(L"hide", str)) return true;

	if(!_wcsicmp(L"event", str)) return true;
	if(!_wcsicmp(L"function", str)) return true;
	return false;
}

void menuSystem::initGraphics()
{
#ifdef _DEBUG
	jProfiler profiler = jProfiler(L"menuSystem::initGraphics");
#endif
	glRenderable::initGraphics();

	if(textRenderLarge) delete textRenderLarge;
	if(textRenderMono) delete textRenderMono;
	if(textRenderSerif) delete textRenderSerif;
	if(textRenderSmall) delete textRenderSmall;

	textRenderLarge = new glTextRender(jEngine::getPointer(), "default.ttf", 16);
	textRenderDefault = gls->getDefaultTextRenderer();
	textRenderMono = new glTextRender(jEngine::getPointer(), "mono.ttf", 12);
	textRenderSerif = new glTextRender(jEngine::getPointer(), "serif.ttf", 12);
	textRenderSmall = new glTextRender(jEngine::getPointer(), "default.ttf", 8);

	if(main) main->initGraphics();
}

void menuSystem::shutdownGraphics()
{
#ifdef _DEBUG
	jProfiler profiler = jProfiler(L"menuSystem::shutdownGraphics");
#endif
	if(main) main->shutdownGraphics();
	
    if(textRenderLarge) delete textRenderLarge;
	if(textRenderMono) delete textRenderMono;
	if(textRenderSerif) delete textRenderSerif;
	if(textRenderSmall) delete textRenderSmall;

	glRenderable::shutdownGraphics();
}

void menuSystem::render()
{
	if(main)
    {
        gl->MatrixMode(GL_PROJECTION);
	    gl->LoadIdentity();
	    gl->Ortho(0.0,gls->getAspectRatio(),1.0,0.0,1.0,-1.0);
	    glDepthRange(0.0,1.0);
	    gl->MatrixMode(GL_MODELVIEW);
	    gl->LoadIdentity();

	    glScalef(MENU_SCALE, MENU_SCALE, 1.0f);

	    glEnable(GL_DEPTH_TEST);

        main->render();

        glDisable(GL_DEPTH_TEST);
    }

	if(rMouse)
	{
		// render mouse
		glBegin(GL_TRIANGLES);
			glVertex2f(mouse->getXPos()*INV_MENU_SCALE, mouse->getYPos()*INV_MENU_SCALE);
			glVertex2f(mouse->getXPos()*INV_MENU_SCALE, mouse->getYPos()*INV_MENU_SCALE+30);
			glVertex2f(mouse->getXPos()*INV_MENU_SCALE+20, mouse->getYPos()*INV_MENU_SCALE+20);
		glEnd();
	}
}

void menuSystem::ccomLoadMenu(const wchar_t* cl)
{
	wchar_t* str = 0;
	str = console::strGetParam(str, cl, 1);
	if(!str) console::ccomHelp(L"help loadmenu");
	else p->loadMenu(str);
    free(str);
}

menuFileLink::menuFileLink(const wchar_t* name, const wchar_t* path)
{
	menuName = new wchar_t[wcslen(name)+1];
	fileName = new wchar_t[wcslen(path)+1];

	menuName = wcscpy(menuName, name);
	fileName = wcscpy(fileName, path);

	ptr = 0;
}

menuFileLink::~menuFileLink()
{
	if(menuName) delete menuName;
	if(fileName) delete fileName;

	if(ptr) delete ptr;
}