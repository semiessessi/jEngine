/* menuVar class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#ifndef __MENUVAR_H
#define __MENUVAR_H

#include "jStack.h"

class menuCom;
class menuItem;
class strSystem;

class menuVar
{
private:
	static strSystem*	str;

	wchar_t*			name;
	char				type;

	union
	{
		bool				bVal;
		float				fVal;
		int					iVal;
	};

	wchar_t*			sVal;

	void*				vPtr;

	bool				(*gBool)(void*);
	float				(*gFloat)(void*);
	int					(*gInt)(void*);
	wchar_t*			(*gString)(void*, wchar_t*&);

	void				(*sBool)(void*, bool);
	void				(*sFloat)(void*, float);
	void				(*sInt)(void*, int);
	void				(*sString)(void*, const wchar_t*);

	void				(*ass)(menuVar*, menuVar*);
	void				(*add)(menuItem*, menuVar*, menuVar*, menuVar*&);
	void				(*sub)(menuItem*, menuVar*, menuVar*, menuVar*&);
	void				(*mul)(menuItem*, menuVar*, menuVar*, menuVar*&);
	void				(*div)(menuItem*, menuVar*, menuVar*, menuVar*&);

	//TODO::++a --a a++ a--
	void				(*preinc)(menuItem*, menuVar*, menuVar*&);
	void				(*predec)(menuItem*, menuVar*, menuVar*&);
	void				(*postinc)(menuItem*, menuVar*, menuVar*&);
	void				(*postdec)(menuItem*, menuVar*, menuVar*&);
	
	//TODO::a += b a -= b a *= b a /= b a &= b a |= b a ^= b
	void				(*addeq)(menuItem*, menuVar*, menuVar*, menuVar*&);
	void				(*subeq)(menuItem*, menuVar*, menuVar*, menuVar*&);
	void				(*muleq)(menuItem*, menuVar*, menuVar*, menuVar*&);
	void				(*diveq)(menuItem*, menuVar*, menuVar*, menuVar*&);
	void				(*andeq)(menuItem*, menuVar*, menuVar*, menuVar*&);
	void				(*oreq)(menuItem*, menuVar*, menuVar*, menuVar*&);
	void				(*xoreq)(menuItem*, menuVar*, menuVar*, menuVar*&);
	
	void				(*cond)(menuItem*, menuVar*, menuVar*, menuVar*, menuVar*&);
	void				(*equal)(menuItem*, menuVar*, menuVar*, menuVar*&);
	void				(*notequal)(menuItem*, menuVar*, menuVar*, menuVar*&);
	void				(*less)(menuItem*, menuVar*, menuVar*, menuVar*&);
	void				(*greater)(menuItem*, menuVar*, menuVar*, menuVar*&);
	void				(*lequal)(menuItem*, menuVar*, menuVar*, menuVar*&);
	void				(*gequal)(menuItem*, menuVar*, menuVar*, menuVar*&);
	void				(*and)(menuItem*, menuVar*, menuVar*, menuVar*&);
	void				(*or)(menuItem*, menuVar*, menuVar*, menuVar*&);
	void				(*not)(menuItem*, menuVar*, menuVar*&);
	void				(*xor)(menuItem*, menuVar*, menuVar*, menuVar*&);

	void				(*sin)(menuItem*, menuVar*, menuVar*&);
	void				(*cos)(menuItem*, menuVar*, menuVar*&);
	void				(*tan)(menuItem*, menuVar*, menuVar*&);
	void				(*atan)(menuItem*, menuVar*, menuVar*&);
	void				(*asin)(menuItem*, menuVar*, menuVar*&);
	void				(*acos)(menuItem*, menuVar*, menuVar*&);

	void				(*sqrt)(menuItem*, menuVar*, menuVar*&);
	void				(*exp)(menuItem*, menuVar*, menuVar*&);
	void				(*log)(menuItem*, menuVar*, menuVar*&);
	void				(*log10)(menuItem*, menuVar*, menuVar*&);
	void				(*invsqrt)(menuItem*, menuVar*, menuVar*&);
	void				(*sqr)(menuItem*, menuVar*, menuVar*&);

	static bool nGetBool(void* p);
	static bool bGetBool(void* p);
	static bool fGetBool(void* p);
	static bool iGetBool(void* p);
	static bool sGetBool(void* p);
	static bool vGetBool(void* p);

	static float nGetFloat(void* p);
	static float bGetFloat(void* p);
	static float fGetFloat(void* p);
	static float iGetFloat(void* p);
	static float sGetFloat(void* p);
	static float vGetFloat(void* p);

	static int nGetInt(void* p);
	static int bGetInt(void* p);
	static int fGetInt(void* p);
	static int iGetInt(void* p);
	static int sGetInt(void* p);
	static int vGetInt(void* p);

	static wchar_t* nGetString(void* p, wchar_t*& sv);
	static wchar_t* bGetString(void* p, wchar_t*& sv);
	static wchar_t* fGetString(void* p, wchar_t*& sv);
	static wchar_t* iGetString(void* p, wchar_t*& sv);
	static wchar_t* sGetString(void* p, wchar_t*& sv);
	static wchar_t* vGetString(void* p, wchar_t*& sv);

	static void nSetBool(void* p, bool b);
	static void bSetBool(void* p, bool b);
	static void fSetBool(void* p, bool b);
	static void iSetBool(void* p, bool b);
	static void sSetBool(void* p, bool b);
	static void vSetBool(void* p, bool b);

	static void nSetFloat(void* p, float f);
	static void bSetFloat(void* p, float f);
	static void fSetFloat(void* p, float f);
	static void iSetFloat(void* p, float f);
	static void sSetFloat(void* p, float f);
	static void vSetFloat(void* p, float f);

	static void nSetInt(void* p, int i);
	static void bSetInt(void* p, int i);
	static void fSetInt(void* p, int i);
	static void iSetInt(void* p, int i);
	static void sSetInt(void* p, int i);
	static void vSetInt(void* p, int i);

	static void nSetString(void* p, const wchar_t* sv);
	static void bSetString(void* p, const wchar_t* sv);
	static void fSetString(void* p, const wchar_t* sv);
	static void iSetString(void* p, const wchar_t* sv);
	static void sSetString(void* p, const wchar_t* sv);
	static void vSetString(void* p, const wchar_t* sv);

	static void nAssign(menuVar* v1, menuVar* v2);
	static void bAssign(menuVar* v1, menuVar* v2);
	static void fAssign(menuVar* v1, menuVar* v2);
	static void iAssign(menuVar* v1, menuVar* v2);
	static void sAssign(menuVar* v1, menuVar* v2);
	static void vAssign(menuVar* v1, menuVar* v2);

	static void nAdd(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);
	static void bAdd(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);
	static void fAdd(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);
	static void iAdd(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);
	static void sAdd(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);
	static void vAdd(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);

	static void nSub(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);
	static void bSub(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);
	static void fSub(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);
	static void iSub(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);
	static void sSub(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);
	static void vSub(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);

	static void nMul(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);
	static void bMul(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);
	static void fMul(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);
	static void iMul(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);
	static void sMul(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);
	static void vMul(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);

	static void nDiv(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);
	static void bDiv(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);
	static void fDiv(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);
	static void iDiv(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);
	static void sDiv(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);
	static void vDiv(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);

	static void nCond(menuItem* p, menuVar* v1, menuVar* v2, menuVar* v3, menuVar*& r);
	static void bCond(menuItem* p, menuVar* v1, menuVar* v2, menuVar* v3, menuVar*& r);
	static void fCond(menuItem* p, menuVar* v1, menuVar* v2, menuVar* v3, menuVar*& r);
	static void iCond(menuItem* p, menuVar* v1, menuVar* v2, menuVar* v3, menuVar*& r);
	static void sCond(menuItem* p, menuVar* v1, menuVar* v2, menuVar* v3, menuVar*& r);
	static void vCond(menuItem* p, menuVar* v1, menuVar* v2, menuVar* v3, menuVar*& r);

	static void nEqual(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);
	static void bEqual(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);
	static void fEqual(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);
	static void iEqual(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);
	static void sEqual(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);
	static void vEqual(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);

	static void nNotEqual(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);
	static void bNotEqual(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);
	static void fNotEqual(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);
	static void iNotEqual(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);
	static void sNotEqual(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);
	static void vNotEqual(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);

	static void nLess(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);
	static void bLess(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);
	static void fLess(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);
	static void iLess(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);
	static void sLess(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);
	static void vLess(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);

	static void nGreater(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);
	static void bGreater(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);
	static void fGreater(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);
	static void iGreater(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);
	static void sGreater(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);
	static void vGreater(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);

	static void nLessEqual(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);
	static void bLessEqual(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);
	static void fLessEqual(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);
	static void iLessEqual(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);
	static void sLessEqual(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);
	static void vLessEqual(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);

	static void nGreaterEqual(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);
	static void bGreaterEqual(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);
	static void fGreaterEqual(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);
	static void iGreaterEqual(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);
	static void sGreaterEqual(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);
	static void vGreaterEqual(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);

	static void nAnd(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);
	static void bAnd(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);

	static void nOr(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);
	static void bOr(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);

	static void nNot(menuItem* p, menuVar* v, menuVar*& r);
	static void bNot(menuItem* p, menuVar* v, menuVar*& r);

	static void nXor(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);
	static void bXor(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r);

	static void nSin(menuItem* p, menuVar* v, menuVar*& r);
	static void fSin(menuItem* p, menuVar* v, menuVar*& r);

	static void nCos(menuItem* p, menuVar* v, menuVar*& r);
	static void fCos(menuItem* p, menuVar* v, menuVar*& r);

	static void nTan(menuItem* p, menuVar* v, menuVar*& r);
	static void fTan(menuItem* p, menuVar* v, menuVar*& r);

	static void nAtan(menuItem* p, menuVar* v, menuVar*& r);
	static void fAtan(menuItem* p, menuVar* v, menuVar*& r);

	static void nAsin(menuItem* p, menuVar* v, menuVar*& r);
	static void fAsin(menuItem* p, menuVar* v, menuVar*& r);

	static void nAcos(menuItem* p, menuVar* v, menuVar*& r);
	static void fAcos(menuItem* p, menuVar* v, menuVar*& r);

	static void nSqrt(menuItem* p, menuVar* v, menuVar*& r);
	static void fSqrt(menuItem* p, menuVar* v, menuVar*& r);

	static void nExp(menuItem* p, menuVar* v, menuVar*& r);
	static void fExp(menuItem* p, menuVar* v, menuVar*& r);

	static void nLog(menuItem* p, menuVar* v, menuVar*& r);
	static void fLog(menuItem* p, menuVar* v, menuVar*& r);

	static void nLog10(menuItem* p, menuVar* v, menuVar*& r);
	static void fLog10(menuItem* p, menuVar* v, menuVar*& r);

	static void nInvSqrt(menuItem* p, menuVar* v, menuVar*& r);
	static void fInvSqrt(menuItem* p, menuVar* v, menuVar*& r);

	static void nSqr(menuItem* p, menuVar* v, menuVar*& r);
	static void fSqr(menuItem* p, menuVar* v, menuVar*& r);
public:
	menuVar(const wchar_t* variableName, char variableType);
	~menuVar();

	wchar_t* getName() { return name; }
	char getType() { return type; }

	bool getBool() { return gBool(vPtr); }
	float getFloat() { return gFloat(vPtr); }
	int getInt() { return gInt(vPtr); }
	wchar_t* getString() { return gString(vPtr, sVal); }

	void setBool(bool b) { sBool(vPtr, b); }
	void setFloat(float f) { sFloat(vPtr, f); }
	void setInt(int i) { sInt(vPtr, i); }
	void setString(const wchar_t* s) { sString(vPtr, s); }

	void assignment(menuVar* v) { ass(this, v); }
	void addition(menuItem* p, menuVar* v, menuVar*& r) { add(p, this, v, r); }
	void subtraction(menuItem* p, menuVar* v, menuVar*& r) { sub(p, this, v, r); }
	void multiplication(menuItem* p, menuVar* v, menuVar*& r) { mul(p, this, v, r); }
	void division(menuItem* p, menuVar* v, menuVar*& r) { div(p, this, v, r); }

	void conditional(menuItem* p, menuVar* v1, menuVar* v3, menuVar*& r) { cond(p, v1, this, v3, r); }
	void equalto(menuItem* p, menuVar* v, menuVar*& r) { equal(p, this, v, r); }
	void notequalto(menuItem* p, menuVar* v, menuVar*& r) { notequal(p, this, v, r); }
	void lessthan(menuItem* p, menuVar* v, menuVar*& r) { less(p, this, v, r); }
	void greaterthan(menuItem* p, menuVar* v, menuVar*& r) { greater(p, this, v, r); }
	void lessthanorequalto(menuItem* p, menuVar* v, menuVar*& r) { lequal(p, this, v, r); }
	void greaterthanorequalto(menuItem* p, menuVar* v, menuVar*& r) { gequal(p, this, v, r); }
	void booleanand(menuItem* p, menuVar* v, menuVar*& r) { and(p, this, v, r); }
	void booleanor(menuItem* p, menuVar* v, menuVar*& r) { or(p, this, v, r); }
	void booleannot(menuItem* p, menuVar*& r) { not(p, this, r); }
	void booleanxor(menuItem* p, menuVar* v, menuVar*& r) { xor(p, this, v, r); }
	
	void sine(menuItem* p, menuVar*& r) { sin(p, this, r); }
	void cosine(menuItem* p, menuVar*& r) { cos(p, this, r); }
	void tangent(menuItem* p, menuVar*& r) { tan(p, this, r); }
	void arctangent(menuItem* p, menuVar*& r) { atan(p, this, r); }
	void arcsine(menuItem* p, menuVar*& r) { asin(p, this, r); }
	void arccosine(menuItem* p, menuVar*& r) { acos(p, this, r); }

	void squareroot(menuItem* p, menuVar*& r) { sqrt(p, this, r); }
	void exponential(menuItem* p, menuVar*& r) { exp(p, this, r); }
	void logarithm(menuItem* p, menuVar*& r) { log(p, this, r); }
	void logarithm10(menuItem* p, menuVar*& r) { log10(p, this, r); }
	void inversesquareroot(menuItem* p, menuVar*& r) { invsqrt(p, this, r); }
	void square(menuItem* p, menuVar*& r) { sqr(p, this, r); }

	bool isVariable(const wchar_t* strName);

	wchar_t* getLanguageString();

	static void strInitialise(strSystem* s);

	static void comAssign(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam);
	static void comAdd(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam);
	static void comSub(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam);
	static void comMul(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam);
	static void comDiv(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam);

	static void comCond(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam);
	static void comEqual(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam);
	static void comNotEqual(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam);
	static void comLess(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam);
	static void comGreater(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam);
	static void comLessEqual(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam);
	static void comGreaterEqual(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam);
	static void comAnd(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam);
	static void comOr(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam);
	static void comNot(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam);
	static void comXor(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam);

	static void comSin(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam);
	static void comCos(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam);
	static void comTan(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam);
	static void comAsin(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam);
	static void comAcos(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam);
	static void comAtan(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam);

	static void comSqrt(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam);
	static void comExp(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam);
	static void comLog(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam);
	static void comLog10(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam);
	static void comInvSqrt(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam);
	static void comSqr(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam);
};

#endif