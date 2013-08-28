/* menuScriptRoutine class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#ifndef __MENUSCRIPTROUTINE_H
#define __MENUSCRIPTROUTINE_H

#include "jStack.h"

class menuCom;
class menuItem;
class menuScriptLine;
class menuSystem;
class menuVar;

class menuScriptRoutine
{
private:
	menuSystem*					m;
	menuScriptLine**			lines;
	int							numLines;
	menuItem*					parentItem;

	static jStack<menuVar>*		tStack;
	static jStack<menuVar>*		pStack;

	menuVar**					statics;
	int							numStatics;

	menuVar**					locals;
	int							numLocals;

	static unsigned int	codeLineNum;

	wchar_t* postfixFromInfix(const wchar_t* exp);
	void addLinesFromPostfix(const wchar_t* exp);
public:
	menuScriptRoutine(menuSystem* ms, menuItem* mi);
	menuScriptRoutine(menuSystem* ms, menuItem* mi, const wchar_t* code, menuVar* dt = 0);
	~menuScriptRoutine();

	void addLine(const wchar_t* funcName, menuCom* cParam, menuVar* vParam);

	void addLocal(menuVar* var);
	void addStatic(menuVar* var);

	bool isLocal(const wchar_t* name);
	menuVar* getLocal(const wchar_t* name);

	void Call();
};

#endif