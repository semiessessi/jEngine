/* inSystem class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#ifndef __INSYSTEM_H
#define __INSYSTEM_H

class jEngine;

class inKeyboard;
class inMouse;

class inKeyReciever;
class inMouseReciever;
class inTextReciever;

class inSystem
{
private:
	// pointer to top-level class
	jEngine*	e;

	// input devices
	inKeyboard*	keyboard;
	inMouse*	mouse;
	
	void				(*ic)();

	// pointer to single instance
	static inSystem* p;

	// make constructors and assignment private to enforce single instance
	inSystem(jEngine* j);
	inSystem(const inSystem& a);
	inSystem& operator=(const inSystem& a);

	// text handler helpers
	static void in() { return; }
	static void it();
public:
	~inSystem();

	// return pointer to our single instance, or create it
	static inSystem* getPointer(jEngine* j);

	inKeyboard* getKeyboard() { return keyboard; }
	inMouse* getMouse() { return mouse; }

    inKeyReciever* getKeyReciever();
	void setKeyReciever(inKeyReciever* t);

	inTextReciever* getTextReciever();
	void setTextReciever(inTextReciever* t);

	inMouseReciever* getMouseReciever();
	void setMouseReciever(inMouseReciever* mr);

	int getConsoleVKCode();

	void inputCycle();
	void changeKeyboardLayout();

	void wmCharHandler(unsigned int wParam, long lParam);
	void wmKeyHandler(unsigned int wParam, long lParam);
};

#endif