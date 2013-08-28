/* glTextRender class by Semi Essessi
 *
 * FreeType2 empowered text renderer
 *
 */

#include "glTextRender.h"
#include "jEngine.h"

#include "glSystem.h"

#include "jMath.h"
#include "jProfiler.h"
#include "log.h"

#include <windows.h>

FT_Library glTextRender::ftLib = 0;

//TODO::maybe fix this so it use wchar_t for the filename
glTextRender::glTextRender(jEngine* j, const char* fontName, int size = 12)
{
#ifdef _DEBUG
	jProfiler profiler = jProfiler(L"glTextRender::glTextRender");
#endif
	char fontName2[1024];
	memset(fontName2,0,sizeof(char)*1024);
	sprintf(fontName2,"fonts\\%s",fontName);

	if(!ftLib)
	{
#ifdef _DEBUG
		wchar_t fn[128];
		mbstowcs(fn,fontName,strlen(fontName)+1);
		LogWriteLine(L"\x25CB\x25CB\x25CF Font: %s was requested before FreeType was initialised", fn);
#endif
		return;
	}

	// constructor code for glTextRender
	e=j;

	gl = j->gl;

	red=green=blue=alpha=1.0f;

	face = 0;

	// remember that for some weird reason below font size 7 everything gets scrambled up
	height = max(6,(int)floorf((float)size*((float)gl->getHeight())*0.001666667f));
	aHeight = ((float)height)/((float)gl->getHeight());

	setPosition(0.0f,0.0f);
	
	// look in base fonts dir
	if(FT_New_Face(ftLib, fontName2, 0, &face ))
	{
		// if we dont have it look in windows fonts dir
		char buf[1024];
		GetWindowsDirectoryA(buf,1024);
		strcat(buf, "\\fonts\\");
		strcat(buf, fontName);

		if(FT_New_Face(ftLib, buf, 0, &face ))
		{
			//TODO::check in mod fonts directory
#ifdef _DEBUG
			wchar_t fn[128];
			mbstowcs(fn,fontName,strlen(fontName)+1);
			LogWriteLine(L"\x25CB\x25CB\x25CF Request for font: %s has failed", fn);
#endif
			face = 0;
			return;
		}
	}

	// FreeType uses 64x size and 72dpi for default
	// doubling size for ms 
	FT_Set_Char_Size(face, mulPow2(height,7), mulPow2(height,7), 96, 96);

	// set up cache table and then generate the first 256 chars and the console prompt character
	for(int i=0;i<65536;i++) 
	{
		cached[i]=false;
		width[i]=0.0f;
	}

	for(unsigned short i = 0; i < 256; i++) getChar((wchar_t)i);
	getChar(CHAR_PROMPT);

#ifdef _DEBUG
	wchar_t fn[128];
	mbstowcs(fn,fontName,strlen(fontName)+1);
	LogWriteLine(L"\x25CB\x25CB\x25CF Font: %s loaded OK", fn);
#endif
}

glTextRender::~glTextRender()
{
	// destructor code for glTextRender
	for(int i=0;i<65536;i++)
	{
		if(cached[i])
		{
			glDeleteLists(listID[i],1);
			glDeleteTextures(1,&(texID[i]));
		}
	}

	// TODO:: work out stupid freetype crashz0rs
	try
	{
        static int foo = 0;
		if(face && foo < 1)
		{
            foo++;
			FT_Done_Face(face);
			face = 0;
		}
	}
	catch(...)
	{
		face = 0;
	}
}


// return true if init works, or if already initialised
bool glTextRender::initFreeType()
{
	if(!ftLib)
	{
		if(!FT_Init_FreeType(&ftLib)) return true;
		else return false;
	} else return true;
}

void glTextRender::shutdownFreeType()
{
	if(ftLib)
	{
		FT_Done_FreeType(ftLib);
		ftLib = 0;
	}
}

void glTextRender::print(const wchar_t* str)
{
	// store old stuff to set start position
	glPushAttrib(GL_TRANSFORM_BIT);
	// get viewport size
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	
	gluOrtho2D(viewport[0],viewport[2],viewport[1],viewport[3]);
	glPopAttrib();

	float color[4];
	glGetFloatv(GL_CURRENT_COLOR, color);

	glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT  | GL_ENABLE_BIT | GL_TRANSFORM_BIT); 
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);
	//glDisable(GL_DEPTH_TEST);
	
	// set blending for AA
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glTranslatef(xPos,yPos,0.0f);

	glColor4f(red,green,blue,alpha);

	// call display lists to render text
	glListBase(0u);
	for(unsigned int i=0;i<wcslen(str);i++) glCallList(getChar(str[i]));
	
	// restore old states
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glPopAttrib();

	glColor4fv(color);

	glPushAttrib(GL_TRANSFORM_BIT);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
}

void glTextRender::printf(const wchar_t* str, ...)
{
	if(!str) return;

	wchar_t*	buf = 0;
	va_list	 parg;
	va_start(parg, str);
	
	// allocate buffer
	int len = (_vscwprintf(str, parg)+1);
	buf = new wchar_t[len];
	if(!buf) return;
	vswprintf(buf, str, parg);
	va_end(parg);

	print(buf);

	delete[] buf;
}

GLuint glTextRender::getChar(const wchar_t c)
{
	int i = (int)c;

	if(cached[i]) return listID[i];

	// load glyph and get bitmap
	if(FT_Load_Glyph(face, FT_Get_Char_Index(face, i), FT_LOAD_DEFAULT )) return 0;

	FT_Glyph glyph;
	if(FT_Get_Glyph(face->glyph, &glyph)) return 0;

	FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1);

	FT_BitmapGlyph bitmapGlyph = (FT_BitmapGlyph)glyph;
	FT_Bitmap& bitmap = bitmapGlyph->bitmap;
	
	int w = roundPow2(bitmap.width);
	int h = roundPow2(bitmap.rows);

	// convert to texture in memory
	GLubyte* texture = new GLubyte[2*w*h];

	for(int j=0;j<h;j++)
    {
        bool cond = j>=bitmap.rows;

	    for(int k=0;k<w;k++)
	    {
			    texture[2*(k+j*w)] = 0xFFu;
			    texture[2*(k+j*w)+1] = ((k>=bitmap.width)||cond) ? 0x0u : bitmap.buffer[k+bitmap.width*j];
	    }
    }

	// store char width and adjust max height
	// note .5f
    float ih = 1.0f/((float)gl->getHeight());
	width[i] = ((float)divPow2(face->glyph->advance.x, 7))*ih;
	aHeight = max(aHeight,(.5f*(float)bitmap.rows)*ih);
	
	glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT  | GL_ENABLE_BIT | GL_TRANSFORM_BIT);

	// create gl texture
	glGenTextures(1, &(texID[i]));

	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, texID[i]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, texture);

	glPopAttrib();
	
	delete[] texture;

	// create display list
	listID[i] = glGenLists(1);

	glNewList(listID[i], GL_COMPILE);

	glBindTexture(GL_TEXTURE_2D, texID[i]);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	// adjust position to account for texture padding
	glTranslatef(.5f*(float)bitmapGlyph->left, 0.0f, 0.0f);
	glTranslatef(0.0f, .5f*(float)(bitmapGlyph->top-bitmap.rows), 0.0f);

	// work out texcoords
	float tx=((float)bitmap.width)/((float)w);
	float ty=((float)bitmap.rows)/((float)h);

	// render
	// note .5f
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(0.0f, .5f*(float)bitmap.rows);
		glTexCoord2f(0.0f, ty);
		glVertex2f(0.0f, 0.0f);
		glTexCoord2f(tx, ty);
		glVertex2f(.5f*(float)bitmap.width, 0.0f);
		glTexCoord2f(tx, 0.0f);
		glVertex2f(.5f*(float)bitmap.width, .5f*(float)bitmap.rows);
	glEnd();

	glPopMatrix();

	// move position for the next character
	// note extra div 2
	glTranslatef((float)divPow2(face->glyph->advance.x, 7), 0.0f, 0.0f);

	glEndList();

	// char is succesfully cached for next time
	cached[i] = true;

	return listID[i];
}

void glTextRender::setPosition(float x, float y)
{
	float fac = ((float)gl->getHeight());
	xPos = fac*x+FONT_BORDER_PIXELS; yPos = fac*(1-y)-(float)height-FONT_BORDER_PIXELS;
}

float glTextRender::getAdjustedWidth(const wchar_t* str)
{
	float w = 0.0f;

	for(unsigned int i=0;i<wcslen(str);i++)
	{
		if(cached[str[i]]) w+=width[str[i]];
		else
		{
			getChar(str[i]);
			w+=width[str[i]];
		}
	}

	return w;
}