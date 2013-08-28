/* math wrapper by Semi Essessi
 *
 * Some helpful maths functions
 *
 */

#include "jMath.h"

#include "log.h"

#ifdef WIN32
#include <windows.h>
#endif

float sgn(float _x)
{
    return (_x < 0.0f) ? -1.0f : 1.0f;
}

bool ___hasMMX = false;
bool ___hasSSE = false;
bool ___hasSSE2 = false;
bool ___hasSSE3 = false;

void(*_add2i)(int a1, int a2, int b1, int b2, int* r1, int* r2)																					= _defAdd2i;
void(*_add2ip)(int* a, int* b, int* r)																											= _defAdd2ip;
void(*_add3i)(int a1, int a2, int a3, int b1, int b2, int b3, int* r1, int* r2, int* r3)														= _defAdd3i;
void(*_add3ip)(int* a, int* b, int* r)																											= _defAdd3ip;
void(*_add4i)(int a1, int a2, int a3, int a4, int b1, int b2, int b3, int b4, int* r1, int* r2, int* r3, int* r4)								= _defAdd4i;
void(*_add4ip)(int* a, int* b, int* r)																											= _defAdd4ip;

void(*_add2f)(float a1, float a2, float b1, float b2, float* r1, float* r2)																		= _defAdd2f;
void(*_add2fp)(float* a, float* b, float* r)																									= _defAdd2fp;
void(*_add3f)(float a1, float a2, float a3, float b1, float b2, float b3, float* r1, float* r2, float* r3)										= _defAdd3f;
void(*_add3fp)(float* a, float* b, float* r)																									= _defAdd3fp;
void(*_add4f)(float a1, float a2, float a3, float a4, float b1, float b2, float b3, float b4, float* r1, float* r2, float* r3, float* r4)		= _defAdd4f;
void(*_add4fp)(float* a, float* b, float* r)																									= _defAdd4fp;


void(*_sub2i)(int a1, int a2, int b1, int b2, int* r1, int* r2)																					= _defSub2i;
void(*_sub2ip)(int* a, int* b, int* r)																											= _defSub2ip;
void(*_sub3i)(int a1, int a2, int a3, int b1, int b2, int b3, int* r1, int* r2, int* r3)														= _defSub3i;
void(*_sub3ip)(int* a, int* b, int* r)																											= _defSub3ip;
void(*_sub4i)(int a1, int a2, int a3, int a4, int b1, int b2, int b3, int b4, int* r1, int* r2, int* r3, int* r4)								= _defSub4i;
void(*_sub4ip)(int* a, int* b, int* r)																											= _defSub4ip;

void(*_sub2f)(float a1, float a2, float b1, float b2, float* r1, float* r2)																		= _defSub2f;
void(*_sub2fp)(float* a, float* b, float* r)																									= _defSub2fp;
void(*_sub3f)(float a1, float a2, float a3, float b1, float b2, float b3, float* r1, float* r2, float* r3)										= _defSub3f;
void(*_sub3fp)(float* a, float* b, float* r)																									= _defSub3fp;
void(*_sub4f)(float a1, float a2, float a3, float a4, float b1, float b2, float b3, float b4, float* r1, float* r2, float* r3, float* r4)		= _defSub4f;
void(*_sub4fp)(float* a, float* b, float* r)																									= _defSub4fp;


void(*_mul2f)(float a1, float a2, float b1, float b2, float* r1, float* r2)																		= _defMul2f;
void(*_mul2fp)(float* a, float* b, float* r)																									= _defMul2fp;
void(*_mul3f)(float a1, float a2, float a3, float b1, float b2, float b3, float* r1, float* r2, float* r3)										= _defMul3f;
void(*_mul3fp)(float* a, float* b, float* r)																									= _defMul3fp;
void(*_mul4f)(float a1, float a2, float a3, float a4, float b1, float b2, float b3, float b4, float* r1, float* r2, float* r3, float* r4)		= _defMul4f;
void(*_mul4fp)(float* a, float* b, float* r)																									= _defMul4fp;

// check for MMX/SSE/SSE2/SSE3 using exceptions hax
bool _cpuHasMMX()
{
	int v1[4] = {1,2,3,4};
	int v2[4] = {4,3,2,1};
	int r[4] = {0,0,0,0};
	bool ret = true;
#ifdef WIN32
	__try
	{
		_mmxAdd2ip(v1,v2,r);
		_mmxAdd3ip(v1,v2,r);
		_mmxAdd4ip(v1,v2,r);
		_mmxSub2ip(v1,v2,r);
		_mmxSub3ip(v1,v2,r);
		_mmxSub4ip(v1,v2,r);
	}
	__except(1)
	{
		ret=false;
	}
#else
	ret = false;
#endif
	
	return ret;
}

bool _cpuHasSSE()
{
	float v1[4] = {1.0f,2.0f,3.0f,4.0f};
	float v2[4] = {4.0f,3.0f,2.0f,1.0f};
	float r[4] = {0.0f,0.0f,0.0f,0.0f};
	bool ret = true;
#ifdef WIN32
	__try
	{
		_sseAdd2fp(v1,v2,r);
		_sseAdd3fp(v1,v2,r);
		_sseAdd4fp(v1,v2,r);
		_sseSub2fp(v1,v2,r);
		_sseSub3fp(v1,v2,r);
		_sseSub4fp(v1,v2,r);
		_sseMul2fp(v1,v2,r);
		_sseMul3fp(v1,v2,r);
		_sseMul4fp(v1,v2,r);
	}
	__except(1)
	{
		ret=false;
	}
#else
	ret = false;
#endif
	
	return ret;
}

bool _cpuHasSSE2()
{
	int v1[4] = {1,2,3,4};
	int v2[4] = {4,3,2,1};
	int r[4] = {0,0,0,0};
	bool ret = true;
#ifdef WIN32
	__try
	{
		_sse2Add3ip(v1,v2,r);
		_sse2Add4ip(v1,v2,r);
		_sse2Sub3ip(v1,v2,r);
		_sse2Sub4ip(v1,v2,r);
	}
	__except(1)
	{
		ret=false;
	}
#else
	ret = false;
#endif

	return ret;
}

bool _cpuHasSSE3()
{
	float v1[4] = {1.0,2.0,3.0,4.0};
	float v2[4] = {4.0,3.0,2.0,1.0};
	bool ret = true;
#ifdef WIN32
	__try
	{
		__asm
		{
			movdqu		xmm0, [v1]
			movdqu		xmm1, [v2]
			haddps		(_xmm0, _xmm1)
			movdqu		[v1], xmm0
		}
	}
	__except(1)
	{
		ret=false;
	}
#else
	ret = false;
#endif
	
	return ret;
}

// get memory info...
int getTotalRAM()
{
	int ret = 0;
#ifdef WIN32
	DWORD v = GetVersion();
	DWORD major = (DWORD)(LOBYTE(LOWORD(v)));
	DWORD minor = (DWORD)(HIBYTE(LOWORD(v)));
	DWORD build;
	if (v < 0x80000000) build = (DWORD)(HIWORD(v));
	else build = 0;
	
	// because compiler static links the function...
	BOOL (__stdcall*GMSEx)(LPMEMORYSTATUSEX) = 0;

	HINSTANCE hIL = LoadLibrary(L"kernel32.dll");
	GMSEx = (BOOL(__stdcall*)(LPMEMORYSTATUSEX))GetProcAddress(hIL, "GlobalMemoryStatusEx");

	if(GMSEx)
	{
		MEMORYSTATUSEX m;
		m.dwLength = sizeof(m);
		if(GMSEx(&m))
		{
			ret = (int)(m.ullTotalPhys>>20);
		}
	}
	else
	{
		MEMORYSTATUS m;
		m.dwLength = sizeof(m);
		GlobalMemoryStatus(&m);
		ret = (int)(m.dwTotalPhys>>20);
	}
#endif
	return ret;
}

int getAvailRAM()
{
	int ret = 0;
#ifdef WIN32
	DWORD v = GetVersion();
	DWORD major =  (DWORD)(LOBYTE(LOWORD(v)));
	DWORD minor =  (DWORD)(HIBYTE(LOWORD(v)));
	DWORD build;
	if (v < 0x80000000) build = (DWORD)(HIWORD(v));
	else build = 0;

	// because compiler static links the function...
	BOOL (__stdcall*GMSEx)(LPMEMORYSTATUSEX) = 0;

	HINSTANCE hIL = LoadLibrary(L"kernel32.dll");
	GMSEx = (BOOL(__stdcall*)(LPMEMORYSTATUSEX))GetProcAddress(hIL, "GlobalMemoryStatusEx");

	if(GMSEx)
	{
		MEMORYSTATUSEX m;
		m.dwLength = sizeof(m);
		if(GMSEx(&m))
		{
			ret = (int)(m.ullAvailPhys>>20);
		}
	}
	else
	{
		MEMORYSTATUS m;
		m.dwLength = sizeof(m);
		GlobalMemoryStatus(&m);
		ret = (int)(m.dwAvailPhys>>20);
	}
#endif
	return ret;
}

int getTotalMemory()
{
	int ret = 0;
#ifdef WIN32
	DWORD v = GetVersion();
	DWORD major =  (DWORD)(LOBYTE(LOWORD(v)));
	DWORD minor =  (DWORD)(HIBYTE(LOWORD(v)));
	DWORD build;
	if (v < 0x80000000) build = (DWORD)(HIWORD(v));
	else build = 0;
	
	// because compiler static links the function...
	BOOL (__stdcall*GMSEx)(LPMEMORYSTATUSEX) = 0;

	HINSTANCE hIL = LoadLibrary(L"kernel32.dll");
	GMSEx = (BOOL(__stdcall*)(LPMEMORYSTATUSEX))GetProcAddress(hIL, "GlobalMemoryStatusEx");

	if(GMSEx)
	{
		MEMORYSTATUSEX m;
		m.dwLength = sizeof(m);
		if(GMSEx(&m))
		{
			ret = (int)(m.ullTotalPhys>>20) + (int)(m.ullTotalVirtual>>20);
		}
	}
	else
	{
		MEMORYSTATUS m;
		m.dwLength = sizeof(m);
		GlobalMemoryStatus(&m);
		ret = (int)(m.dwTotalPhys>>20) + (int)(m.dwTotalVirtual>>20);
	}
#endif
	return ret;
}

int getAvailMemory()
{
	int ret = 0;
#ifdef WIN32
	DWORD v = GetVersion();
	DWORD major =  (DWORD)(LOBYTE(LOWORD(v)));
	DWORD minor =  (DWORD)(HIBYTE(LOWORD(v)));
	DWORD build;
	if (v < 0x80000000) build = (DWORD)(HIWORD(v));
	else build = 0;
	
	// because compiler static links the function...
	BOOL (__stdcall*GMSEx)(LPMEMORYSTATUSEX) = 0;

	HINSTANCE hIL = LoadLibrary(L"kernel32.dll");
	GMSEx = (BOOL(__stdcall*)(LPMEMORYSTATUSEX))GetProcAddress(hIL, "GlobalMemoryStatusEx");

	if(GMSEx)
	{
		MEMORYSTATUSEX m;
		m.dwLength = sizeof(m);
		if(GMSEx(&m))
		{
			ret = (int)(m.ullAvailPhys>>20) + (int)(m.ullAvailVirtual>>20);
		}
	}
	else
	{
		MEMORYSTATUS m;
		m.dwLength = sizeof(m);
		GlobalMemoryStatus(&m);
		ret = (int)(m.dwAvailPhys>>20) + (int)(m.dwAvailVirtual>>20);
	}
#endif
	return ret;
}

void jMathInit()
{
	bool hasMMX = _cpuHasMMX();
	bool hasSSE = _cpuHasSSE();
	bool hasSSE2 = _cpuHasSSE2();
	bool hasSSE3 = _cpuHasSSE3();

	___hasMMX = hasMMX;
	___hasSSE = hasSSE;
	___hasSSE2 = hasSSE2;
	___hasSSE3 = hasSSE3;

	if(hasMMX)
	{
		_add2i = _mmxAdd2i;
		_add2ip = _mmxAdd2ip;
		_add3i = _mmxAdd3i;
		_add3ip = _mmxAdd3ip;
		_add4i = _mmxAdd4i;
		_add4ip = _mmxAdd4ip;

		_sub2i = _mmxSub2i;
		_sub2ip = _mmxSub2ip;
		_sub3i = _mmxSub3i;
		_sub3ip = _mmxSub3ip;
		_sub4i = _mmxSub4i;
		_sub4ip = _mmxSub4ip;
	}
	if(hasSSE)
	{
		_add2f = _sseAdd2f;
		_add2fp = _sseAdd2fp;
		_add3f = _sseAdd3f;
		_add3fp = _sseAdd3fp;
		_add4f = _sseAdd4f;
		_add4fp = _sseAdd4fp;

		_sub2f = _sseSub2f;
		_sub2fp = _sseSub2fp;
		_sub3f = _sseSub3f;
		_sub3fp = _sseSub3fp;
		_sub4f = _sseSub4f;
		_sub4fp = _sseSub4fp;

		_mul2f = _sseMul2f;
		_mul2fp = _sseMul2fp;
		_mul3f = _sseMul3f;
		_mul3fp = _sseMul3fp;
		_mul4f = _sseMul4f;
		_mul4fp = _sseMul4fp;
	}
	if(hasSSE2)
	{
		_add3i = _sse2Add3i;
		_add3ip = _sse2Add3ip;
		_add4i = _sse2Add4i;
		_add4ip = _sse2Add4ip;

		_sub3i = _sse2Sub3i;
		_sub3ip = _sse2Sub3ip;
		_sub4i = _sse2Sub4i;
		_sub4ip = _sse2Sub4ip;
	}
}

bool cpuHasMMX()
{
	return ___hasMMX;
}

bool cpuHasSSE()
{
	return ___hasSSE;
}

bool cpuHasSSE2()
{
	return ___hasSSE2;
}

bool cpuHasSSE3()
{
	return ___hasSSE3;
}

// power of two bithax useful for textures and stuff
int roundPow2(int x)
{
	int r = x;
	r--;
	r |= r >> 1;
	r |= r >> 2;
	r |= r >> 4;
	r |= r >> 8;
	r |= r >> 16;
	r++;

	if(!r) r++;

	return r;
}

int mulPow2(int x, int pow)
{
	return (x<<pow);
}

int divPow2(int x, int pow)
{
	return (x>>pow);
}

// SIMD functions
void Add2i(int a1, int a2, int b1, int b2, int* r1, int* r2)
{
	_add2i(a1,a2,b1,b2,r1,r2);
}

void Add2i(int* a, int* b, int* r)
{
	_add2ip(a,b,r);
}

void Add3i(int a1, int a2, int a3, int b1, int b2, int b3, int* r1, int* r2, int* r3)
{
	_add3i(a1,a2,3,b1,b2,b3,r1,r2,r3);
}

void Add3i(int* a, int* b, int* r)
{
	_add3ip(a,b,r);
}

void Add4i(int a1, int a2, int a3, int a4, int b1, int b2, int b3, int b4, int* r1, int* r2, int* r3, int* r4)
{
	_add4i(a1,a2,a3,a4,b1,b2,b3,b4,r1,r2,r3,r4);
}

void Add4i(int* a, int* b, int* r)
{
	_add4ip(a,b,r);
}

void Add2f(float a1, float a2, float b1, float b2, float* r1, float* r2)
{
	_add2f(a1,a2,b1,b2,r1,r2);
}

void Add2f(float* a, float* b, float* r)
{
	_add2fp(a,b,r);
}

void Add3f(float a1, float a2, float a3, float b1, float b2, float b3, float* r1, float* r2, float* r3)
{
	_add3f(a1,a2,3,b1,b2,b3,r1,r2,r3);
}

void Add3f(float* a, float* b, float* r)
{
	_add3fp(a,b,r);
}

void Add4f(float a1, float a2, float a3, float a4, float b1, float b2, float b3, float b4, float* r1, float* r2, float* r3, float* r4)
{
	_add4f(a1,a2,a3,a4,b1,b2,b3,b4,r1,r2,r3,r4);
}

void Add4f(float* a, float* b, float* r)
{
	_add4fp(a,b,r);
}

void Sub2i(int a1, int a2, int b1, int b2, int* r1, int* r2)
{
	_sub2i(a1,a2,b1,b2,r1,r2);
}

void Sub2i(int* a, int* b, int* r)
{
	_sub2ip(a,b,r);
}

void Sub3i(int a1, int a2, int a3, int b1, int b2, int b3, int* r1, int* r2, int* r3)
{
	_sub3i(a1,a2,3,b1,b2,b3,r1,r2,r3);
}

void Sub3i(int* a, int* b, int* r)
{
	_sub3ip(a,b,r);
}

void Sub4i(int a1, int a2, int a3, int a4, int b1, int b2, int b3, int b4, int* r1, int* r2, int* r3, int* r4)
{
	_sub4i(a1,a2,a3,a4,b1,b2,b3,b4,r1,r2,r3,r4);
}

void Sub4i(int* a, int* b, int* r)
{
	_sub4ip(a,b,r);
}

void Sub2f(float a1, float a2, float b1, float b2, float* r1, float* r2)
{
	_sub2f(a1,a2,b1,b2,r1,r2);
}

void Sub2f(float* a, float* b, float* r)
{
	_sub2fp(a,b,r);
}

void Sub3f(float a1, float a2, float a3, float b1, float b2, float b3, float* r1, float* r2, float* r3)
{
	_sub3f(a1,a2,3,b1,b2,b3,r1,r2,r3);
}

void Sub3f(float* a, float* b, float* r)
{
	_sub3fp(a,b,r);
}

void Sub4f(float a1, float a2, float a3, float a4, float b1, float b2, float b3, float b4, float* r1, float* r2, float* r3, float* r4)
{
	_sub4f(a1,a2,a3,a4,b1,b2,b3,b4,r1,r2,r3,r4);
}

void Sub4f(float* a, float* b, float* r)
{
	_sub4fp(a,b,r);
}

void Mul2f(float a1, float a2, float b1, float b2, float* r1, float* r2)
{
	_mul2f(a1,a2,b1,b2,r1,r2);
}

void Mul2f(float* a, float* b, float* r)
{
	_mul2fp(a,b,r);
}

void Mul3f(float a1, float a2, float a3, float b1, float b2, float b3, float* r1, float* r2, float* r3)
{
	_mul3f(a1,a2,3,b1,b2,b3,r1,r2,r3);
}

void Mul3f(float* a, float* b, float* r)
{
	_mul3fp(a,b,r);
}

void Mul4f(float a1, float a2, float a3, float a4, float b1, float b2, float b3, float b4, float* r1, float* r2, float* r3, float* r4)
{
	_mul4f(a1,a2,a3,a4,b1,b2,b3,b4,r1,r2,r3,r4);
}

void Mul4f(float* a, float* b, float* r)
{
	_mul4fp(a,b,r);
}

// SIMD additions
void _defAdd2i(int a1, int a2, int b1, int b2, int* r1, int* r2)
{
	*r1=a1+b1;
	*r2=a2+b2;
}

void _mmxAdd2i(int a1, int a2, int b1, int b2, int* r1, int* r2)
{
	int a[2] = {a1,a2};
	int b[2] = {b1,b2};

	int r[2] = {0,0};
	
	__asm
	{
		mov			eax,  a
		mov			ebx,  b
		mov			ecx,  r
		movq		mm0, [eax]
		paddd		mm0, [ebx]
		movq		[ecx], mm0
		emms
	}

	*r1=r[0];
	*r2=r[1];
}

void _defAdd2ip(int* a, int* b, int* r)
{
	r[0]=a[0]+b[0];
	r[1]=a[1]+b[1];
}

void _mmxAdd2ip(int* a, int* b, int* r)
{
	__asm
	{
		mov			eax,  a
		mov			ebx,  b
		mov			ecx,  r
		movq		mm0, [eax]
		paddd		mm0, [ebx]
		movq		[ecx], mm0
		emms
	}
}

void _defAdd3i(int a1, int a2, int a3, int b1, int b2, int b3, int* r1, int* r2, int* r3)
{
	*r1=a1+b1;
	*r2=a2+b2;
	*r3=a3+b3;
}

void _mmxAdd3i(int a1, int a2, int a3, int b1, int b2, int b3, int* r1, int* r2, int* r3)
{
	int a[2] = {a1,a2};
	int b[2] = {b1,b2};

	int r[2] = {0,0};
	
	__asm
	{
		mov			eax,  a
		mov			ebx,  b
		mov			ecx,  r
		movq		mm0, [eax]
		paddd		mm0, [ebx]
		movq		[ecx], mm0
		emms
	}

	*r1=r[0];
	*r2=r[1];
	*r3=a3+b3;
}

void _sse2Add3i(int a1, int a2, int a3, int b1, int b2, int b3, int* r1, int* r2, int* r3)
{
	int a[3] = {a1,a2,a3};
	int b[3] = {b1,b2,b3};
	int r[3] = {0,0,0};
	
	__asm
	{
		mov			eax, a
		mov			ebx, b
		mov			ecx, r
		movd		xmm2, [eax+8]
		movq		xmm0, qword ptr [eax]
		pshufd		xmm2, xmm2, 01001110B		// 0,1,2,3 -> 2,3,0,1
		pxor		xmm0, xmm2
		movd		xmm2, [ebx+8]
		movq		xmm1, qword ptr [ebx]
		pshufd		xmm2, xmm2, 01001110B		// 0,1,2,3 -> 2,3,0,1
		pxor		xmm1, xmm2
		paddd		xmm0, xmm1
		movdqu		[ecx], xmm0
	}

	*r1=r[0];
	*r2=r[1];
	*r3=r[2];
}

void _defAdd3ip(int* a, int* b, int* r)
{
	r[0]=a[0]+b[0];
	r[1]=a[1]+b[1];
	r[2]=a[2]+b[2];
}

void _mmxAdd3ip(int* a, int* b, int* r)
{
	__asm
	{
		mov			eax,  a
		mov			ebx,  b
		mov			ecx,  r
		movq		mm0, [eax]
		paddd		mm0, [ebx]
		movq		[ecx], mm0
		emms
	}
	r[2]=a[2]+b[2];
}

void _sse2Add3ip(int* a, int* b, int* r)
{
	__asm
	{
		mov			eax, a
		mov			ebx, b
		mov			ecx, r
		movd		xmm2, [eax+8]
		movq		xmm0, qword ptr [eax]
		pshufd		xmm2, xmm2, 01001110B		// 0,1,2,3 -> 2,3,0,1
		pxor		xmm0, xmm2
		movd		xmm2, [ebx+8]
		movq		xmm1, qword ptr [ebx]
		pshufd		xmm2, xmm2, 01001110B		// 0,1,2,3 -> 2,3,0,1
		pxor		xmm1, xmm2
		paddd		xmm0, xmm1
		movdqu		[ecx], xmm0
	}	
}

void _defAdd4i(int a1, int a2, int a3, int a4, int b1, int b2, int b3, int b4, int* r1, int* r2, int* r3, int* r4)
{
	*r1=a1+b1;
	*r2=a2+b2;
	*r3=a3+b3;
	*r4=a4+b4;
}

void _defAdd4ip(int* a, int* b, int* r)
{
	r[0]=a[0]+b[0];
	r[1]=a[1]+b[1];
	r[2]=a[2]+b[2];
	r[3]=a[3]+b[3];
}

void _mmxAdd4i(int a1, int a2, int a3, int a4, int b1, int b2, int b3, int b4, int* r1, int* r2, int* r3, int* r4)
{
	int a[4] = {a1,a2,a3,a4};
	int b[4] = {b1,b2,b3,b4};
	int r[4] = {0,0,0,0};
	
	__asm
	{
		mov			eax,  a
		mov			ebx,  b
		mov			ecx,  r
		movq		mm0, qword ptr [eax]
		movq		mm1, qword ptr [eax+8]
		paddd		mm0, qword ptr [ebx]
		paddd		mm1, qword ptr [ebx+8]
		movq		qword ptr [ecx], mm0
		movq		qword ptr [ecx+8], mm1
		emms
	}

	*r1=r[0];
	*r2=r[1];
	*r3=r[2];
	*r4=r[3];
}

void _mmxAdd4ip(int* a, int* b, int* r)
{
	__asm
	{
		mov			eax,  a
		mov			ebx,  b
		mov			ecx,  r
		movq		mm0, qword ptr [eax]
		movq		mm1, qword ptr [eax+8]
		paddd		mm0, qword ptr [ebx]
		paddd		mm1, qword ptr [ebx+8]
		movq		qword ptr [ecx], mm0
		movq		qword ptr [ecx+8], mm1
		emms
	}
}

void _sse2Add4i(int a1, int a2, int a3, int a4, int b1, int b2, int b3, int b4, int* r1, int* r2, int* r3, int* r4)
{
	int a[4] = {a1,a2,a3,a4};
	int b[4] = {b1,b2,b3,b4};
	int r[4] = {0,0,0,0};
	
	__asm
	{
		mov			eax,  a
		mov			ebx,  b
		mov			ecx,  r
		movdqu		xmm0, [eax]
		movdqu		xmm1, [ebx]
		paddd		xmm0, xmm1
		movdqu		[ecx], xmm0
	}

	*r1=r[0];
	*r2=r[1];
	*r3=r[2];
	*r4=r[3];
}

void _sse2Add4ip(int* a, int* b, int* r)
{
	__asm
	{
		mov			eax,  a
		mov			ebx,  b
		mov			ecx,  r
		movdqu		xmm0, [eax]
		movdqu		xmm1, [ebx]
		paddd		xmm0, xmm1
		movdqu		[ecx], xmm0
	}
}

void _defAdd2f(float a1, float a2, float b1, float b2, float* r1, float* r2)
{
	*r1=a1+b1;
	*r2=a2+b2;
}

void _sseAdd2f(float a1, float a2, float b1, float b2, float* r1, float* r2)
{
	float a[2] = {a1,a2};
	float b[2] = {b1,b2};

	float r[2] = {0,0};
	
	__asm
	{
		mov			eax,  a
		mov			ebx,  b
		mov			ecx,  r
		movlps		xmm0, [eax]
		movlps		xmm1, [ebx]
		addps		xmm0, xmm1
		movlps		[ecx], xmm0
	}

	*r1=r[0];
	*r2=r[1];
}

void _defAdd2fp(float* a, float* b, float* r)
{
	r[0]=a[0]+b[0];
	r[1]=a[1]+b[1];
}

void _sseAdd2fp(float* a, float* b, float* r)
{
	__asm
	{
		mov			eax,  a
		mov			ebx,  b
		mov			ecx,  r
		movlps		xmm0, [eax]
		movlps		xmm1, [ebx]
		addps		xmm0, xmm1
		movlps		[ecx], xmm0
	}
}

void _defAdd3f(float a1, float a2, float a3, float b1, float b2, float b3, float* r1, float* r2, float* r3)
{
	*r1=a1+b1;
	*r2=a2+b2;
	*r3=a3+b3;
}

void _sseAdd3f(float a1, float a2, float a3, float b1, float b2, float b3, float* r1, float* r2, float* r3)
{
	float a[3] = {a1,a2,a3};
	float b[3] = {b1,b2,b3};
	float r[3] = {0,0,0};
	
	__asm
	{
		mov			eax, a
		mov			ebx, b
		mov			ecx, r
		movhps		xmm0, [eax]
		movss		xmm0, [eax+4]
		movhps		xmm1, [ebx]
		movss		xmm0, [ebx+4]
		addps		xmm0, xmm1
		movhps		[ecx], xmm0
		movss		[ecx+4], xmm0
	}

	*r1=r[0];
	*r2=r[1];
	*r3=r[2];
}

void _defAdd3fp(float* a, float* b, float* r)
{
	r[0]=a[0]+b[0];
	r[1]=a[1]+b[1];
	r[2]=a[2]+b[2];
}

void _sseAdd3fp(float* a, float* b, float* r)
{
	__asm
	{
		mov			eax, a
		mov			ebx, b
		mov			ecx, r
		movhps		xmm0, [eax]
		movss		xmm0, [eax+4]
		movhps		xmm1, [ebx]
		movss		xmm0, [ebx+4]
		addps		xmm0, xmm1
		movhps		[ecx], xmm0
		movss		[ecx+4], xmm0
	}	
}

void _defAdd4f(float a1, float a2, float a3, float a4, float b1, float b2, float b3, float b4, float* r1, float* r2, float* r3, float* r4)
{
	*r1=a1+b1;
	*r2=a2+b2;
	*r3=a3+b3;
	*r4=a4+b4;
}

void _defAdd4fp(float* a, float* b, float* r)
{
	r[0]=a[0]+b[0];
	r[1]=a[1]+b[1];
	r[2]=a[2]+b[2];
	r[3]=a[3]+b[3];
}

void _sseAdd4f(float a1, float a2, float a3, float a4, float b1, float b2, float b3, float b4, float* r1, float* r2, float* r3, float* r4)
{
	float a[4] = {a1,a2,a3,a4};
	float b[4] = {b1,b2,b3,b4};
	float r[4] = {0,0,0,0};
	
	__asm
	{
		mov			eax,  a
		mov			ebx,  b
		mov			ecx,  r
		movups		xmm0, [eax]
		movups		xmm1, [ebx]
		addps		xmm0, xmm1
		movups		[ecx], xmm0
	}

	*r1=r[0];
	*r2=r[1];
	*r3=r[2];
	*r4=r[3];
}

void _sseAdd4fp(float* a, float* b, float* r)
{
	__asm
	{
		mov			eax,  a
		mov			ebx,  b
		mov			ecx,  r
		movups		xmm0, [eax]
		movups		xmm1, [ebx]
		addps		xmm0, xmm1
		movups		[ecx], xmm0
	}
}

// SIMD subtractions
void _defSub2i(int a1, int a2, int b1, int b2, int* r1, int* r2)
{
	*r1=a1-b1;
	*r2=a2-b2;
}

void _mmxSub2i(int a1, int a2, int b1, int b2, int* r1, int* r2)
{
	int a[2] = {a1,a2};
	int b[2] = {b1,b2};

	int r[2] = {0,0};
	
	__asm
	{
		mov			eax,  a
		mov			ebx,  b
		mov			ecx,  r
		movq		mm0, [eax]
		psubd		mm0, [ebx]
		movq		[ecx], mm0
		emms
	}

	*r1=r[0];
	*r2=r[1];
}

void _defSub2ip(int* a, int* b, int* r)
{
	r[0]=a[0]-b[0];
	r[1]=a[1]-b[1];
}

void _mmxSub2ip(int* a, int* b, int* r)
{
	__asm
	{
		mov			eax,  a
		mov			ebx,  b
		mov			ecx,  r
		movq		mm0, [eax]
		psubd		mm0, [ebx]
		movq		[ecx], mm0
		emms
	}
}

void _defSub3i(int a1, int a2, int a3, int b1, int b2, int b3, int* r1, int* r2, int* r3)
{
	*r1=a1-b1;
	*r2=a2-b2;
	*r3=a3-b3;
}

void _mmxSub3i(int a1, int a2, int a3, int b1, int b2, int b3, int* r1, int* r2, int* r3)
{
	int a[2] = {a1,a2};
	int b[2] = {b1,b2};

	int r[2] = {0,0};
	
	__asm
	{
		mov			eax,  a
		mov			ebx,  b
		mov			ecx,  r
		movq		mm0, [eax]
		psubd		mm0, [ebx]
		movq		[ecx], mm0
		emms
	}

	*r1=r[0];
	*r2=r[1];
	*r3=a3-b3;
}

void _sse2Sub3i(int a1, int a2, int a3, int b1, int b2, int b3, int* r1, int* r2, int* r3)
{
	int a[3] = {a1,a2,a3};
	int b[3] = {b1,b2,b3};
	int r[3] = {0,0,0};
	
	__asm
	{
		mov			eax, a
		mov			ebx, b
		mov			ecx, r
		movd		xmm2, [eax+8]
		movq		xmm0, qword ptr [eax]
		pshufd		xmm2, xmm2, 01001110B		// 0,1,2,3 -> 2,3,0,1
		pxor		xmm0, xmm2
		movd		xmm2, [ebx+8]
		movq		xmm1, qword ptr [ebx]
		pshufd		xmm2, xmm2, 01001110B		// 0,1,2,3 -> 2,3,0,1
		pxor		xmm1, xmm2
		psubd		xmm0, xmm1
		movdqu		[ecx], xmm0
	}

	*r1=r[0];
	*r2=r[1];
	*r3=r[2];
}

void _defSub3ip(int* a, int* b, int* r)
{
	r[0]=a[0]-b[0];
	r[1]=a[1]-b[1];
	r[2]=a[2]-b[2];
}

void _mmxSub3ip(int* a, int* b, int* r)
{
	__asm
	{
		mov			eax,  a
		mov			ebx,  b
		mov			ecx,  r
		movq		mm0, [eax]
		psubd		mm0, [ebx]
		movq		[ecx], mm0
		emms
	}
	r[2]=a[2]-b[2];
}

void _sse2Sub3ip(int* a, int* b, int* r)
{
	__asm
	{
		mov			eax, a
		mov			ebx, b
		mov			ecx, r
		movd		xmm2, [eax+8]
		movq		xmm0, qword ptr [eax]
		pshufd		xmm2, xmm2, 01001110B		// 0,1,2,3 -> 2,3,0,1
		pxor		xmm0, xmm2
		movd		xmm2, [ebx+8]
		movq		xmm1, qword ptr [ebx]
		pshufd		xmm2, xmm2, 01001110B		// 0,1,2,3 -> 2,3,0,1
		pxor		xmm1, xmm2
		psubd		xmm0, xmm1
		movdqu		[ecx], xmm0
	}	
}

void _defSub4i(int a1, int a2, int a3, int a4, int b1, int b2, int b3, int b4, int* r1, int* r2, int* r3, int* r4)
{
	*r1=a1-b1;
	*r2=a2-b2;
	*r3=a3-b3;
	*r4=a4-b4;
}

void _defSub4ip(int* a, int* b, int* r)
{
	r[0]=a[0]-b[0];
	r[1]=a[1]-b[1];
	r[2]=a[2]-b[2];
	r[3]=a[3]-b[3];
}

void _mmxSub4i(int a1, int a2, int a3, int a4, int b1, int b2, int b3, int b4, int* r1, int* r2, int* r3, int* r4)
{
	int a[4] = {a1,a2,a3,a4};
	int b[4] = {b1,b2,b3,b4};
	int r[4] = {0,0,0,0};
	
	__asm
	{
		mov			eax,  a
		mov			ebx,  b
		mov			ecx,  r
		movq		mm0, qword ptr [eax]
		movq		mm1, qword ptr [eax+8]
		psubd		mm0, qword ptr [ebx]
		psubd		mm1, qword ptr [ebx+8]
		movq		qword ptr [ecx], mm0
		movq		qword ptr [ecx+8], mm1
		emms
	}

	*r1=r[0];
	*r2=r[1];
	*r3=r[2];
	*r4=r[3];
}

void _mmxSub4ip(int* a, int* b, int* r)
{
	__asm
	{
		mov			eax,  a
		mov			ebx,  b
		mov			ecx,  r
		movq		mm0, qword ptr [eax]
		movq		mm1, qword ptr [eax+8]
		psubd		mm0, qword ptr [ebx]
		psubd		mm1, qword ptr [ebx+8]
		movq		qword ptr [ecx], mm0
		movq		qword ptr [ecx+8], mm1
		emms
	}
}

void _sse2Sub4i(int a1, int a2, int a3, int a4, int b1, int b2, int b3, int b4, int* r1, int* r2, int* r3, int* r4)
{
	int a[4] = {a1,a2,a3,a4};
	int b[4] = {b1,b2,b3,b4};
	int r[4] = {0,0,0,0};
	
	__asm
	{
		mov			eax,  a
		mov			ebx,  b
		mov			ecx,  r
		movdqu		xmm0, [eax]
		movdqu		xmm1, [ebx]
		psubd		xmm0, xmm1
		movdqu		[ecx], xmm0
	}

	*r1=r[0];
	*r2=r[1];
	*r3=r[2];
	*r4=r[3];
}

void _sse2Sub4ip(int* a, int* b, int* r)
{
	__asm
	{
		mov			eax,  a
		mov			ebx,  b
		mov			ecx,  r
		movdqu		xmm0, [eax]
		movdqu		xmm1, [ebx]
		psubd		xmm0, xmm1
		movdqu		[ecx], xmm0
	}
}

void _defSub2f(float a1, float a2, float b1, float b2, float* r1, float* r2)
{
	*r1=a1-b1;
	*r2=a2-b2;
}

void _sseSub2f(float a1, float a2, float b1, float b2, float* r1, float* r2)
{
	float a[2] = {a1,a2};
	float b[2] = {b1,b2};

	float r[2] = {0,0};
	
	__asm
	{
		mov			eax,  a
		mov			ebx,  b
		mov			ecx,  r
		movlps		xmm0, [eax]
		movlps		xmm1, [ebx]
		subps		xmm0, xmm1
		movlps		[ecx], xmm0
	}

	*r1=r[0];
	*r2=r[1];
}

void _defSub2fp(float* a, float* b, float* r)
{
	r[0]=a[0]-b[0];
	r[1]=a[1]-b[1];
}

void _sseSub2fp(float* a, float* b, float* r)
{
	__asm
	{
		mov			eax,  a
		mov			ebx,  b
		mov			ecx,  r
		movlps		xmm0, [eax]
		movlps		xmm1, [ebx]
		subps		xmm0, xmm1
		movlps		[ecx], xmm0
	}
}

void _defSub3f(float a1, float a2, float a3, float b1, float b2, float b3, float* r1, float* r2, float* r3)
{
	*r1=a1-b1;
	*r2=a2-b2;
	*r3=a3-b3;
}

void _sseSub3f(float a1, float a2, float a3, float b1, float b2, float b3, float* r1, float* r2, float* r3)
{
	float a[3] = {a1,a2,a3};
	float b[3] = {b1,b2,b3};
	float r[3] = {0,0,0};
	
	__asm
	{
		mov			eax, a
		mov			ebx, b
		mov			ecx, r
		movhps		xmm0, [eax]
		movss		xmm0, [eax+4]
		movhps		xmm1, [ebx]
		movss		xmm0, [ebx+4]
		subps		xmm0, xmm1
		movhps		[ecx], xmm0
		movss		[ecx+4], xmm0
	}

	*r1=r[0];
	*r2=r[1];
	*r3=r[2];
}

void _defSub3fp(float* a, float* b, float* r)
{
	r[0]=a[0]-b[0];
	r[1]=a[1]-b[1];
	r[2]=a[2]-b[2];
}

void _sseSub3fp(float* a, float* b, float* r)
{
	__asm
	{
		mov			eax, a
		mov			ebx, b
		mov			ecx, r
		movhps		xmm0, [eax]
		movss		xmm0, [eax+4]
		movhps		xmm1, [ebx]
		movss		xmm0, [ebx+4]
		subps		xmm0, xmm1
		movhps		[ecx], xmm0
		movss		[ecx+4], xmm0
	}	
}

void _defSub4f(float a1, float a2, float a3, float a4, float b1, float b2, float b3, float b4, float* r1, float* r2, float* r3, float* r4)
{
	*r1=a1-b1;
	*r2=a2-b2;
	*r3=a3-b3;
	*r4=a4-b4;
}

void _defSub4fp(float* a, float* b, float* r)
{
	r[0]=a[0]-b[0];
	r[1]=a[1]-b[1];
	r[2]=a[2]-b[2];
	r[3]=a[3]-b[3];
}

void _sseSub4f(float a1, float a2, float a3, float a4, float b1, float b2, float b3, float b4, float* r1, float* r2, float* r3, float* r4)
{
	float a[4] = {a1,a2,a3,a4};
	float b[4] = {b1,b2,b3,b4};
	float r[4] = {0,0,0,0};
	
	__asm
	{
		mov			eax,  a
		mov			ebx,  b
		mov			ecx,  r
		movups		xmm0, [eax]
		movups		xmm1, [ebx]
		subps		xmm0, xmm1
		movups		[ecx], xmm0
	}

	*r1=r[0];
	*r2=r[1];
	*r3=r[2];
	*r4=r[3];
}

void _sseSub4fp(float* a, float* b, float* r)
{
	__asm
	{
		mov			eax,  a
		mov			ebx,  b
		mov			ecx,  r
		movups		xmm0, [eax]
		movups		xmm1, [ebx]
		subps		xmm0, xmm1
		movups		[ecx], xmm0
	}
}

// SIMD multiplications
void _defMul2f(float a1, float a2, float b1, float b2, float* r1, float* r2)
{
	*r1=a1*b1;
	*r2=a2*b2;
}

void _sseMul2f(float a1, float a2, float b1, float b2, float* r1, float* r2)
{
	float a[2] = {a1,a2};
	float b[2] = {b1,b2};

	float r[2] = {0,0};
	
	__asm
	{
		mov			eax,  a
		mov			ebx,  b
		mov			ecx,  r
		movlps		xmm0, [eax]
		movlps		xmm1, [ebx]
		mulps		xmm0, xmm1
		movlps		[ecx], xmm0
	}

	*r1=r[0];
	*r2=r[1];
}

void _defMul2fp(float* a, float* b, float* r)
{
	r[0]=a[0]*b[0];
	r[1]=a[1]*b[1];
}

void _sseMul2fp(float* a, float* b, float* r)
{
	__asm
	{
		mov			eax,  a
		mov			ebx,  b
		mov			ecx,  r
		movlps		xmm0, [eax]
		movlps		xmm1, [ebx]
		mulps		xmm0, xmm1
		movlps		[ecx], xmm0
	}
}

void _defMul3f(float a1, float a2, float a3, float b1, float b2, float b3, float* r1, float* r2, float* r3)
{
	*r1=a1*b1;
	*r2=a2*b2;
	*r3=a3*b3;
}

void _sseMul3f(float a1, float a2, float a3, float b1, float b2, float b3, float* r1, float* r2, float* r3)
{
	float a[3] = {a1,a2,a3};
	float b[3] = {b1,b2,b3};
	float r[3] = {0,0,0};
	
	__asm
	{
		mov			eax, a
		mov			ebx, b
		mov			ecx, r
		movhps		xmm0, [eax]
		movss		xmm0, [eax+4]
		movhps		xmm1, [ebx]
		movss		xmm0, [ebx+4]
		mulps		xmm0, xmm1
		movhps		[ecx], xmm0
		movss		[ecx+4], xmm0
	}

	*r1=r[0];
	*r2=r[1];
	*r3=r[2];
}

void _defMul3fp(float* a, float* b, float* r)
{
	r[0]=a[0]*b[0];
	r[1]=a[1]*b[1];
	r[2]=a[2]*b[2];
}

void _sseMul3fp(float* a, float* b, float* r)
{
	__asm
	{
		mov			eax, a
		mov			ebx, b
		mov			ecx, r
		movhps		xmm0, [eax]
		movss		xmm0, [eax+4]
		movhps		xmm1, [ebx]
		movss		xmm0, [ebx+4]
		mulps		xmm0, xmm1
		movhps		[ecx], xmm0
		movss		[ecx+4], xmm0
	}	
}

void _defMul4f(float a1, float a2, float a3, float a4, float b1, float b2, float b3, float b4, float* r1, float* r2, float* r3, float* r4)
{
	*r1=a1*b1;
	*r2=a2*b2;
	*r3=a3*b3;
	*r4=a4*b4;
}

void _defMul4fp(float* a, float* b, float* r)
{
	r[0]=a[0]*b[0];
	r[1]=a[1]*b[1];
	r[2]=a[2]*b[2];
	r[3]=a[3]*b[3];
}

void _sseMul4f(float a1, float a2, float a3, float a4, float b1, float b2, float b3, float b4, float* r1, float* r2, float* r3, float* r4)
{
	float a[4] = {a1,a2,a3,a4};
	float b[4] = {b1,b2,b3,b4};
	float r[4] = {0,0,0,0};
	
	__asm
	{
		mov			eax,  a
		mov			ebx,  b
		mov			ecx,  r
		movups		xmm0, [eax]
		movups		xmm1, [ebx]
		mulps		xmm0, xmm1
		movups		[ecx], xmm0
	}

	*r1=r[0];
	*r2=r[1];
	*r3=r[2];
	*r4=r[3];
}

void _sseMul4fp(float* a, float* b, float* r)
{
	__asm
	{
		mov			eax,  a
		mov			ebx,  b
		mov			ecx,  r
		movups		xmm0, [eax]
		movups		xmm1, [ebx]
		mulps		xmm0, xmm1
		movups		[ecx], xmm0
	}
}

float ipLinear(const float& f1, const float& f2, const float& x)
{
    return f1 - (f1 + f2)*x;
}

float ipCosine(const float& f1, const float& f2, const float& x)
{
    return f1 - (f1 + f2)*(1 - cos(x*3.1415926536f))*0.5f;
}

float ipCubic(const float& f1, const float& f2, const float& f3, const float& f4, const float& x)
{
    /*
    x2 = x*x;
    x3 = x*x*x;
    a0 = f4 - f3 - f1 + f2;
    a1 = f1 - f2 - a0;
    a2 = f3 - f1;
    a3 = f2;

    return a0*x3 + a1*x2 + a2*x + a3;
    */

    float a = (f4 - f3);
    float b = (f1 - f2);
    a -= b;
    b -= a;

    return ((a*x + b)*x + f3 - f1)*x + f2;
}

double ipLinear(const double& f1, const double& f2, const double& x)
{
    return f1 - (f1 + f2)*x;
}

double ipCosine(const double& f1, const double& f2, const double& x)
{
    return f1 - (f1 + f2)*(1 - cos(x*3.1415926536f))*0.5f;
}

double ipCubic(const double& f1, const double& f2, const double& f3, const double& f4, const double& x)
{
    double a = (f4 - f3);
    double b = (f1 - f2);
    a -= b;
    b -= a;

    return ((a*x + b)*x + f3 - f1)*x + f2;
}