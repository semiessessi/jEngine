/* menuScriptLine class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#include "menuScriptLine.h"

#include "menuCom.h"
#include "menuVar.h"

#include "log.h"

#include <malloc.h>

menuScriptLine::menuScriptLine()
{
	// constructor code for menuScriptLine
	command = 0;
	parent = 0;
	fSt = 0;
	vSt = 0;
	paramCom = 0;
	paramVar = 0;
}

menuScriptLine::menuScriptLine(menuCom* com, menuItem* item, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam)
{
	// constructor code for menuScriptLine
	command = com;
	parent = item;
	fSt = tempStack;
	vSt = varStack;
	paramCom = comParam;
	paramVar = varParam;
}

menuScriptLine::~menuScriptLine()
{
	// destructor code for menuScriptLine
}

void menuScriptLine::Call()
{
	command->Call(parent, fSt, vSt, paramCom, paramVar);
}