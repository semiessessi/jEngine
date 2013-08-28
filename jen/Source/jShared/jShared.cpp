/* jShared library by Semi Essessi
 *
 * Some system global things for jEngine
 *
 */

#define _WIN32_WINNT 0x0500
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#pragma data_seg(".jShared")

bool instanced = false;
HHOOK hook = 0;

#pragma data_seg()

#pragma comment(linker, "/section:.jShared,RWS")

#define dllx __declspec(dllexport)

dllx void jsCatchKeys(bool enable);
dllx bool jsHasInstanced();
dllx void jsInstance();
dllx void jsDeInstance();

static HMODULE hInstance = 0;

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	hInstance = hinstDLL;
	return TRUE;
}

LRESULT CALLBACK Hook(int nCode, WPARAM wp, LPARAM lp)
{
	KBDLLHOOKSTRUCT* pkh = (KBDLLHOOKSTRUCT*)lp;

	if(nCode==HC_ACTION)
	{
		BOOL bCtrlKeyDown = (GetAsyncKeyState(VK_CONTROL) & 0x8000) ? true : false;

		if((pkh->vkCode==VK_ESCAPE && bCtrlKeyDown) ||
			(pkh->vkCode==VK_PAUSE && bCtrlKeyDown) ||
			(pkh->vkCode==VK_TAB && pkh->flags & LLKHF_ALTDOWN) ||
			(pkh->vkCode==VK_ESCAPE && pkh->flags & LLKHF_ALTDOWN)||
			(pkh->vkCode==VK_LWIN) || 
			(pkh->vkCode==VK_RWIN))
			return 1;
	}
	return CallNextHookEx(hook, nCode, wp, lp);
}

dllx void jsCatchKeys(bool enable)
{
	if(enable)
	{
		if(!hook) hook = SetWindowsHookEx(WH_KEYBOARD_LL, Hook, hInstance, 0);
	}
	else if(hook)
	{
		UnhookWindowsHookEx(hook);
		hook = 0;
	}
}

dllx bool jsHasInstanced()
{
	return instanced;
}

dllx void jsInstance()
{
	instanced = true;
}

dllx void jsDeInstance()
{
	instanced = false;
}