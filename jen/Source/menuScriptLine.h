/* menuScriptLine class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#ifndef __MENUSCRIPTLINE_H
#define __MENUSCRIPTLINE_H

#include "jStack.h"

class menuCom;
class menuItem;
class menuVar;

class menuScriptLine
{
private:
	menuCom*		command;
	menuItem*		parent;

	jStack<menuVar>*	fSt;
	jStack<menuVar>*	vSt;

	menuCom*		paramCom;
	menuVar*		paramVar;
public:
	menuScriptLine();
	menuScriptLine(menuCom* com, menuItem* item, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam);
	~menuScriptLine();

	void Call();
};

#endif