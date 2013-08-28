/* cVar class by Semi Essessi
 *
 * Console variable
 *
 */

#include "cVar.h"
#include "jEngine.h"

#include "strSystem.h"

#include <malloc.h>
#include <string.h>
#include <wchar.h>

cVar::cVar(jEngine* j, const wchar_t* variableName, const wchar_t* description, char t, void (*up)(void**), void (*down)(void**), void (*set)(void**, const wchar_t*))
{
	// constructor code for cVar
	e=j;

	// if any of the funcs are null replace with 'null' function pointer
	cycleUp = up ? up : swNop;
	cycleDown = down ? down : swNop;
	setter = set ? set : setNop;
	getStr = getNop;

	name = new wchar_t[wcslen(variableName)+1];
	helpString = new wchar_t[wcslen(description)+1];

	name = wcscpy(name, variableName);
	helpString = wcscpy(helpString, description);

	type = t;
	defaultValue = 0;

	// initialise variable
	if(type=='b') // boolean
	{
		variable = (void*)(new bool);
		*((bool*)variable) = false;
		defaultValue = (void*)(new bool);
		*((bool*)defaultValue) = false;
		setter = set ? setter : setBool;
		cycleUp = up ? cycleUp : swBool;
		cycleDown = down ? cycleDown : swBool;
		getStr = getBool;
	}
	else if(type=='f') // float
	{
		variable = (void*)(new float);
		*((float*)variable) = 0.0f;
		defaultValue = (void*)(new float);
		*((float*)defaultValue) = 0.0f;
		setter = set ? setter : setFloat;
		cycleUp = up ? cycleUp : swFloatInc;
		cycleDown = down ? cycleDown : swFloatDec;
		getStr = getFloat;
	}
	else if(type=='s') // string
	{
		variable = (wchar_t*)malloc(sizeof(wchar_t));
		*(wchar_t*)variable = 0;
		defaultValue = malloc(sizeof(wchar_t));
		*(wchar_t*)defaultValue = 0;
		setter = set ? setter : setString;
		getStr = getString;
	}
	else // integer
	{
		type = 'i';
		variable = (void*)(new int);
		*((int*)variable) = 0;
		defaultValue = (void*)(new int);
		*((int*)defaultValue) = 0;
		setter = set ? setter : setInt;
		cycleUp = up ? cycleUp : swIntInc;
		cycleDown = down ? cycleDown : swIntDec;
		getStr = getInt;
	}
}

cVar::cVar(jEngine* j, const wchar_t* variableName, const wchar_t* description, char t, void (*cycler)(void**), void (*set)(void**, const wchar_t*))
{
	// constructor code for cVar
	e=j;

	// if any of the funcs are null replace with 'null' function pointer
	cycleUp = cycler ? cycler : swNop;
	cycleDown = cycler ? cycler : swNop;
	setter = set ? set : setNop;
	getStr = getNop;

	name = new wchar_t[wcslen(variableName)+1];
	helpString = new wchar_t[wcslen(description)+1];

	name = wcscpy(name, variableName);
	helpString = wcscpy(helpString, description);

	type = t;
	defaultValue = 0;

	// initialise variable
	if(type=='b') // boolean
	{
		variable = (void*)(new bool);
		*((bool*)variable) = false;
		defaultValue = (void*)(new bool);
		*((bool*)defaultValue) = false;
		setter = set ? setter : setBool;
		cycleUp = cycler ? cycleUp : swBool;
		cycleDown = cycler ? cycleDown : swBool;
		getStr = getBool;
	}
	else if(type=='f') // float
	{
		variable = (void*)(new float);
		*((float*)variable) = 0.0f;
		defaultValue = (void*)(new float);
		*((float*)defaultValue) = 0.0f;
		setter = set ? setter : setFloat;
		cycleUp = cycler ? cycleUp : swFloatInc;
		cycleDown = cycler ? cycleDown : swFloatDec;
		getStr = getFloat;
	}
	else if(type=='s') // string
	{
		variable = (wchar_t*)malloc(sizeof(wchar_t));
		*(wchar_t*)variable = 0;
		defaultValue = malloc(sizeof(wchar_t));
		*(wchar_t*)defaultValue = 0;
		setter = set ? setter : setString;
		getStr = getString;
	}
	else // integer
	{
		type = 'i';
		variable = (void*)(new int);
		*((int*)variable) = 0;
		defaultValue = (void*)(new int);
		*((int*)defaultValue) = 0;
		setter = set ? setter : setInt;
		cycleUp = cycler ? cycleUp : swIntInc;
		cycleDown = cycler ? cycleDown : swIntDec;
		getStr = getInt;
	}
}

cVar::cVar(jEngine* j, const wchar_t* variableName, const wchar_t* description, char t, void (*set)(void**, const wchar_t*))
{
	// constructor code for cVar
	e=j;

	// if any of the funcs are null replace with 'null' function pointer
	cycleUp = swNop;
	cycleDown = swNop;
	setter = set ? set : setNop;
	getStr = getNop;

	name = new wchar_t[wcslen(variableName)+1];
	helpString = new wchar_t[wcslen(description)+1];

	name = wcscpy(name, variableName);
	helpString = wcscpy(helpString, description);

	type = t;
	defaultValue = 0;

	// initialise variable
	if(type=='b') // boolean
	{
		variable = (void*)(new bool);
		*((bool*)variable) = false;
		defaultValue = (void*)(new bool);
		*((bool*)defaultValue) = false;
		setter = set ? setter : setBool;
		cycleUp = swBool;
		cycleDown = swBool;
		getStr = getBool;
	}
	else if(type=='f') // float
	{
		variable = (void*)(new float);
		*((float*)variable) = 0.0f;
		defaultValue = (void*)(new float);
		*((float*)defaultValue) = 0.0f;
		setter = set ? setter : setFloat;
		cycleUp = swFloatInc;
		cycleDown = swFloatDec;
		getStr = getFloat;
	}
	else if(type=='s') // string
	{
		variable = (wchar_t*)malloc(sizeof(wchar_t));
		*(wchar_t*)variable = 0;
		defaultValue = malloc(sizeof(wchar_t));
		*(wchar_t*)defaultValue = 0;
		setter = set ? setter : setString;
		getStr = getString;
	}
	else // integer
	{
		type = 'i';
		variable = (void*)(new int);
		*((int*)variable) = 0;
		defaultValue = (void*)(new int);
		*((int*)defaultValue) = 0;
		setter = set ? setter : setInt;
		cycleUp = swIntInc;
		cycleDown = swIntDec;
		getStr = getInt;
	}
}

cVar::cVar(jEngine* j, const wchar_t* variableName, char t, void (*set)(void**, const wchar_t*))
{
	// constructor code for cVar
	e=j;

	// if any of the funcs are null replace with 'null' function pointer
	cycleUp = swNop;
	cycleDown = swNop;
	setter = set ? set : setNop;
	getStr = getNop;

	name = new wchar_t[wcslen(variableName)+1];
	name = wcscpy(name, variableName);

	helpString = new wchar_t[wcslen(L"(no description available)")+1];
	helpString = wcscpy(helpString, L"(no description available)");

	type = t;
	defaultValue = 0;

	// initialise variable
	if(type=='b') // boolean
	{
		variable = (void*)(new bool);
		*((bool*)variable) = false;
		defaultValue = (void*)(new bool);
		*((bool*)defaultValue) = false;
		setter = set ? setter : setBool;
		cycleUp = swBool;
		cycleDown = swBool;
		getStr = getBool;
	}
	else if(type=='f') // float
	{
		variable = (void*)(new float);
		*((float*)variable) = 0.0f;
		defaultValue = (void*)(new float);
		*((float*)defaultValue) = 0.0f;
		setter = set ? setter : setFloat;
		cycleUp = swFloatInc;
		cycleDown = swFloatDec;
		getStr = getFloat;
	}
	else if(type=='s') // string
	{
		variable = (wchar_t*)malloc(sizeof(wchar_t));
		*(wchar_t*)variable = 0;
		defaultValue = malloc(sizeof(wchar_t));
		*(wchar_t*)defaultValue = 0;
		setter = set ? setter : setString;
		getStr = getString;
	}
	else // integer
	{
		type = 'i';
		variable = (void*)(new int);
		*((int*)variable) = 0;
		defaultValue = (void*)(new int);
		*((int*)defaultValue) = 0;
		setter = set ? setter : setInt;
		cycleUp = swIntInc;
		cycleDown = swIntDec;
		getStr = getInt;
	}
}

cVar::~cVar()
{
	// destructor code for cVar
	if(name) delete[] name;
	if(helpString) delete[] helpString;

	// clean up variable
	if(variable) 
	{
		if(type=='s') free(variable);
		else delete variable;
	}

	if(defaultValue) 
	{
		if(type=='s') free(defaultValue);
		else delete defaultValue;
	}
}

void cVar::setValue(const wchar_t* str)
{
	setter(&variable, str);
}

void cVar::increment()
{
	cycleUp(&variable);
}

void cVar::decrement()
{
	cycleDown(&variable);
}

bool cVar::isVariable(const wchar_t* strName)
{
	wchar_t* cs = e->str->getString(name);
	if(!_wcsicmp(strName,cs))
	{
		free(cs);
		return true;
	}
	else
	{
		free(cs);
		return false;
	}
}

void cVar::setDefaultValue(const wchar_t* str)
{
	setter(&defaultValue,str);
}

void cVar::setToDefaultValue()
{
	if(type=='s') setter(&variable,(wchar_t*)defaultValue);
	else setter(&variable,getDefaultString());
}

void cVar::overrideGet(wchar_t* (*func)(void* p))
{
	if(func) getStr = func;
	else
	{
		if(type=='b') getStr = getBool;
		else if(type=='f') getStr = getFloat;
		else if(type=='i') getStr = getInt;
		else if(type=='s') getStr = getString;
	}
}

// setters

void cVar::setNop(void** p, const wchar_t* str)
{
	return;
}

void cVar::setBool(void** p, const wchar_t* str)
{
	strSystem* s = strSystem::getPointer(jEngine::getPointer());
	wchar_t* t = s->getString(L"True");
	wchar_t* f = s->getString(L"False");
	if(!_wcsicmp(f, str)) **((bool**)p) = false;
	else if(!_wcsicmp(t, str)) **((bool**)p) = true;
	else if(_wtoi(str))	**((bool**)p) = true;
	else **((bool**)p) = false;
	free(t);
	free(f);
}

void cVar::setFloat(void** p, const wchar_t* str)
{
	**((float**)p) = (float)_wtof(str);
}

void cVar::setInt(void** p, const wchar_t* str)
{
	**((int**)p) = _wtoi(str);
}

void cVar::setString(void** p, const wchar_t* str)
{
	if(str)
	{
		*p = realloc(*p,sizeof(wchar_t)*(wcslen(str)+1));
		*p = (void*)wcscpy((wchar_t*)(*p),str);
	}
	else
	{
		free(*p);
		*p = 0;
	}
	return;
}

// getter functions

wchar_t* cVar::getNop(void* p)
{
	return 0;
}

wchar_t* cVar::getBool(void* p)
{
	wchar_t* ret = 0;
	if(*((bool*)p))
	{
		ret = (wchar_t*)malloc(5*sizeof(wchar_t));
		ret = wcscpy(ret,L"True");
	} 
	else
	{
		ret = (wchar_t*)malloc(6*sizeof(wchar_t));
		ret = wcscpy(ret,L"False");
	}

	return ret;
}

wchar_t* cVar::getFloat(void* p)
{
	wchar_t* ret = (wchar_t*)malloc(64*sizeof(wchar_t));
	int len = swprintf(ret, L"%f", *((float*)p)) + 1;
	ret = (wchar_t*)realloc(ret,len*sizeof(wchar_t));

	return ret;
}

wchar_t* cVar::getInt(void* p)
{
	wchar_t* ret = (wchar_t*)malloc(64*sizeof(wchar_t));
	int len = swprintf(ret, L"%i", *((int*)p)) + 1;
	ret = (wchar_t*)realloc(ret,len*sizeof(wchar_t));

	return ret;
}

wchar_t* cVar::getString(void* p)
{
	wchar_t* ret = (wchar_t*)malloc(32767*sizeof(wchar_t));
	// add quotes
	int len = swprintf(ret, L"'%ls'", (wchar_t*)p) + 1;
	ret = (wchar_t*)realloc(ret,len*sizeof(wchar_t));

	return ret;
}

// increment/decrement functions

void cVar::swNop(void** p)
{
	return;
}

void cVar::swBool(void** p)
{
	(**((bool**)p)) = !(*((bool*)p));
}

void cVar::swFloatInc(void** p)
{
	(**((float**)p))++;
}

void cVar::swFloatDec(void** p)
{
	(**((float**)p))--;
}

void cVar::swIntInc(void** p)
{
	(**((int**)p))++;
}

void cVar::swIntDec(void** p)
{
	(**((int**)p))--;
}