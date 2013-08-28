/* menuCom class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#include "menuCom.h"

#include "menuItem.h"
#include "menuScriptRoutine.h"
#include "menuSystem.h"
#include "menuVar.h"

#include "log.h"
#include "strSystem.h"

#include <string.h>

void menuSystem::initComs()
{
	addCom(L"=",				L"assignment",											menuVar::comAssign,				"vv",	0, true);
	addCom(L"+",				L"addition",											menuVar::comAdd,				"vv",	4, true);
	addCom(L"-",				L"subtraction",											menuVar::comSub,				"vv",	4, true);
	addCom(L"*",				L"multiplication",										menuVar::comMul,				"vv",	5, true);
	addCom(L"/",				L"division",											menuVar::comDiv,				"vv",	6, true);
	addCom(L"?",				L"conditional",											menuVar::comCond,				"vvv",	3, true);
	addCom(L"==",				L"equal to",											menuVar::comEqual,				"vv",	2, true);
	addCom(L"!=",				L"not equal to",										menuVar::comNotEqual,			"vv",	2, true);
	addCom(L"<",				L"less than",											menuVar::comLess,				"vv",	2, true);
	addCom(L">",				L"greater than",										menuVar::comGreater,			"vv",	2, true);
	addCom(L"<=",				L"less than or equal to",								menuVar::comLessEqual,			"vv",	2, true);
	addCom(L">=",				L"greater than or equal to",							menuVar::comGreaterEqual,		"vv",	2, true);
	addCom(L"&&",				L"logical and",											menuVar::comAnd,				"vv",	7, true);
	addCom(L"||",				L"logical or",											menuVar::comOr,					"vv",	7, true);
	addCom(L"!",				L"logical not",											menuVar::comNot,				"vv",	7, true);
	addCom(L"^^",				L"logical xor",											menuVar::comXor,				"vv",	7, true);
	addCom(L"atan",				L"arc (inverse) tangent",								menuVar::comAtan,				"f");
	addCom(L"asin",				L"arc (inverse) sine",									menuVar::comAsin,				"f");
	addCom(L"acos",				L"arc (inverse) cosine",								menuVar::comAcos,				"f");
	addCom(L"backcolor",		L"sets the background color of the current menu item",	menuItem::comBackColor,			"ffff");
	addCom(L"borderstyle",		L"sets the border style of the current menu item",		menuItem::comBorderStyle,		"i");
	addCom(L"backgroundstyle",	L"sets the background style of the current menu item",	menuItem::comBackgroundStyle,	"i");
	addCom(L"console",			L"runs console commands",								menuCom::comConsole,			"s");
	addCom(L"cos",				L"cosine",												menuVar::comCos,				"f");
	addCom(L"exp",				L"exponential function",								menuVar::comExp,				"f");
	addCom(L"forecolor",		L"sets the foreground color of the current menu item",	menuItem::comForeColor,			"ffff");
	addCom(L"invsqrt",			L"inverse square root",									menuVar::comInvSqrt,			"f");
	addCom(L"ln",				L"logarithm",											menuVar::comLog,				"f");
	addCom(L"log",				L"logarithm",											menuVar::comLog,				"f");
	addCom(L"log10",			L"logarithm base 10",									menuVar::comLog10,				"f");
	addCom(L"nop",				L"null operation (does nothing)",						menuCom::comNop);
	addCom(L"position",			L"sets the position of the current menu item",			menuItem::comPosition,			"ff");
	addCom(L"size",				L"sets the size of the current menu item",				menuItem::comSize,				"ff");
	addCom(L"sin",				L"sine",												menuVar::comSin,				"f");
	addCom(L"sqr",				L"square",												menuVar::comSqr,				"f");
	addCom(L"sqrt",				L"square root",											menuVar::comSqrt,				"f");
	addCom(L"tan",				L"tangent",												menuVar::comTan,				"f");
	addCom(L"text",				L"sets the text of the current menu item",				menuItem::comText,				"s");
	addCom(L"@pusht",			L"<inbuilt>",											menuCom::comPushT);
	addCom(L"@popt",			L"<inbuilt>",											menuCom::comPopT);
	addCom(L"@pushv",			L"<inbuilt>",											menuCom::comPushV);
	addCom(L"@popv",			L"<inbuilt>",											menuCom::comPopV);
	addCom(L"@stclear",			L"<inbuilt>",											menuCom::comStackClear);

#ifdef _DEBUG
	LogWriteLine(L"\x25CB\x25CB\x25CF Registered menu script functions");
#endif
}