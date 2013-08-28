/* glResolution class by Semi Essessi
 *
 * Screen resolution wrapper
 *
 */

#ifndef __GLRESOLUTION_H
#define __GLRESOLUTION_H

class glResolution
{
private:
	int w;
	int h;
	
	wchar_t*	descriptor;
public:
	glResolution(int width, int height);
	~glResolution();

	int getWidth() { return w; }
	int getHeight() { return h; }

	wchar_t* getDescription() { return descriptor; }
	
	int calcDifference(int screenWidth, int screenHeight);
};

#endif