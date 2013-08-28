/* glResolution class by Semi Essessi
 *
 * Screen resolution wrapper
 *
 */

#include "glResolution.h"

#include <malloc.h>
#include <math.h>
#include <stdio.h>

glResolution::glResolution(int width, int height)
{
	// constructor code for glResolution
	w=width;
	h=height;

	descriptor = (wchar_t*)malloc(sizeof(wchar_t)*16);
	int len = swprintf(descriptor, L"%ix%i", w, h)+1;
	descriptor = (wchar_t*)realloc(descriptor,sizeof(wchar_t)*len);
}

glResolution::~glResolution()
{
	// destructor code for glResolution
	free(descriptor);
}

int glResolution::calcDifference(int screenWidth, int screenHeight)
{
	return abs(screenWidth-w)+abs(screenHeight-h);
}