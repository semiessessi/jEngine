/* math wrapper by Semi Essessi
 *
 * Some helpful maths functions
 *
 */
#ifndef __JMATH_H
#define __JMATH_H

/*
****************************      *                              | | |
                                *                                | | |
need to fix 3f functions!!!!  ***************************        ! ! !
                                *
****************************      *                              * * *
*/

#include <math.h>

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

// register codes
#define _xmm0	0xC0
#define _xmm1	0xC1
#define _xmm2	0xC2
#define _xmm3	0xC3
#define _xmm4	0xC4
#define _xmm5	0xC5
#define _xmm6	0xC6
#define _xmm7	0xC7

// sse3 opcodes
#define addsubps( reg1, reg2 ) \
	__asm _emit 0xF2 \
	__asm _emit 0x0F \
	__asm _emit 0xD0 \
	__asm _emit ( ( reg1 & 7 ) << 3 ) | reg2

#define addsubpd( reg1, reg2 ) \
	__asm _emit 0x66 \
	__asm _emit 0x0F \
	__asm _emit 0xD0 \
	__asm _emit ( ( reg1 & 7 ) << 3 ) | reg2

#define haddps( reg1, reg2 ) \
	__asm _emit 0xF2 \
	__asm _emit 0x0F \
	__asm _emit 0x7C \
	__asm _emit ( ( reg1 & 7 ) << 3 ) | reg2

#define haddpd( reg1, reg2 ) \
	__asm _emit 0x66 \
	__asm _emit 0x0F \
	__asm _emit 0x7C \
	__asm _emit ( ( reg1 & 7 ) << 3 ) | reg2


#define hsubps( reg1, reg2 ) \
	__asm _emit 0xF2 \
	__asm _emit 0x0F \
	__asm _emit 0x7D \
	__asm _emit ( ( reg1 & 7 ) << 3 ) | reg2

#define hsubpd( reg1, reg2 ) \
	__asm _emit 0x66 \
	__asm _emit 0x0F \
	__asm _emit 0x7D \
	__asm _emit ( ( reg1 & 7 ) << 3 ) | reg2

#define lddqu( reg1, reg2 ) \
	__asm _emit 0xF2 \
	__asm _emit 0x0F \
	__asm _emit 0xF0 \
	__asm _emit ( ( reg1 & 7 ) << 3 ) | reg2

#define movsldup( reg1, reg2 ) \
	__asm _emit 0xF3 \
	__asm _emit 0x0F \
	__asm _emit 0x12 \
	__asm _emit ( ( reg1 & 7 ) << 3 ) | reg2

#define movdldup( reg1, reg2 ) \
	__asm _emit 0xF2 \
	__asm _emit 0x0F \
	__asm _emit 0x12 \
	__asm _emit ( ( reg1 & 7 ) << 3 ) | reg2

#define movshdup( reg1, reg2 ) \
	__asm _emit 0xF3 \
	__asm _emit 0x0F \
	__asm _emit 0x16 \
	__asm _emit ( ( reg1 & 7 ) << 3 ) | reg2

#define movdhdup( reg1, reg2 ) \
	__asm _emit 0xF2 \
	__asm _emit 0x0F \
	__asm _emit 0x16 \
	__asm _emit ( ( reg1 & 7 ) << 3 ) | reg2

// function predefines...
void jMathInit();

bool cpuHasMMX();
bool cpuHasSSE();
bool cpuHasSSE2();
bool cpuHasSSE3();

int getTotalRAM();
int getAvailRAM();
int getTotalMemory();
int getAvailMemory();

// bithax
int roundPow2(int x);
int mulPow2(int x, int pow);
int divPow2(int x, int pow);

// math
float sgn(float _x);

// crap
#define cDOT(a,b) a[0]*b[0] + a[1]*b[1] + a[2]*b[2]
#define cDEG2RAD(a) 3.14159265f*a/180.0f
#ifndef PI
#define PI 3.14159265f
#endif

// simd stuff

void Add2i(int a1, int a2, int b1, int b2, int* r1, int* r2);
void Add2i(int* a, int* b, int* r);
void Add3i(int a1, int a2, int a3, int b1, int b2, int b3, int* r1, int* r2, int* r3);
void Add3i(int* a, int* b, int* r);
void Add4i(int a1, int a2, int a3, int a4, int b1, int b2, int b3, int b4, int* r1, int* r2, int* r3, int* r4);
void Add4i(int* a, int* b, int* r);

void Add2f(float a1, float a2, float b1, float b2, float* r1, float* r2);
void Add2f(float* a, float* b, float* r);
void Add3f(float a1, float a2, float a3, float b1, float b2, float b3, float* r1, float* r2, float* r3);
void Add3f(float* a, float* b, float* r);
void Add4f(float a1, float a2, float a3, float a4, float b1, float b2, float b3, float b4, float* r1, float* r2, float* r3, float* r4);
void Add4f(float* a, float* b, float* r);


void Sub2i(int a1, int a2, int b1, int b2, int* r1, int* r2);
void Sub2i(int* a, int* b, int* r);
void Sub3i(int a1, int a2, int a3, int b1, int b2, int b3, int* r1, int* r2, int* r3);
void Sub3i(int* a, int* b, int* r);
void Sub4i(int a1, int a2, int a3, int a4, int b1, int b2, int b3, int b4, int* r1, int* r2, int* r3, int* r4);
void Sub4i(int* a, int* b, int* r);

void Sub2f(float a1, float a2, float b1, float b2, float* r1, float* r2);
void Sub2f(float* a, float* b, float* r);
void Sub3f(float a1, float a2, float a3, float b1, float b2, float b3, float* r1, float* r2, float* r3);
void Sub3f(float* a, float* b, float* r);
void Sub4f(float a1, float a2, float a3, float a4, float b1, float b2, float b3, float b4, float* r1, float* r2, float* r3, float* r4);
void Sub4f(float* a, float* b, float* r);


void Mul2f(float a1, float a2, float b1, float b2, float* r1, float* r2);
void Mul2f(float* a, float* b, float* r);
void Mul3f(float a1, float a2, float a3, float b1, float b2, float b3, float* r1, float* r2, float* r3);
void Mul3f(float* a, float* b, float* r);
void Mul4f(float a1, float a2, float a3, float a4, float b1, float b2, float b3, float b4, float* r1, float* r2, float* r3, float* r4);
void Mul4f(float* a, float* b, float* r);

// bools for functionaity and functions for implementation
extern bool ___hasMMX;
extern bool ___hasSSE;
extern bool ___hasSSE2;
extern bool ___hasSSE3;

void _defAdd2i(int a1, int a2, int b1, int b2, int* r1, int* r2);
void _mmxAdd2i(int a1, int a2, int b1, int b2, int* r1, int* r2);
void _defAdd2ip(int* a, int* b, int* r);
void _mmxAdd2ip(int* a, int* b, int* r);
void _defAdd3i(int a1, int a2, int a3, int b1, int b2, int b3, int* r1, int* r2, int* r3);
void _mmxAdd3i(int a1, int a2, int a3, int b1, int b2, int b3, int* r1, int* r2, int* r3);
void _sse2Add3i(int a1, int a2, int a3, int b1, int b2, int b3, int* r1, int* r2, int* r3);
void _defAdd3ip(int* a, int* b, int* r);
void _mmxAdd3ip(int* a, int* b, int* r);
void _sse2Add3ip(int* a, int* b, int* r);
void _defAdd4i(int a1, int a2, int a3, int a4, int b1, int b2, int b3, int b4, int* r1, int* r2, int* r3, int* r4);
void _defAdd4ip(int* a, int* b, int* r);
void _mmxAdd4i(int a1, int a2, int a3, int a4, int b1, int b2, int b3, int b4, int* r1, int* r2, int* r3, int* r4);
void _mmxAdd4ip(int* a, int* b, int* r);
void _sse2Add4i(int a1, int a2, int a3, int a4, int b1, int b2, int b3, int b4, int* r1, int* r2, int* r3, int* r4);
void _sse2Add4ip(int* a, int* b, int* r);

void _defAdd2f(float a1, float a2, float b1, float b2, float* r1, float* r2);
void _sseAdd2f(float a1, float a2, float b1, float b2, float* r1, float* r2);
void _defAdd2fp(float* a, float* b, float* r);
void _sseAdd2fp(float* a, float* b, float* r);
void _defAdd3f(float a1, float a2, float a3, float b1, float b2, float b3, float* r1, float* r2, float* r3);
void _sseAdd3f(float a1, float a2, float a3, float b1, float b2, float b3, float* r1, float* r2, float* r3);
void _defAdd3fp(float* a, float* b, float* r);
void _sseAdd3fp(float* a, float* b, float* r);
void _defAdd4f(float a1, float a2, float a3, float a4, float b1, float b2, float b3, float b4, float* r1, float* r2, float* r3, float* r4);
void _defAdd4fp(float* a, float* b, float* r);
void _sseAdd4f(float a1, float a2, float a3, float a4, float b1, float b2, float b3, float b4, float* r1, float* r2, float* r3, float* r4);
void _sseAdd4fp(float* a, float* b, float* r);


void _defSub2i(int a1, int a2, int b1, int b2, int* r1, int* r2);
void _mmxSub2i(int a1, int a2, int b1, int b2, int* r1, int* r2);
void _defSub2ip(int* a, int* b, int* r);
void _mmxSub2ip(int* a, int* b, int* r);
void _defSub3i(int a1, int a2, int a3, int b1, int b2, int b3, int* r1, int* r2, int* r3);
void _mmxSub3i(int a1, int a2, int a3, int b1, int b2, int b3, int* r1, int* r2, int* r3);
void _sse2Sub3i(int a1, int a2, int a3, int b1, int b2, int b3, int* r1, int* r2, int* r3);
void _defSub3ip(int* a, int* b, int* r);
void _mmxSub3ip(int* a, int* b, int* r);
void _sse2Sub3ip(int* a, int* b, int* r);
void _defSub4i(int a1, int a2, int a3, int a4, int b1, int b2, int b3, int b4, int* r1, int* r2, int* r3, int* r4);
void _defSub4ip(int* a, int* b, int* r);
void _mmxSub4i(int a1, int a2, int a3, int a4, int b1, int b2, int b3, int b4, int* r1, int* r2, int* r3, int* r4);
void _mmxSub4ip(int* a, int* b, int* r);
void _sse2Sub4i(int a1, int a2, int a3, int a4, int b1, int b2, int b3, int b4, int* r1, int* r2, int* r3, int* r4);
void _sse2Sub4ip(int* a, int* b, int* r);

void _defSub2f(float a1, float a2, float b1, float b2, float* r1, float* r2);
void _sseSub2f(float a1, float a2, float b1, float b2, float* r1, float* r2);
void _defSub2fp(float* a, float* b, float* r);
void _sseSub2fp(float* a, float* b, float* r);
void _defSub3f(float a1, float a2, float a3, float b1, float b2, float b3, float* r1, float* r2, float* r3);
void _sseSub3f(float a1, float a2, float a3, float b1, float b2, float b3, float* r1, float* r2, float* r3);
void _defSub3fp(float* a, float* b, float* r);
void _sseSub3fp(float* a, float* b, float* r);
void _defSub4f(float a1, float a2, float a3, float a4, float b1, float b2, float b3, float b4, float* r1, float* r2, float* r3, float* r4);
void _defSub4fp(float* a, float* b, float* r);
void _sseSub4f(float a1, float a2, float a3, float a4, float b1, float b2, float b3, float b4, float* r1, float* r2, float* r3, float* r4);
void _sseSub4fp(float* a, float* b, float* r);


void _defMul2f(float a1, float a2, float b1, float b2, float* r1, float* r2);
void _sseMul2f(float a1, float a2, float b1, float b2, float* r1, float* r2);
void _defMul2fp(float* a, float* b, float* r);
void _sseMul2fp(float* a, float* b, float* r);
void _defMul3f(float a1, float a2, float a3, float b1, float b2, float b3, float* r1, float* r2, float* r3);
void _sseMul3f(float a1, float a2, float a3, float b1, float b2, float b3, float* r1, float* r2, float* r3);
void _defMul3fp(float* a, float* b, float* r);
void _sseMul3fp(float* a, float* b, float* r);
void _defMul4f(float a1, float a2, float a3, float a4, float b1, float b2, float b3, float b4, float* r1, float* r2, float* r3, float* r4);
void _defMul4fp(float* a, float* b, float* r);
void _sseMul4f(float a1, float a2, float a3, float a4, float b1, float b2, float b3, float b4, float* r1, float* r2, float* r3, float* r4);
void _sseMul4fp(float* a, float* b, float* r);

float ipLinear(const float& f1, const float& f2, const float& x);
float ipCosine(const float& f1, const float& f2, const float& x);
float ipCubic(const float& f1, const float& f2, const float& f3, const float& f4, const float& x);

double ipLinear(const double& f1, const double& f2, const double& x);
double ipCosine(const double& f1, const double& f2, const double& x);
double ipCubic(const double& f1, const double& f2, const double& f3, const double& f4, const double& x);


extern void(*_add2i)(int a1, int a2, int b1, int b2, int* r1, int* r2);
extern void(*_add2ip)(int* a, int* b, int* r);
extern void(*_add3i)(int a1, int a2, int a3, int b1, int b2, int b3, int* r1, int* r2, int* r3);
extern void(*_add3ip)(int* a, int* b, int* r);
extern void(*_add4i)(int a1, int a2, int a3, int a4, int b1, int b2, int b3, int b4, int* r1, int* r2, int* r3, int* r4);
extern void(*_add4ip)(int* a, int* b, int* r);

extern void(*_add2f)(float a1, float a2, float b1, float b2, float* r1, float* r2);
extern void(*_add2fp)(float* a, float* b, float* r);
extern void(*_add3f)(float a1, float a2, float a3, float b1, float b2, float b3, float* r1, float* r2, float* r3);
extern void(*_add3fp)(float* a, float* b, float* r);
extern void(*_add4f)(float a1, float a2, float a3, float a4, float b1, float b2, float b3, float b4, float* r1, float* r2, float* r3, float* r4);
extern void(*_add4fp)(float* a, float* b, float* r);


extern void(*_sub2i)(int a1, int a2, int b1, int b2, int* r1, int* r2);
extern void(*_sub2ip)(int* a, int* b, int* r);
extern void(*_sub3i)(int a1, int a2, int a3, int b1, int b2, int b3, int* r1, int* r2, int* r3);
extern void(*_sub3ip)(int* a, int* b, int* r);
extern void(*_sub4i)(int a1, int a2, int a3, int a4, int b1, int b2, int b3, int b4, int* r1, int* r2, int* r3, int* r4);
extern void(*_sub4ip)(int* a, int* b, int* r);

extern void(*_sub2f)(float a1, float a2, float b1, float b2, float* r1, float* r2);
extern void(*_sub2fp)(float* a, float* b, float* r);
extern void(*_sub3f)(float a1, float a2, float a3, float b1, float b2, float b3, float* r1, float* r2, float* r3);
extern void(*_sub3fp)(float* a, float* b, float* r);
extern void(*_sub4f)(float a1, float a2, float a3, float a4, float b1, float b2, float b3, float b4, float* r1, float* r2, float* r3, float* r4);
extern void(*_sub4fp)(float* a, float* b, float* r);


extern void(*_mul2f)(float a1, float a2, float b1, float b2, float* r1, float* r2);
extern void(*_mul2fp)(float* a, float* b, float* r);
extern void(*_mul3f)(float a1, float a2, float a3, float b1, float b2, float b3, float* r1, float* r2, float* r3);
extern void(*_mul3fp)(float* a, float* b, float* r);
extern void(*_mul4f)(float a1, float a2, float a3, float a4, float b1, float b2, float b3, float b4, float* r1, float* r2, float* r3, float* r4);
extern void(*_mul4fp)(float* a, float* b, float* r);

#endif