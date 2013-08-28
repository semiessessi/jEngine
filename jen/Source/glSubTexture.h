/* glSubTexture class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#ifndef __GLSUBTEXTURE_H
#define __GLSUBTEXTURE_H

#include "glSystem.h"

class jEngine;

class glSubTexture
{
private:
	// pointer to top-level class
	jEngine*	e;
	glSystem*	gl;

	wchar_t*	filename;
	unsigned int	texID;

	// stuff for scriptable texture coord transforms and colour values
	float		(*getTCX)(float, float**);
	float		(*getTCY)(float, float**);
	float		(*getR)(float, float**);
	float		(*getG)(float, float**);
	float		(*getB)(float, float**);
	float		(*getA)(float, float**);

	float**		tcxParams;  // parameters for texcoord x
	float**		tcyParams;  // parameters for texcoord y
	float**		rParams;    // red..
	float**		gParams;    // etc...
	float**		bParams;
	float**		aParams;

	int			ntcxParams; // count of each of the param types
	int			ntcyParams;
	int			nrParams;
	int			ngParams;
	int			nbParams;
	int			naParams;

	static unsigned int rgbatou(const float r, const float g, const float b, const float a);

	static unsigned int* texPlain(const int w, const int h, const float r, const float g, const float b, const float a);
public:
	glSubTexture(const wchar_t* str);
	~glSubTexture();

	static float defMod(float in, float** params) { return in; }
};

#endif