/* glSubTexture class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#include "glSubTexture.h"
#include "jEngine.h"

#include "glSystem.h"

#include "jMath.h"
#include "log.h"

glSubTexture::glSubTexture(const wchar_t* str)
{
	// constructor code for glSubTexture
	e = jEngine::getPointer();
	gl = e->gl;

	getTCX = defMod;
	getTCY = defMod;
	getR = defMod;
	getG = defMod;
	getB = defMod;
	getA = defMod;

	filename = 0;

	// parse script
}

glSubTexture::~glSubTexture()
{
	// destructor code for glSubTexture
	if(filename) free(filename);
}

unsigned int glSubTexture::rgbatou(const float r, const float g, const float b, const float a)
{
	return ((min(255,max(0,(unsigned int)floorf(r*255.0f)))<<24)
		|(min(255,max(0,(unsigned int)floorf(r*255.0f)))<<16)
		|(min(255,max(0,(unsigned int)floorf(r*255.0f)))<<8)
		|(min(255,max(0,(unsigned int)floorf(r*255.0f)))));
}

unsigned int* glSubTexture::texPlain(const int w, const int h, const float r, const float g, const float b, const float a)
{
	int size = w*h;
	unsigned int c = rgbatou(r,g,b,a);	

	unsigned int* ret = new unsigned int[size];

	for(int i=0;i<size;i++)
	{
		ret[i] = c;
	}

	return ret;
}