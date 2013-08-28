/* Entry point code by Semi Essessi
 *
 * This is the starting point of execution
 *
 */

#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include <jShared.h>

#ifdef _DEBUG
#pragma comment(lib,"jShared_d.lib")
#else
#ifdef X64
#pragma comment(lib,"jShared_x64.lib")
#else
#pragma comment(lib,"jShared.lib")
#endif
#endif

#include "jEngine.h"
#include "log.h"

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR cmdLine, int nShow)
{
	if(jsHasInstanced()) return 0;

	jsInstance();
	jsCatchKeys(true);
#ifdef _DEBUG
	LogInit();
#endif
	jEngine* j = jEngine::getPointer();

	int ret = j->Run(hInst, GetCommandLineW());

	delete j;

#ifdef _DEBUG
	LogShutdown();
#endif

	jsDeInstance();
	jsCatchKeys(false);
	return ret;
}

/*
    workarounds to allow static linkage
*/

__declspec(dllexport) char* __imp__strncpy(char* _Dest, const char* _Source, size_t _Count)
{
    return strncpy(_Dest, _Source, _Count);
}

__declspec(dllexport) int __imp__fprintf(_Inout_ FILE * _File, _In_z_ _Printf_format_string_ const char * _Format, ...)
{
    va_list	 parg;
	va_start(parg, _Format);

    // allocate buffer
	int len = (_vscprintf(_Format, parg)+1);
	char* buf = new char[len];
	if(!buf) return 0;
	vsprintf(buf, _Format, parg);
	va_end(parg);

	len = fprintf(_File, buf);

	delete[] buf;

    return len;
}

__declspec(dllexport) void __imp__longjmp(_In_ jmp_buf _Buf, _In_ int _Value)
{
    longjmp(_Buf, _Value);
}

__declspec(dllexport) const void* __imp__memchr( _In_opt_bytecount_(_MaxCount) const void * _Buf , _In_ int _Val, _In_ size_t _MaxCount)
{
    return memchr(_Buf, _Val, _MaxCount);
}

__declspec(dllexport) void* __imp__memchr( _In_opt_bytecount_(_MaxCount) void * _Buf , _In_ int _Val, _In_ size_t _MaxCount)
{
    return memchr(_Buf, _Val, _MaxCount);
}

__declspec(dllexport) double __imp__strtod(_In_z_ const char * _Str, _Out_opt_ _Deref_post_z_ char ** _EndPtr)
{
    return strtod(_Str, _EndPtr);
}