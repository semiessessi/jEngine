/* menuCom class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#include "menuCom.h"

#include "menuItem.h"
#include "menuSystem.h"
#include "menuVar.h"

#include "console.h"
#include "jEngine.h"
#include "strSystem.h"

#include <string.h>

menuCom::menuCom(menuSystem* ms, const wchar_t* name, const wchar_t* help, void(*func)(menuItem*, jStack<menuVar>*, jStack<menuVar>*, menuCom*&, menuVar*&), const char* typeList, unsigned int precedence, bool isOp)
{
	// constructor code for menuCom
	m = ms;
	
	command = 0;
	helpString = 0;
	// if func is null replace with 'null' function pointer
	caller = func ? func : comNop;

	prec = precedence;

	op = isOp;
	
	command = new wchar_t[wcslen(name)+1];
	helpString = new wchar_t[wcslen(help)+1];

	command = wcscpy(command, name);
	helpString = wcscpy(helpString, help);

	if(typeList)
	{
		paramTypes = new char[strlen(typeList)+1];
		strcpy(paramTypes, typeList);
	}
	else
	{
		paramTypes = new char[1];
		paramTypes[0] = 0;
	}
}

menuCom::~menuCom()
{
	// destructor code for menuCom
	if(command) delete[] command;
	if(helpString) delete[] helpString;
	if(paramTypes) delete[] paramTypes;
}

unsigned int menuCom::getNumParams()
{
	return (unsigned int)strlen(paramTypes);
}

bool menuCom::isCommand(const wchar_t* strName)
{
	if(_wcsicmp(strName,command)) return false;
	else return true;
}

void menuCom::comNop(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam)
{
	return;
}

void menuCom::comConsole(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam)
{
	menuVar* v1 = varStack->popFast();
	jEngine::getPointer()->con->executeLine(v1->getString());
	return;
}

void menuCom::comPushT(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam)
{
	tempStack->push(varParam);
	return;
}

void menuCom::comPopT(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam)
{
	varParam = tempStack->popFast();
	return;
}

void menuCom::comPushV(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam)
{
	varStack->push(varParam);
	return;
}

void menuCom::comPopV(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam)
{
	varStack->popFast();
	return;
}

void menuCom::comStackClear(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam)
{
	varStack->clear();
	while(tempStack->peek())
	{
		delete tempStack->pop();
	}
	return;
}