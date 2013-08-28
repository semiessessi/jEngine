/* glScreenMode class by Semi Essessi
 *
 * Screen mode wrapper
 *
 */

#include "glScreenMode.h"

#include "log.h"

#include <malloc.h>
#include <math.h>
#include <stdio.h>

glScreenMode::glScreenMode(int screenWidth, int screenHeight, int colorDepth, int refreshRate)
{
	// constructor code for glScreenMode
	width = screenWidth;
	height = screenHeight;
	bpp = colorDepth;
	refresh = refreshRate;

	descriptor = (wchar_t*)malloc(sizeof(wchar_t)*32);
	int len = swprintf(descriptor, L"%ix%ix%ibpp @ %iHz", width, height, bpp, refresh)+1;
	descriptor = (wchar_t*)realloc(descriptor,sizeof(wchar_t)*len);

#ifdef _DEBUG
	LogWriteLine(L"\x25CB\x25CB\x25CF %s screenmode detected", descriptor);
#endif
}

glScreenMode::~glScreenMode()
{
	// destructor code for glScreenMode
	free(descriptor);
}

int glScreenMode::calcDifference(int screenWidth, int screenHeight, int refreshRate)
{
	int i = abs(screenWidth-width)+abs(screenHeight-height);
	if(i) return i;
	return abs(refreshRate-refresh);
}