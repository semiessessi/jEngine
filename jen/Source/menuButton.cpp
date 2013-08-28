/* menuButton class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#include "menuButton.h"

menuButton::menuButton(menuSystem* ms, const wchar_t* code) : menuItem(ms, code)
{
	// constructor code for menuButton
	setText(L"Button");
}

menuButton::~menuButton()
{
	// destructor code for menuButton
}

void menuButton::eventOnInit()
{
	menuItem::eventOnInit();
	setVisualStyle(BVS_BEVEL_OUT);
	setBackgroundStyle(BGS_PLAIN);
	setShapeStyle(BSS_RECTANGLE);
	setResizeStyle(BRS_FIXED);
}

void menuButton::eventOnMouseDown()
{
	menuItem::eventOnMouseDown();
	setVisualStyle(BVS_BEVEL_IN);
}

void menuButton::eventOnMouseUp()
{
	menuItem::eventOnMouseUp();
	setVisualStyle(BVS_BEVEL_OUT);
}

void menuButton::eventOnMouseExit()
{
	menuItem::eventOnMouseExit();
	setVisualStyle(BVS_BEVEL_OUT);
}