/* cCom class by Semi Essessi
 *
 * Console command
 *
 */

#include "cCom.h"
#include "jEngine.h"

#include "strSystem.h"

#include <string.h>

cCom::cCom(jEngine* j, const wchar_t* name, const wchar_t* help, void(*func)(const wchar_t*))
{
	// constructor code for cCom
	e=j;
	command = 0;
	helpString = 0;
	// if func is null replace with 'null' function pointer
	caller = func ? func : ccomNop;
	
	command = new wchar_t[wcslen(name)+1];
	helpString = new wchar_t[wcslen(help)+1];

	command = wcscpy(command, name);
	helpString = wcscpy(helpString, help);
}

cCom::cCom(jEngine* j, const wchar_t* name, void(*func)(const wchar_t*))
{
	// constructor code for cCom
	e=j;
	command = 0;
	helpString = 0;
	func = 0;
	command = new wchar_t[wcslen(name)+1];
	command = wcscpy(command, name);

	helpString = new wchar_t[wcslen(L"(no description available)")+1];
	helpString = wcscpy(helpString, L"(no description available)");
}

cCom::~cCom()
{
	// destructor code for cCom
	if(command) delete[] command;
	if(helpString) delete[] helpString;
}

bool cCom::isCommand(const wchar_t* strName)
{
	wchar_t* cs = e->str->getString(command);
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

void cCom::ccomNop(const wchar_t* p)
{
	return;
}