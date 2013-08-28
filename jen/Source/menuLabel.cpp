/* menuLabel class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#include "menuLabel.h"

menuLabel::menuLabel(menuSystem* ms, const wchar_t* code) : menuItem(ms, code)
{
	// constructor code for menuLabel
	setText(L"Label");
}

menuLabel::~menuLabel()
{
	// destructor code for menuLabel
}

void menuLabel::eventOnInit()
{
	menuItem::eventOnInit();
	setVisualStyle(BVS_NONE);
	setBackgroundStyle(BGS_NONE);
	setShapeStyle(BSS_RECTANGLE);
	setResizeStyle(BRS_FIXED);
}