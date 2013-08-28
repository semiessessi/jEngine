/* menuHScroll class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#include "menuHScroll.h"

menuHScroll::menuHSLeft::menuHSLeft(menuSystem *ms) : menuButton(ms, 0)
{
}

menuHScroll::menuHSRight::menuHSRight(menuSystem *ms) : menuButton(ms, 0)
{
}

menuHScroll::menuHScroll(menuSystem* ms, const wchar_t* code) : menuItem(ms, code)
{
	// constructor code for menuHScroll
    l = new menuHSLeft(ms);

    addChild(l);
}

menuHScroll::~menuHScroll()
{
	// destructor code for menuHScroll
    delete l;
}

void menuHScroll::eventOnInit()
{
	menuItem::eventOnInit();
	setBackgroundStyle(BGS_PLAIN);
	setShapeStyle(BSS_RECTANGLE);
	setResizeStyle(BRS_FIXED);
}

void menuHScroll::eventOnMouseDown()
{
	menuItem::eventOnMouseDown();
}

void menuHScroll::eventOnMouseUp()
{
	menuItem::eventOnMouseUp();
}

void menuHScroll::eventOnMouseExit()
{
	menuItem::eventOnMouseExit();
}


// left

void menuHScroll::menuHSLeft::eventOnMouseDown()
{
	menuButton::eventOnMouseDown();
}

void menuHScroll::menuHSLeft::eventOnMouseUp()
{
	menuButton::eventOnMouseUp();
}

void menuHScroll::menuHSLeft::eventOnClick()
{
	menuButton::eventOnClick();
}

// right

void menuHScroll::menuHSRight::eventOnMouseDown()
{
	menuButton::eventOnMouseDown();
}

void menuHScroll::menuHSRight::eventOnMouseUp()
{
	menuButton::eventOnMouseUp();
}

void menuHScroll::menuHSRight::eventOnClick()
{
	menuButton::eventOnClick();
}