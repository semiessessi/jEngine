/* inSystem class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#include "inSystem.h"
#include "jEngine.h"

#include "inKeyboard.h"
#include "inMouse.h"

#include "log.h"

inSystem* inSystem::p = 0;

inSystem::inSystem(jEngine* j)
{
	// constructor code for inSystem
	e=j;

	keyboard = inKeyboard::getPointer();
	mouse = inMouse::getPointer();

	ic = it;
#ifdef _DEBUG
	LogWriteLine(L"\x25CB\x25CB\x25CB inSystem constructor complete");
#endif
}

inSystem::inSystem(const inSystem& a)
{
	// copy constructor code for inSystem
}

inSystem& inSystem::operator=(const inSystem& a)
{
	// since we only want a single instance always return this 
	return (*this);
}

inSystem::~inSystem()
{
	// destructor code for inSystem
	delete keyboard;
	delete mouse;

#ifdef _DEBUG
	LogWriteLine(L"\x25CB\x25CB\x25CB inSystem destructor complete");
#endif
}

inSystem* inSystem::getPointer(jEngine* j)
{
	// return our instance of inSystem if its created 
	if(!p) p = new inSystem(j);
	return p;
}

void inSystem::inputCycle()
{
	ic();
}

void inSystem::changeKeyboardLayout()
{
	keyboard->changeKeyboardLayout();
}

void inSystem::wmCharHandler(unsigned int wParam, long lParam)
{
	keyboard->wmCharHandler(wParam, lParam);
}

void inSystem::wmKeyHandler(unsigned int wParam, long lParam)
{
	keyboard->wmKeyHandler(wParam, lParam);
}

void inSystem::it()
{
	p->keyboard->inputCycle();
	p->mouse->inputCycle();
}

inTextReciever* inSystem::getTextReciever()
{
	return keyboard->getTextReciever();
}

void inSystem::setTextReciever(inTextReciever* t)
{
	keyboard->setTextReciever(t);
}

inMouseReciever* inSystem::getMouseReciever()
{
	return mouse->getMouseReciever();
}

void inSystem::setMouseReciever(inMouseReciever* mr)
{
	mouse->setMouseReciever(mr);
}

int inSystem::getConsoleVKCode()
{
	return keyboard->getConsoleVKCode();
}