/* menuCom class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#ifndef __MENUCOM_H
#define __MENUCOM_H

#include "jStack.h"

class menuItem;
class menuSystem;
class menuVar;

class menuCom
{
private:
	wchar_t*		command;
	wchar_t*		helpString;
	void			(*caller)(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam);
	char*			paramTypes;
	bool			op;

	unsigned int	prec;

	menuSystem*		m;
public:
	menuCom(menuSystem* ms, const wchar_t* name, const wchar_t* help, void(*func)(menuItem*, jStack<menuVar>*, jStack<menuVar>*, menuCom*& comParam, menuVar*& varParam), const char* typeList, unsigned int precedence, bool isOp);
	~menuCom();

	bool isCommand(const wchar_t* strName);
	void Call(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam) { caller(parent, tempStack, varStack, comParam, varParam); }

	wchar_t* getName() { return command; }
	wchar_t* getHelp() { return helpString; }

	bool isOperator() { return op; }

	unsigned int getNumParams();
	unsigned int getPrecedence() { return prec; }

	static void comNop(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam);
	static void comConsole(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam);
	static void comPushT(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam);
	static void comPopT(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam);
	static void comPushV(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam);
	static void comPopV(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam);
	static void comStackClear(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam);
};
#endif