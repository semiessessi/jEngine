/* inKeyboard class by Semi Essessi
 *
* Keyboard handler
 *
 */

#include "jEngine.h"
#include "inKeyboard.h"
#include "inTextReciever.h"

#include "console.h"
#include "log.h"

#include <stdio.h>
#include <windows.h>

inKeyboard* inKeyboard::p = 0;

inKeyboard::inKeyboard()
{
	for(int i=0;i<256;i++)
	{
		key[i]=false;
		oldKey[i]=false;

		keyLabel[i]=0;
	}

	text = 0;
	ch = hn;
	kh = hn;

	keyr = 0;

	vkConsole = 0;

	changeKeyboardLayout();
	// constructor code for inKeyboard
#ifdef _DEBUG
	LogWriteLine(L"\x25CB\x25CB\x25CB inKeyboard constructor complete");
#endif
}

inKeyboard::inKeyboard(const inKeyboard& a)
{
	// copy constructor code for inKeyboard
}

inKeyboard& inKeyboard::operator=(const inKeyboard& a)
{
	// since we only want a single instance always return this 
	return (*this);
}

inKeyboard::~inKeyboard()
{
	// destructor code for inKeyboard
    for(int i=0;i<256;i++)
	{
        if(keyLabel[i]) free(keyLabel[i]);
    }
#ifdef _DEBUG
	LogWriteLine(L"\x25CB\x25CB\x25CB inKeyboard destructor complete");
#endif
}

inKeyboard* inKeyboard::getPointer()
{
	// return our instance of inKeyboard if its created 
	if(!p) p = new inKeyboard();
	return p;
}

void inKeyboard::inputCycle()
{
	for(int i=0;i<256;i++)
	{
		oldKey[i]=key[i];
	}

	for(int i=0;i<256;i++)
	{
		key[i] = (GetAsyncKeyState(i) & 0x8000) ? true : false;
	}

	//TODO::something better about this special key handling
	// i.e. write key bind system
	if(key[VK_ESCAPE])
    {
        //PostQuitMessage(0);
        jEngine::getPointer()->con->executeLine(L"loadmenu main");
    }
	if(key[vkConsole]&&!oldKey[vkConsole]) jEngine::getPointer()->con->toggleDown();
}

void inKeyboard::changeKeyboardLayout()
{
	keyLayout = GetKeyboardLayout(0);

	// update key names
	BYTE fakeKeys[256];
	
	for(int i=0;i<256;i++)
	{
		fakeKeys[i]=0;
	}

	// get console key code
	vkConsole = MapVirtualKeyEx(0x29u,1,keyLayout);

	for(int i=0;i<256;i++)
	{
		wchar_t buf[256];

		fakeKeys[i]=0xFF;
		int ret = ToUnicodeEx((UINT)i, 0, fakeKeys, buf, 256, 0, keyLayout);
		fakeKeys[i]=0;

		if(i==VK_RMENU)
		{
			keyLabel[i] = (wchar_t*)realloc(keyLabel[i],sizeof(wchar_t)*(wcslen(L"Right Alt")+1));
			keyLabel[i] = wcscpy(keyLabel[i],L"Right Alt");
		}
		else if(i==VK_LMENU)
		{
			keyLabel[i] = (wchar_t*)realloc(keyLabel[i],sizeof(wchar_t)*(wcslen(L"Left Alt")+1));
			keyLabel[i] = wcscpy(keyLabel[i],L"Left Alt");
		}
		else if(i==VK_RCONTROL)
		{
			keyLabel[i] = (wchar_t*)realloc(keyLabel[i],sizeof(wchar_t)*(wcslen(L"Right Control")+1));
			keyLabel[i] = wcscpy(keyLabel[i],L"Right Control");
		}
		else if(i==VK_LCONTROL)
		{
			keyLabel[i] = (wchar_t*)realloc(keyLabel[i],sizeof(wchar_t)*(wcslen(L"Left Control")+1));
			keyLabel[i] = wcscpy(keyLabel[i],L"Left Control");
		}
		else if(i==VK_RSHIFT)
		{
			keyLabel[i] = (wchar_t*)realloc(keyLabel[i],sizeof(wchar_t)*(wcslen(L"Right Shift")+1));
			keyLabel[i] = wcscpy(keyLabel[i],L"Right Shift");
		}
		else if(i==VK_LSHIFT)
		{
			keyLabel[i] = (wchar_t*)realloc(keyLabel[i],sizeof(wchar_t)*(wcslen(L"Left Shift")+1));
			keyLabel[i] = wcscpy(keyLabel[i],L"Left Shift");
		}
		else if(i==VK_LEFT)
		{
			keyLabel[i] = (wchar_t*)realloc(keyLabel[i],sizeof(wchar_t)*(wcslen(L"Left")+1));
			keyLabel[i] = wcscpy(keyLabel[i],L"Left");
		}
		else if(i==VK_RIGHT)
		{
			keyLabel[i] = (wchar_t*)realloc(keyLabel[i],sizeof(wchar_t)*(wcslen(L"Right")+1));
			keyLabel[i] = wcscpy(keyLabel[i],L"Right");
		}
		else if(i==VK_UP)
		{
			keyLabel[i] = (wchar_t*)realloc(keyLabel[i],sizeof(wchar_t)*(wcslen(L"Up")+1));
			keyLabel[i] = wcscpy(keyLabel[i],L"Up");
		}
		else if(i==VK_DOWN)
		{
			keyLabel[i] = (wchar_t*)realloc(keyLabel[i],sizeof(wchar_t)*(wcslen(L"Down")+1));
			keyLabel[i] = wcscpy(keyLabel[i],L"Down");
		}
		else if((i>=VK_F1)&&(i<=VK_F24))
		{
			wchar_t tmp[8];
			swprintf(tmp, L"F%i", (i-111));
			keyLabel[i] = (wchar_t*)realloc(keyLabel[i],sizeof(wchar_t)*(wcslen(tmp)+1));
			keyLabel[i] = wcscpy(keyLabel[i],tmp);
		}
		else if(ret)
		{
			keyLabel[i] = (wchar_t*)realloc(keyLabel[i],sizeof(wchar_t)*(wcslen(buf)+1));
			keyLabel[i] = wcscpy(keyLabel[i],buf);
		}
		else
		{
			keyLabel[i] = (wchar_t*)realloc(keyLabel[i],sizeof(wchar_t)*4);
			keyLabel[i] = wcscpy(keyLabel[i],L"???");
		}
	}

}

void inKeyboard::wmCharHandler(unsigned int wParam, long lParam)
{
	ch(wParam, lParam);
}

void inKeyboard::wmKeyHandler(unsigned int wParam, long lParam)
{
	kh(wParam, lParam);
}

void inKeyboard::hc(unsigned int wParam, long lParam)
{
	p->text->wmCharHandler(wParam, lParam);
}

void inKeyboard::hk(unsigned int wParam, long lParam)
{
	p->text->wmKeyHandler(wParam, lParam);
}

void inKeyboard::setTextReciever(inTextReciever* t)
{
	text = t;
	
	if(text)
	{
		ch = hc;
		kh = hk;
	}
	else
	{
		ch = hn;
		kh = hn;
	}
}

void inKeyboard::setKeyReciever(inKeyReciever* k)
{
	keyr = k;

	ch = hn;
	kh = hn;

	if(keyr)
	{
		//...
	}
	else
	{
		//...
	}
}