/* glTextRender class by Semi Essessi
 *
 * FreeType2 empowered text renderer
 *
 */

#ifndef __GLTEXTRENDER_H
#define __GLTEXTRENDER_H

#pragma comment(lib, "zlib.lib")
#ifndef _DEBUG
#pragma comment(lib, "freetype.lib")
#else
#pragma comment(lib, "freetyped.lib")
#endif

class jEngine;
class glSystem;

#include "glSystem.h"

#include <ft2build.h>  
#include FT_FREETYPE_H
#include FT_GLYPH_H

#define FONT_BORDER_PIXELS 2
#define FREETYPE_LEGAL L"Portions of this software are copyright © 2006 The FreeType Project (www.freetype.org).  All rights reserved."

#define CHAR_PROMPT	L'>'
#define STR_PROMPT	L">"

class glTextRender
{
private:
	// pointer to top-level class
	jEngine*			e;
	glSystem*			gl;

	static FT_Library	ftLib;
	
	int					height;
	float				aHeight;
	FT_Face				face;

	bool				cached[65536];
	GLuint				listID[65536];
	GLuint				texID[65536];
	float				width[65536];

	float				xPos;
	float				yPos;

	float				red;
	float				green;
	float				blue;
	float				alpha;

	GLuint getChar(const wchar_t c);
public:
	glTextRender(jEngine* j, const char* fontName, int s);
	~glTextRender();

	void setPosition(float x, float y);
	void setColor(float r, float g, float b, float a) { red = r; blue = b; green = g; alpha = a; }

	void print(const wchar_t* str);
	void printf(const wchar_t* str, ...);

	int getHeight() { return height; }
	float getAdjustedHeight() { return aHeight; }
	float getAdjustedWidth(const wchar_t* str);

	static bool initFreeType();
	static void shutdownFreeType();
};

#endif