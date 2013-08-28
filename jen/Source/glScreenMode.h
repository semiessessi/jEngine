/* glScreenMode class by Semi Essessi
 *
 * Screen mode wrapper
 *
 */

#ifndef __GLSCREENMODE_H
#define __GLSCREENMODE_H

class glScreenMode
{
private:
	int		width;
	int		height;
	int		bpp;
	int		refresh;

	wchar_t*	descriptor;
public:
	glScreenMode(int screenWidth, int screenHeight, int colorDepth, int refreshRate);
	~glScreenMode();

	int getWidth() { return width; }
	int getHeight() { return height; }
	int getBitsPerPixel() { return bpp; }
	int getRefreshRate() { return refresh; }

	wchar_t* getDescription() { return descriptor; }

	int calcDifference(int screenWidth, int screenHeight, int refreshRate);
};

#endif