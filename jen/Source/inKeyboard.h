/* inKeyboard class by Semi Essessi
 *
 * Keyboard handler
 *
 */

#ifndef __INKEYBOARD_H
#define __INKEYBOARD_H

#include <windows.h>

class inKeyReciever;
class inTextReciever;

class inKeyboard
{
private:
	bool				key[256];
	bool				oldKey[256];
	wchar_t*			keyLabel[256];

	HKL					keyLayout;

	inTextReciever*		text;
	void				(*ch)(unsigned int, long);
	void				(*kh)(unsigned int, long);

    inKeyReciever*		keyr;

	int					vkConsole;

	// pointer to single instance
	static inKeyboard*	p;

	// make constructors and assignment private to enforce single instance
	inKeyboard();
	inKeyboard(const inKeyboard& a);
	inKeyboard& operator=(const inKeyboard& a);

	// text handler helpers
	static void hn(unsigned int, long) { return; }
	static void hc(unsigned int, long);
	static void hk(unsigned int, long);
public:
	~inKeyboard();

	// return pointer to our single instance, or create it
	static inKeyboard* getPointer();

	void inputCycle();

	void changeKeyboardLayout();

	bool isKeyHeld(int nVirtKey) { return key[nVirtKey]; }
	bool isKeyHeldOverride(int nVirtKey) { return ((GetAsyncKeyState(nVirtKey) & 0x8000) ? true : false); }
	wchar_t* getKeyLabel(int nVirtKey) { return keyLabel[nVirtKey]; }
	int getConsoleVKCode() { return vkConsole; }

	inTextReciever* getTextReciever() { return text; }
	void setTextReciever(inTextReciever* t);

	inKeyReciever* getKeyReciever() { return keyr; }
	void setKeyReciever(inKeyReciever* k);

	void wmCharHandler(unsigned int wParam, long lParam);
	void wmKeyHandler(unsigned int wParam, long lParam);
};

#endif