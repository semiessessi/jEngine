/* menuVar class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#include "menuItem.h"
#include "menuVar.h"

#include "jMath.h"
#include "strSystem.h"

#include <malloc.h>
#include <wchar.h>

strSystem* menuVar::str = 0;

menuVar::menuVar(const wchar_t* variableName, char variableType)
{
	// constructor code for menuVar
	name = new wchar_t[wcslen(variableName)+1];
	name = wcscpy(name, variableName);

	type = variableType;

	bVal = false;
	fVal = 0.0f;
	iVal = 0;
	sVal = 0;

	gBool = nGetBool;
	gFloat = nGetFloat;
	gInt = nGetInt;
	gString = nGetString;

	sBool = nSetBool;
	sFloat = nSetFloat;
	sInt = nSetInt;
	sString = nSetString;

	ass = nAssign;
	add = nAdd;
	sub = nSub;
	mul = nAdd;
	div = nSub;

	cond = nCond;
	equal = nEqual;
	notequal = nNotEqual;
	less = nLess;
	greater = nGreater;
	lequal = nLessEqual;
	gequal = nGreaterEqual;
	and = nAnd;
	or = nOr;
	not = nNot;
	xor = nXor;

	sin = nSin;
	cos = nCos;
	tan = nTan;
	atan = nAtan;
	asin = nAsin;
	acos = nAcos;

	sqrt = nSqrt;
	exp = nExp;
	log = nLog;
	log10 = nLog10;
	invsqrt = nInvSqrt;
	sqr = nSqr;

	switch(type)
	{
	case L'b':
		gBool = bGetBool;
		gFloat = bGetFloat;
		gInt = bGetInt;
		gString = bGetString;

		sBool = bSetBool;
		sFloat = bSetFloat;
		sInt = bSetInt;
		sString = bSetString;

		ass = bAssign;
		add = bAdd;
		sub = bSub;
		mul = bMul;
		div = bDiv;

		cond		= bCond;
		equal		= bEqual;
		notequal	= bNotEqual;
		less		= bLess;
		greater		= bGreater;
		lequal		= bLessEqual;
		gequal		= bGreaterEqual;
		and = bAnd;
		or = bOr;
		not = bNot;
		xor = bXor;

		sin = fSin;
		cos = fCos;
		tan = fTan;
		atan = fAtan;
		asin = fAsin;
		acos = fAcos;

		sqrt = fSqrt;
		exp = fExp;
		log = fLog;
		log10 = fLog10;
		invsqrt = fInvSqrt;
		sqr = fSqr;

		vPtr = &bVal;
		break;
	case L'f':
		gBool = fGetBool;
		gFloat = fGetFloat;
		gInt = fGetInt;
		gString = fGetString;

		sBool = fSetBool;
		sFloat = fSetFloat;
		sInt = fSetInt;
		sString = fSetString;

		ass = fAssign;
		add = fAdd;
		sub = fSub;
		mul = fMul;
		div = fDiv;

		cond		= fCond;
		equal		= fEqual;
		notequal	= fNotEqual;
		less		= fLess;
		greater		= fGreater;
		lequal		= fLessEqual;
		gequal		= fGreaterEqual;
		and = bAnd;
		or = bOr;
		not = bNot;
		xor = bXor;

		sin = fSin;
		cos = fCos;
		tan = fTan;
		atan = fAtan;
		asin = fAsin;
		acos = fAcos;

		sqrt = fSqrt;
		exp = fExp;
		log = fLog;
		log10 = fLog10;
		invsqrt = fInvSqrt;
		sqr = fSqr;

		vPtr = &fVal;
		break;
	case L'i':
		gBool = iGetBool;
		gFloat = iGetFloat;
		gInt = iGetInt;
		gString = iGetString;

		sBool = iSetBool;
		sFloat = iSetFloat;
		sInt = iSetInt;
		sString = iSetString;

		ass = iAssign;
		add = iAdd;
		sub = iSub;
		mul = iMul;
		div = iDiv;

		cond		= iCond;
		equal		= iEqual;
		notequal	= iNotEqual;
		less		= iLess;
		greater		= iGreater;
		lequal		= iLessEqual;
		gequal		= iGreaterEqual;
		and = bAnd;
		or = bOr;
		not = bNot;
		xor = bXor;

		sin = fSin;
		cos = fCos;
		tan = fTan;
		atan = fAtan;
		asin = fAsin;
		acos = fAcos;

		sqrt = fSqrt;
		exp = fExp;
		log = fLog;
		log10 = fLog10;
		invsqrt = fInvSqrt;
		sqr = fSqr;

		vPtr = &iVal;
		break;
	case L's':
		gBool = sGetBool;
		gFloat = sGetFloat;
		gInt = sGetInt;
		gString = sGetString;

		sBool = sSetBool;
		sFloat = sSetFloat;
		sInt = sSetInt;
		sString = sSetString;

		ass = sAssign;
		add = sAdd;
		sub = sSub;
		mul = sMul;
		div = sDiv;

		cond		= sCond;
		equal		= sEqual;
		notequal	= sNotEqual;
		less		= sLess;
		greater		= sGreater;
		lequal		= sLessEqual;
		gequal		= sGreaterEqual;
		and = bAnd;
		or = bOr;
		not = bNot;
		xor = bXor;

		sin = fSin;
		cos = fCos;
		tan = fTan;
		atan = fAtan;
		asin = fAsin;
		acos = fAcos;

		sqrt = fSqrt;
		exp = fExp;
		log = fLog;
		log10 = fLog10;
		invsqrt = fInvSqrt;
		sqr = fSqr;

		vPtr = &sVal;
		break;
	default:
		type = L'v';
	case L'v':
		gBool = vGetBool;
		gFloat = vGetFloat;
		gInt = vGetInt;
		gString = vGetString;

		sBool = vSetBool;
		sFloat = vSetFloat;
		sInt = vSetInt;
		sString = vSetString;

		ass = vAssign;
		add = vAdd;
		sub = vSub;
		mul = vMul;
		div = vDiv;

		cond		= vCond;
		equal		= vEqual;
		notequal	= vNotEqual;
		less		= vLess;
		greater		= vGreater;
		lequal		= vLessEqual;
		gequal		= vGreaterEqual;
		and = bAnd;
		or = bOr;
		not = bNot;
		xor = bXor;

		sin = fSin;
		cos = fCos;
		tan = fTan;
		atan = fAtan;
		asin = fAsin;
		acos = fAcos;

		sqrt = fSqrt;
		exp = fExp;
		log = fLog;
		log10 = fLog10;
		invsqrt = fInvSqrt;
		sqr = fSqr;

		vPtr = &sVal;
		break;
	};
}

menuVar::~menuVar()
{
	// destructor code for menuVar
	if(name) delete[] name;

	if(sVal) free(sVal);
}

bool menuVar::bGetBool(void* p)
{
	return (*(bool*)p);
}

bool menuVar::fGetBool(void* p)
{
	return ((*(float*)p)!=0.0f);
}

bool menuVar::iGetBool(void* p)
{
	return ((*(int*)p)!=0);
}

bool menuVar::sGetBool(void* p)
{
	bool ret = (_wcsicmp(L"True", *(wchar_t**)p) == 0);
	return ret;
}

bool menuVar::vGetBool(void* p)
{
	bool ret = (_wcsicmp(L"True", *(wchar_t**)p) == 0);
	return ret;
}

float menuVar::bGetFloat(void* p)
{
	return (*(bool*)p) ? 1.0f : 0.0f;
}

float menuVar::fGetFloat(void* p)
{
	return (*(float*)p);
}

float menuVar::iGetFloat(void* p)
{
	return (float)(*(int*)p);
}

float menuVar::sGetFloat(void* p)
{
	return (float)_wtof(*(wchar_t**)p);
}

float menuVar::vGetFloat(void* p)
{
	return (float)_wtof(*(wchar_t**)p);
}

int menuVar::bGetInt(void* p)
{
	return (*(bool*)p) ? 1 : 0;
}

int menuVar::fGetInt(void* p)
{
	return (int)(*(float*)p);
}

int menuVar::iGetInt(void* p)
{
	return (*(int*)p);
}

int menuVar::sGetInt(void* p)
{
	return (int)_wtoi(*(wchar_t**)p);
}

int menuVar::vGetInt(void* p)
{
	return (int)_wtoi(*(wchar_t**)p);
}

wchar_t* menuVar::bGetString(void* p, wchar_t*& sv)
{
	if(sv) free(sv);
	int len = ((*(bool*)p) ? _scwprintf(L"True") : _scwprintf(L"False")) + 1;
	sv = (wchar_t*)malloc(len*sizeof(wchar_t));
	(*(bool*)p) ? swprintf(sv, L"True") :  swprintf(sv, L"False");
	
	return sv;
}

wchar_t* menuVar::fGetString(void* p, wchar_t*& sv)
{
	int len = _scwprintf(L"%f", *(float*)p)+1;
	if(sv) free(sv);
	sv = (wchar_t*)malloc(len*sizeof(wchar_t));
	swprintf(sv, L"%f", *(float*)p); 
	
	return sv;
}

wchar_t* menuVar::iGetString(void* p, wchar_t*& sv)
{
	int len = _scwprintf(L"%i", *(int*)p)+1;
	if(sv) free(sv);
	sv = (wchar_t*)malloc(len*sizeof(wchar_t));
	swprintf(sv, L"%i", *(int*)p); 
	
	return sv;
}

wchar_t* menuVar::sGetString(void* p, wchar_t*& sv)
{
	return sv;
}

wchar_t* menuVar::vGetString(void* p, wchar_t*& sv)
{
	return sv;
}

void menuVar::bSetBool(void* p, bool b)
{
	(*(bool*)p) = b;
	return;
}

void menuVar::fSetBool(void* p, bool b)
{
	(*(float*)p) = b ? 1.0f : 0.0f;
	return;
}

void menuVar::iSetBool(void* p, bool b)
{
	(*(int*)p) = b ? 1 : 0;
	return;
}

void menuVar::sSetBool(void* p, bool b)
{
	if(*(wchar_t**)p) free(*(wchar_t**)p);
	int len = (b ? _scwprintf(L"True") : _scwprintf(L"False")) + 1;
	*(wchar_t**)p = (wchar_t*)malloc(len*sizeof(wchar_t));
	b ? swprintf(*(wchar_t**)p, L"True") :  swprintf(*(wchar_t**)p, L"False");
	return;
}

void menuVar::vSetBool(void* p, bool b)
{
	if(*(wchar_t**)p) free(*(wchar_t**)p);
	int len = (b ? _scwprintf(L"True") : _scwprintf(L"False")) + 1;
	*(wchar_t**)p = (wchar_t*)malloc(len*sizeof(wchar_t));
	b ? swprintf(*(wchar_t**)p, L"True") :  swprintf(*(wchar_t**)p, L"False");
	return;
}

void menuVar::bSetFloat(void* p, float f)
{
	(*(bool*)p) = (f!=0.0f);
	return;
}

void menuVar::fSetFloat(void* p, float f)
{
	(*(float*)p) = f;
	return;
}

void menuVar::iSetFloat(void* p, float f)
{
	(*(int*)p) = (int)f;
	return;
}

void menuVar::sSetFloat(void* p, float f)
{
	if(*(wchar_t**)p) free(*(wchar_t**)p);
	int len =  _scwprintf(L"%f", f)+1;
	(*(wchar_t**)p) = (wchar_t*)malloc(sizeof(wchar_t)*len);
	swprintf(*(wchar_t**)p, L"%f", f); 

	return;
}

void menuVar::vSetFloat(void* p, float f)
{
	if(*(wchar_t**)p) free(*(wchar_t**)p);
	int len =  _scwprintf(L"%f", f)+1;
	(*(wchar_t**)p) = (wchar_t*)malloc(sizeof(wchar_t)*len);
	swprintf(*(wchar_t**)p, L"%f", f); 

	return;
}

void menuVar::bSetInt(void* p, int i)
{
	(*(bool*)p) = (i!=0);
	return;
}

void menuVar::fSetInt(void* p, int i)
{
	(*(float*)p) = (float)i;
	return;
}

void menuVar::iSetInt(void* p, int i)
{
	(*(int*)p) = (int)i;
	return;
}

void menuVar::sSetInt(void* p, int i)
{
	if(*(wchar_t**)p) free(*(wchar_t**)p);
	int len =  _scwprintf(L"%i", i)+1;
	(*(wchar_t**)p) = (wchar_t*)malloc(sizeof(wchar_t)*len);
	swprintf(*(wchar_t**)p, L"%i", i); 

	return;
}

void menuVar::vSetInt(void* p, int i)
{
	if(*(wchar_t**)p) free(*(wchar_t**)p);
	int len =  _scwprintf(L"%i", i)+1;
	(*(wchar_t**)p) = (wchar_t*)malloc(sizeof(wchar_t)*len);
	swprintf(*(wchar_t**)p, L"%i", i); 

	return;
}

void menuVar::bSetString(void* p, const wchar_t* sv)
{
	wchar_t* c = str->getString(L"True");
	(*(bool*)p) = (_wcsicmp(c, *(wchar_t**)sv) == 0);
	free(c);
	return;
}

void menuVar::fSetString(void* p, const wchar_t* sv)
{
	(*(float*)p) = (float)_wtof(sv);
	return;
}

void menuVar::iSetString(void* p, const wchar_t* sv)
{
	(*(int*)p) = (int)_wtoi(sv);
	return;
}

void menuVar::sSetString(void* p, const wchar_t* sv)
{
	if(*(wchar_t**)p) free(*(wchar_t**)p);
	(*(wchar_t**)p) = (wchar_t*)malloc(sizeof(wchar_t)*(wcslen(sv)+1));
	*(wchar_t**)p = wcscpy(*(wchar_t**)p, sv);
	return;
}

void menuVar::vSetString(void* p, const wchar_t* sv)
{
	if(*(wchar_t**)p) free(*(wchar_t**)p);
	(*(wchar_t**)p) = (wchar_t*)malloc(sizeof(wchar_t)*(wcslen(sv)+1));
	*(wchar_t**)p = wcscpy(*(wchar_t**)p, sv);
	return;
}

void menuVar::bAssign(menuVar* v1, menuVar* v2)
{
	v1->setBool(v2->getBool());
}

void menuVar::fAssign(menuVar* v1, menuVar* v2)
{
	v1->setFloat(v2->getFloat());
}

void menuVar::iAssign(menuVar* v1, menuVar* v2)
{
	v1->setInt(v2->getInt());
}

void menuVar::sAssign(menuVar* v1, menuVar* v2)
{
	v1->setString(v2->getString());
}

void menuVar::vAssign(menuVar* v1, menuVar* v2)
{
	v1->setString(v2->getString());
}

void menuVar::bAdd(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	r = new menuVar(L"@", 'b');
	r->setBool(((int)(v1->getBool()) + (int)(v2->getBool())) == 0);
	return;
}

void menuVar::fAdd(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	r = new menuVar(L"@", 'f');
	r->setFloat(v1->getFloat() + v2->getFloat());
	return;
}

void menuVar::iAdd(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	r = new menuVar(L"@", 'i');
	r->setInt(v1->getInt() + v2->getInt());
	return;
}

void menuVar::sAdd(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	r = new menuVar(L"@", 's');
	int len = (int)wcslen(v1->getString())+(int)wcslen(v2->getString())+1;
	wchar_t* tmp = new wchar_t[len];
	swprintf(tmp, L"%s%s", v1->getString(), v2->getString());
	r->setString(tmp);
	delete[] tmp;
	return;
}

void menuVar::vAdd(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	r = new menuVar(L"@", 'v');
	r->setFloat(v1->getFloat() + v2->getFloat());
	return;
}

void menuVar::bSub(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	r = new menuVar(L"@", 'b');
	r->setBool(((int)(v1->getBool()) - (int)(v2->getBool())) == 0);
	return;
}

void menuVar::fSub(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	r = new menuVar(L"@", 'f');
	r->setFloat(v1->getFloat() - v2->getFloat());
	return;
}

void menuVar::iSub(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	r = new menuVar(L"@", 'i');
	r->setInt(v1->getInt() - v2->getInt());
	return;
}

void menuVar::sSub(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	r = new menuVar(L"@", 's');
	// TODO:: string subtraction
	r->setString(L"");
	return;
}

void menuVar::vSub(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	r = new menuVar(L"@", 'v');
	r->setFloat(v1->getFloat() - v2->getFloat());
	return;
}

void menuVar::bMul(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	r = new menuVar(L"@", 'b');
	r->setBool(((int)(v1->getBool()) * (int)(v2->getBool())) == 0);
	return;
}

void menuVar::fMul(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	r = new menuVar(L"@", 'f');
	r->setFloat(v1->getFloat() * v2->getFloat());
	return;
}

void menuVar::iMul(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	r = new menuVar(L"@", 'i');
	r->setInt(v1->getInt() * v2->getInt());
	return;
}

void menuVar::sMul(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	r = new menuVar(L"@", 's');
	// TODO:: string multiplication
	r->setString(L"");
	return;
}

void menuVar::vMul(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	r = new menuVar(L"@", 'v');
	r->setFloat(v1->getFloat() * v2->getFloat());
	return;
}

void menuVar::bDiv(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	r = new menuVar(L"@", 'b');
	r->setBool(((int)(v1->getBool()) / (int)(v2->getBool())) == 0);
	return;
}

void menuVar::fDiv(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	r = new menuVar(L"@", 'f');
	r->setFloat(v1->getFloat() / v2->getFloat());
	return;
}

void menuVar::iDiv(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	r = new menuVar(L"@", 'i');
	r->setInt(v1->getInt() / v2->getInt());
	return;
}

void menuVar::sDiv(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	r = new menuVar(L"@", 's');
	// TODO:: string division
	r->setString(L"");
	return;
}

void menuVar::vDiv(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	r = new menuVar(L"@", 'v');
	r->setFloat(v1->getFloat() / v2->getFloat());
	return;
}

void menuVar::bCond(menuItem* p, menuVar* v1, menuVar* v2, menuVar* v3, menuVar*& r)
{
	r = new menuVar(L"@", 'b');
	r->setBool( v1->getBool() ? v2->getBool() : v3->getBool() );	
	return;
}

void menuVar::fCond(menuItem* p, menuVar* v1, menuVar* v2, menuVar* v3, menuVar*& r)
{
	r = new menuVar(L"@", 'f');
	r->setFloat( v1->getBool() ? v2->getFloat() : v3->getFloat() );	
	return;
}

void menuVar::iCond(menuItem* p, menuVar* v1, menuVar* v2, menuVar* v3, menuVar*& r)
{
	r = new menuVar(L"@", 'i');
	r->setInt( v1->getBool() ? v2->getInt() : v3->getInt() );	
	return;
}

void menuVar::sCond(menuItem* p, menuVar* v1, menuVar* v2, menuVar* v3, menuVar*& r)
{
	r = new menuVar(L"@", 's');
	r->setString( v1->getBool() ? v2->getString() : v3->getString() );	
	return;
}

void menuVar::vCond(menuItem* p, menuVar* v1, menuVar* v2, menuVar* v3, menuVar*& r)
{
	r = new menuVar(L"@", 'v');
	r->setString( v1->getBool() ? v2->getString() : v3->getString() );	
	return;
}

void menuVar::bEqual(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	r = new menuVar(L"@", 'b');
	r->setBool(v1->getBool() == v2->getBool());
	return;
}

void menuVar::fEqual(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	r = new menuVar(L"@", 'b');
	r->setBool(v1->getFloat() == v2->getFloat());
	return;
}

void menuVar::iEqual(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	r = new menuVar(L"@", 'b');
	r->setBool(v1->getInt() == v2->getInt());
	return;
}

void menuVar::sEqual(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	r = new menuVar(L"@", 'b');
	r->setBool(!wcscmp(v1->getString(), v2->getString()));
	return;
}

void menuVar::vEqual(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	r = new menuVar(L"@", 'b');
	r->setBool(!wcscmp(v1->getString(), v2->getString()));
	return;
}

void menuVar::bNotEqual(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	r = new menuVar(L"@", 'b');
	r->setBool(v1->getBool() != v2->getBool());
	return;
}

void menuVar::fNotEqual(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	r = new menuVar(L"@", 'b');
	r->setBool(v1->getFloat() != v2->getFloat());
	return;
}

void menuVar::iNotEqual(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	r = new menuVar(L"@", 'b');
	r->setBool(v1->getInt() != v2->getInt());
	return;
}

void menuVar::sNotEqual(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	r = new menuVar(L"@", 'b');
	r->setBool(wcscmp(v1->getString(), v2->getString())!=0);
	return;
}

void menuVar::vNotEqual(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	r = new menuVar(L"@", 'b');
	r->setBool(wcscmp(v1->getString(), v2->getString())!=0);
	return;
}

void menuVar::bLess(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	r = new menuVar(L"@", 'b');
	r->setBool(v1->getBool() < v2->getBool());
	return;
}

void menuVar::fLess(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	r = new menuVar(L"@", 'b');
	r->setBool(v1->getFloat() < v2->getFloat());
	return;
}

void menuVar::iLess(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	r = new menuVar(L"@", 'b');
	r->setBool(v1->getInt() < v2->getInt());
	return;
}

void menuVar::sLess(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	r = new menuVar(L"@", 'b');
	r->setBool(wcslen(v1->getString()) < wcslen(v2->getString()));
	return;
}

void menuVar::vLess(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	r = new menuVar(L"@", 'b');
	r->setBool(wcslen(v1->getString()) < wcslen(v2->getString()));
	return;
}

void menuVar::bGreater(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	r = new menuVar(L"@", 'b');
	r->setBool(v1->getBool() > v2->getBool());
	return;
}

void menuVar::fGreater(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	r = new menuVar(L"@", 'b');
	r->setBool(v1->getFloat() > v2->getFloat());
	return;
}

void menuVar::iGreater(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	r = new menuVar(L"@", 'b');
	r->setBool(v1->getInt() > v2->getInt());
	return;
}

void menuVar::sGreater(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	r = new menuVar(L"@", 'b');
	r->setBool(wcslen(v1->getString()) > wcslen(v2->getString()));
	return;
}

void menuVar::vGreater(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	r = new menuVar(L"@", 'b');
	r->setBool(wcslen(v1->getString()) > wcslen(v2->getString()));
	return;
}

void menuVar::bLessEqual(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	r = new menuVar(L"@", 'b');
	r->setBool(v1->getBool() <= v2->getBool());
	return;
}

void menuVar::fLessEqual(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	r = new menuVar(L"@", 'b');
	r->setBool(v1->getFloat() <= v2->getFloat());
	return;
}

void menuVar::iLessEqual(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	r = new menuVar(L"@", 'b');
	r->setBool(v1->getInt() <= v2->getInt());
	return;
}

void menuVar::sLessEqual(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	r = new menuVar(L"@", 'b');
	r->setBool(wcslen(v1->getString()) <= wcslen(v2->getString()));
	return;
}

void menuVar::vLessEqual(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	r = new menuVar(L"@", 'b');
	r->setBool(wcslen(v1->getString()) <= wcslen(v2->getString()));
	return;
}

void menuVar::bGreaterEqual(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	r = new menuVar(L"@", 'b');
	r->setBool(v1->getBool() >= v2->getBool());
	return;
}

void menuVar::fGreaterEqual(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	r = new menuVar(L"@", 'b');
	r->setBool(v1->getFloat() >= v2->getFloat());
	return;
}

void menuVar::iGreaterEqual(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	r = new menuVar(L"@", 'b');
	r->setBool(v1->getInt() >= v2->getInt());
	return;
}

void menuVar::sGreaterEqual(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	r = new menuVar(L"@", 'b');
	r->setBool(wcslen(v1->getString()) >= wcslen(v2->getString()));
	return;
}

void menuVar::vGreaterEqual(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	r = new menuVar(L"@", 'b');
	r->setBool(wcslen(v1->getString()) >= wcslen(v2->getString()));
	return;
}

void menuVar::bAnd(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	r = new menuVar(L"@", 'b');
	r->setBool(v1->getBool() && v2->getBool());
	return;
}

void menuVar::bOr(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	r = new menuVar(L"@", 'b');
	r->setBool(v1->getBool() || v2->getBool());
	return;
}

void menuVar::bNot(menuItem* p, menuVar* v, menuVar*& r)
{
	r = new menuVar(L"@", 'b');
	r->setBool(!v->getBool());
	return;
}
void menuVar::bXor(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	r = new menuVar(L"@", 'b');
	r->setBool((v1->getBool() || v2->getBool()) && !((v1->getBool() && v2->getBool())));
	return;
}

void menuVar::fSin(menuItem* p, menuVar* v, menuVar*& r)
{
	r = new menuVar(L"@", 'f');
	r->setFloat(sinf(v->getFloat()));
	return;
}

void menuVar::fCos(menuItem* p, menuVar* v, menuVar*& r)
{
	r = new menuVar(L"@", 'f');
	r->setFloat(cosf(v->getFloat()));
	return;
}

void menuVar::fTan(menuItem* p, menuVar* v, menuVar*& r)
{
	r = new menuVar(L"@", 'f');
	r->setFloat(tanf(v->getFloat()));
	return;
}

void menuVar::fAtan(menuItem* p, menuVar* v, menuVar*& r)
{
	r = new menuVar(L"@", 'f');
	r->setFloat(atanf(v->getFloat()));
	return;
}

void menuVar::fAsin(menuItem* p, menuVar* v, menuVar*& r)
{
	r = new menuVar(L"@", 'f');
	r->setFloat(asinf(v->getFloat()));
	return;
}

void menuVar::fAcos(menuItem* p, menuVar* v, menuVar*& r)
{
	r = new menuVar(L"@", 'f');
	r->setFloat(acosf(v->getFloat()));
	return;
}

void menuVar::fSqrt(menuItem* p, menuVar* v, menuVar*& r)
{
	r = new menuVar(L"@", 'f');
	r->setFloat(sqrtf(v->getFloat()));
	return;
}

void menuVar::fExp(menuItem* p, menuVar* v, menuVar*& r)
{
	r = new menuVar(L"@", 'f');
	r->setFloat(expf(v->getFloat()));
	return;
}

void menuVar::fLog(menuItem* p, menuVar* v, menuVar*& r)
{
	r = new menuVar(L"@", 'f');
	r->setFloat(logf(v->getFloat()));
	return;
}

void menuVar::fLog10(menuItem* p, menuVar* v, menuVar*& r)
{
	r = new menuVar(L"@", 'f');
	r->setFloat(log10f(v->getFloat()));
	return;
}

void menuVar::fInvSqrt(menuItem* p, menuVar* v, menuVar*& r)
{
	r = new menuVar(L"@", 'f');
	r->setFloat(1/sqrtf(v->getFloat()));
	return;
}

void menuVar::fSqr(menuItem* p, menuVar* v, menuVar*& r)
{
	r = new menuVar(L"@", 'f');
	r->setFloat(v->getFloat() * v->getFloat());
	return;
}

bool menuVar::nGetBool(void* p)
{
	return false;
}

float menuVar::nGetFloat(void* p)
{
	return 0.0f;
}

int menuVar::nGetInt(void* p)
{
	return 0;
}

wchar_t* menuVar::nGetString(void* p, wchar_t*& sv)
{
	return sv;
}

void menuVar::nSetBool(void* p, bool b)
{
	return;
}

void menuVar::nSetFloat(void* p, float f)
{
	return;
}

void menuVar::nSetInt(void* p, int i)
{
	return;
}

void menuVar::nSetString(void* p, const wchar_t* sv)
{
	return;
}

void menuVar::nAssign(menuVar* v1, menuVar* v2)
{
	return;
}

void menuVar::nAdd(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	return;
}

void menuVar::nSub(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	return;
}

void menuVar::nMul(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	return;
}

void menuVar::nDiv(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	return;
}

void menuVar::nCond(menuItem* p, menuVar* v1, menuVar* v2,  menuVar* v3, menuVar*& r)
{
	return;
}

void menuVar::nEqual(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	return;
}

void menuVar::nNotEqual(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	return;
}

void menuVar::nLess(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	return;
}

void menuVar::nGreater(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	return;
}

void menuVar::nLessEqual(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	return;
}

void menuVar::nGreaterEqual(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	return;
}

void menuVar::nAnd(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	return;
}

void menuVar::nOr(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	return;
}

void menuVar::nNot(menuItem* p, menuVar* v, menuVar*& r)
{
	return;
}
void menuVar::nXor(menuItem* p, menuVar* v1, menuVar* v2, menuVar*& r)
{
	return;
}

void menuVar::nSin(menuItem* p, menuVar* v, menuVar*& r)
{
	return;
}

void menuVar::nCos(menuItem* p, menuVar* v, menuVar*& r)
{
	return;
}

void menuVar::nTan(menuItem* p, menuVar* v, menuVar*& r)
{
	return;
}

void menuVar::nAtan(menuItem* p, menuVar* v, menuVar*& r)
{
	return;
}
void menuVar::nAsin(menuItem* p, menuVar* v, menuVar*& r)
{
	return;
}

void menuVar::nAcos(menuItem* p, menuVar* v, menuVar*& r)
{
	return;
}

void menuVar::nSqrt(menuItem* p, menuVar* v, menuVar*& r)
{
	return;
}

void menuVar::nExp(menuItem* p, menuVar* v, menuVar*& r)
{
	return;
}

void menuVar::nLog10(menuItem* p, menuVar* v, menuVar*& r)
{
	return;
}

void menuVar::nLog(menuItem* p, menuVar* v, menuVar*& r)
{
	return;
}

void menuVar::nInvSqrt(menuItem* p, menuVar* v, menuVar*& r)
{
	return;
}

void menuVar::nSqr(menuItem* p, menuVar* v, menuVar*& r)
{
	return;
}

wchar_t* menuVar::getLanguageString()
{
	return str->getString(gString(vPtr, sVal));
}

void menuVar::strInitialise(strSystem* s)
{
	str = s;
}

bool menuVar::isVariable(const wchar_t* strName)
{
	if(_wcsicmp(strName, name)) return false;
	else return true;
}

void menuVar::comAssign(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam)
{
	menuVar* v2 = varStack->popFast();
	menuVar* v1 = varStack->popFast();
	v1->assignment(v2);
	varStack->pushFast(v1);
	return;
}

void menuVar::comAdd(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam)
{
	menuVar* v2 = varStack->popFast();
	menuVar* v1 = varStack->popFast();
	menuVar* r = 0;
	v1->addition(parent, v2, r);
	varStack->pushFast(r);
	tempStack->push(r);
	return;
}

void menuVar::comSub(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam)
{
	menuVar* v2 = varStack->popFast();
	menuVar* v1 = varStack->popFast();
	menuVar* r = 0;
	v1->subtraction(parent, v2, r);
	varStack->pushFast(r);
	tempStack->push(r);
	return;
}

void menuVar::comMul(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam)
{
	menuVar* v2 = varStack->popFast();
	menuVar* v1 = varStack->popFast();
	menuVar* r = 0;
	v1->multiplication(parent, v2, r);
	varStack->pushFast(r);
	tempStack->push(r);
	return;
}

void menuVar::comDiv(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam)
{
	menuVar* v2 = varStack->popFast();
	menuVar* v1 = varStack->popFast();
	menuVar* r = 0;
	v1->division(parent, v2, r);
	varStack->pushFast(r);
	tempStack->push(r);
	return;
}

void menuVar::comCond(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam)
{
	menuVar* v3 = varStack->popFast();
	menuVar* v2 = varStack->popFast();
	menuVar* v1 = varStack->popFast();
	menuVar* r = 0;
	v2->conditional(parent, v1, v3, r);
	varStack->pushFast(r);
	tempStack->push(r);
	return;
}

void menuVar::comEqual(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam)
{
	menuVar* v2 = varStack->popFast();
	menuVar* v1 = varStack->popFast();
	menuVar* r = 0;
	v1->equalto(parent, v2, r);
	varStack->pushFast(r);
	tempStack->push(r);
	return;
}

void menuVar::comNotEqual(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam)
{
	menuVar* v2 = varStack->popFast();
	menuVar* v1 = varStack->popFast();
	menuVar* r = 0;
	v1->notequalto(parent, v2, r);
	varStack->pushFast(r);
	tempStack->push(r);
	return;
}

void menuVar::comLess(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam)
{
	menuVar* v2 = varStack->popFast();
	menuVar* v1 = varStack->popFast();
	menuVar* r = 0;
	v1->lessthan(parent, v2, r);
	varStack->pushFast(r);
	tempStack->push(r);
	return;
}

void menuVar::comGreater(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam)
{
	menuVar* v2 = varStack->popFast();
	menuVar* v1 = varStack->popFast();
	menuVar* r = 0;
	v1->greaterthan(parent, v2, r);
	varStack->pushFast(r);
	tempStack->push(r);
	return;
}

void menuVar::comLessEqual(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam)
{
	menuVar* v2 = varStack->popFast();
	menuVar* v1 = varStack->popFast();
	menuVar* r = 0;
	v1->lessthanorequalto(parent, v2, r);
	varStack->pushFast(r);
	tempStack->push(r);
	return;
}

void menuVar::comGreaterEqual(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam)
{
	menuVar* v2 = varStack->popFast();
	menuVar* v1 = varStack->popFast();
	menuVar* r = 0;
	v1->greaterthanorequalto(parent, v2, r);
	varStack->pushFast(r);
	tempStack->push(r);
	return;
}

void menuVar::comAnd(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam)
{
	menuVar* v2 = varStack->popFast();
	menuVar* v1 = varStack->popFast();
	menuVar* r = 0;
	v1->booleanand(parent, v2, r);
	varStack->pushFast(r);
	tempStack->push(r);
	return;
}

void menuVar::comOr(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam)
{
	menuVar* v2 = varStack->popFast();
	menuVar* v1 = varStack->popFast();
	menuVar* r = 0;
	v1->booleanor(parent, v2, r);
	varStack->pushFast(r);
	tempStack->push(r);
	return;
}

void menuVar::comNot(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam)
{
	menuVar* v1 = varStack->popFast();
	menuVar* r = 0;
	v1->booleannot(parent, r);
	varStack->pushFast(r);
	tempStack->push(r);
	return;
}

void menuVar::comXor(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam)
{
	menuVar* v2 = varStack->popFast();
	menuVar* v1 = varStack->popFast();
	menuVar* r = 0;
	v1->booleanxor(parent, v2, r);
	varStack->pushFast(r);
	tempStack->push(r);
	return;
}

void menuVar::comSin(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam)
{
	menuVar* v1 = varStack->popFast();
	menuVar* r = 0;
	v1->sine(parent, r);
	varStack->pushFast(r);
	tempStack->push(r);
	return;
}

void menuVar::comCos(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam)
{
	menuVar* v1 = varStack->popFast();
	menuVar* r = 0;
	v1->cosine(parent, r);
	varStack->pushFast(r);
	tempStack->push(r);
	return;
}

void menuVar::comTan(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam)
{
	menuVar* v1 = varStack->popFast();
	menuVar* r = 0;
	v1->tangent(parent, r);
	varStack->pushFast(r);
	tempStack->push(r);
	return;
}

void menuVar::comAtan(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam)
{
	menuVar* v1 = varStack->popFast();
	menuVar* r = 0;
	v1->arctangent(parent, r);
	varStack->pushFast(r);
	tempStack->push(r);
	return;
}

void menuVar::comAsin(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam)
{
	menuVar* v1 = varStack->popFast();
	menuVar* r = 0;
	v1->arcsine(parent, r);
	varStack->pushFast(r);
	tempStack->push(r);
	return;
}

void menuVar::comAcos(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam)
{
	menuVar* v1 = varStack->popFast();
	menuVar* r = 0;
	v1->arccosine(parent, r);
	varStack->pushFast(r);
	tempStack->push(r);
	return;
}

void menuVar::comSqrt(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam)
{
	menuVar* v1 = varStack->popFast();
	menuVar* r = 0;
	v1->squareroot(parent, r);
	varStack->pushFast(r);
	tempStack->push(r);
	return;
}

void menuVar::comExp(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam)
{
	menuVar* v1 = varStack->popFast();
	menuVar* r = 0;
	v1->exponential(parent, r);
	varStack->pushFast(r);
	tempStack->push(r);
	return;
}

void menuVar::comLog(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam)
{
	menuVar* v1 = varStack->popFast();
	menuVar* r = 0;
	v1->logarithm(parent, r);
	varStack->pushFast(r);
	tempStack->push(r);
	return;
}

void menuVar::comLog10(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam)
{
	menuVar* v1 = varStack->popFast();
	menuVar* r = 0;
	v1->logarithm10(parent, r);
	varStack->pushFast(r);
	tempStack->push(r);
	return;
}

void menuVar::comInvSqrt(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam)
{
	menuVar* v1 = varStack->popFast();
	menuVar* r = 0;
	v1->inversesquareroot(parent, r);
	varStack->pushFast(r);
	tempStack->push(r);
	return;
}

void menuVar::comSqr(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam)
{
	menuVar* v1 = varStack->popFast();
	menuVar* r = 0;
	v1->square(parent, r);
	varStack->pushFast(r);
	tempStack->push(r);
	return;
}
