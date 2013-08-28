/* glSystem class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#ifndef __GLSYSTEM_H
#define __GLSYSTEM_H

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#include "glDefines.h"

#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>

class jEngine;
class glRenderable;
class glResolution;
class glScreenMode;
class glTextRender;
class glTextureManager;
class mVector3d;

class glSystem
{
private:
	// pointer to top-level class
	jEngine*			e;

	// pointer to single instance
	static glSystem*	p;

	glScreenMode**		sModes;
	int					numModes;

	glResolution**		resList;
	int					numRes;

	int*				refList;
	int					numRef;

    glTextRender*       defaultText;

	HDC					hDC;
	HGLRC				hRC;
	int					wWidth;
	int					wHeight;
	bool				dblBuf;
	float				asp;

	glRenderable*		consoleRender;
	glRenderable*		menuRender;
	glRenderable*		gameRender;

	wchar_t*		extString;
	wchar_t*		renString;
	wchar_t*		venString;
	wchar_t*		verString;

	// make constructors and assignment private to enforce single instance
	glSystem(jEngine* j);
	glSystem(const glSystem& a);
	glSystem& operator=(const glSystem& a);

	// sort helper functions
	static int compInt(const void* a, const void* b);
	static int compRes(const void* a, const void* b);

	// render helpers
	static void ren(glRenderable* r);
	static void nren(glRenderable* r);

	static void sb(HDC dev);
	static void nsb(HDC dev);

	void (*conr)(glRenderable*);
	void (*menr)(glRenderable*);
	void (*gamr)(glRenderable*);

	void (*swb)(HDC dev);
public:
    glTextureManager*   tex;

	~glSystem();

	// return pointer to our single instance, or create it
	static glSystem* getPointer(jEngine* j);

	bool init(HWND hWnd, bool doubleBuffer);
	void shutdown(HWND hWnd);

	void setConsole(glRenderable* c);
	void setMenu(glRenderable* m);
	void setGame(glRenderable* g);

    static void setVSync(void** p, const wchar_t* str);

	HDC getHDC() { return hDC; }
	int getWidth() { return wWidth; }
	int getHeight() { return wHeight; }
	float getAspectRatio() { return asp; }

	glScreenMode* getScreenMode(int idx);
	glResolution* getResolution(int idx);
	wchar_t* getScreenModeDescription(int idx);
	wchar_t* getResolutionDescription(int idx);
	int getNearestScreenMode(glResolution* res, int refreshRate);
	int getNearestScreenMode(int width, int height, int refreshRate);
	int getNearestResolution(int width, int height);

    glTextRender* getDefaultTextRenderer() { return defaultText; }

	wchar_t* getExtensionString() { return extString; }
	wchar_t* getRendererString() { return renString; }
	wchar_t* getVendorString() { return venString; }
	wchar_t* getVersionString() { return verString; }

	void initGraphics();
	void shutdownGraphics();

	void render();

    float getLastDT();

    class glFuncs
    {
    private:
        // draw buffers
        void (__stdcall* pglDrawBuffers)(int n, const unsigned int *bufs);

        // convolution
        void (__stdcall* pglConvolutionFilter1D)(unsigned int target,unsigned int internalformat,int width,unsigned int format,unsigned int type,const void* image);
        void (__stdcall* pglConvolutionFilter2D)(unsigned int target,unsigned int internalformat,int width,int height,unsigned int format,unsigned int type,const void* image);
        void (__stdcall* pglCopyConvolutionFilter1D)(unsigned int target,unsigned int internalformat,int x,int y,int width);
        void (__stdcall* pglCopyConvolutionFilter2D)(unsigned int target,unsigned int internalformat,int x,int y,int width,int height);
        void (__stdcall* pglGetConvolutionFilter)(unsigned int target,unsigned int format,unsigned int type,void* image);
        void (__stdcall* pglSeparableFilter2D)(unsigned int target,unsigned int internalformat,int width,int height,unsigned int format,unsigned int type,const void* row,const void* column);
        void (__stdcall* pglGetSeparableFilter)(unsigned int target,unsigned int format,unsigned int type,void* row,void* column,void* span);
        void (__stdcall* pglConvolutionParameteri)(unsigned int target,unsigned int pname,int param);
        void (__stdcall* pglConvolutionParameteriv)(unsigned int target,unsigned int pname,const int* params);
        void (__stdcall* pglConvolutionParameterf)(unsigned int target,unsigned int pname,float param);
        void (__stdcall* pglConvolutionParameterfv)(unsigned int target,unsigned int pname,const float* params);
        void (__stdcall* pglGetConvolutionParameteriv)(unsigned int target,unsigned int pname,int* params);
        void (__stdcall* pglGetConvolutionParameterfv)(unsigned int target,unsigned int pname,float* params);

        // multitexture
        void (__stdcall* pglActiveTexture)(unsigned int t);
        void (__stdcall* pglClientActiveTexture)(unsigned int t);
        void (__stdcall* pglMultiTexCoord1i)(unsigned int texunit, int s);
        void (__stdcall* pglMultiTexCoord2i)(unsigned int texunit, int s, int t);
        void (__stdcall* pglMultiTexCoord3i)(unsigned int texunit, int s, int t, int u);
        void (__stdcall* pglMultiTexCoord4i)(unsigned int texunit, int s, int t, int u, int v);
        void (__stdcall* pglMultiTexCoord1f)(unsigned int texunit, float s);
        void (__stdcall* pglMultiTexCoord2f)(unsigned int texunit, float s, float t);
        void (__stdcall* pglMultiTexCoord3f)(unsigned int texunit, float s, float t, float u);
        void (__stdcall* pglMultiTexCoord4f)(unsigned int texunit, float s, float t, float u, float v);

        // point parameter functions
        void (__stdcall* pglPointParameterf)(unsigned int pname, float param);
        void (__stdcall* pglPointParameterfv)(unsigned int pname, const float *params);

        // framebuffer objects
        unsigned char (__stdcall* pglIsRenderbuffer)(unsigned int renderbuffer);
        void (__stdcall* pglBindRenderbuffer)(unsigned int target, unsigned int renderbuffer);
        void (__stdcall* pglDeleteRenderbuffers)(int n, const unsigned int *renderbuffers);
        void (__stdcall* pglGenRenderbuffers)(int n, unsigned int *renderbuffers);
        void (__stdcall* pglRenderbufferStorage)(unsigned int target, unsigned int internalformat,int width, int height);
        void (__stdcall* pglGetRenderbufferParameteriv)(unsigned int target, unsigned int pname, int *params);
        unsigned char (__stdcall* pglIsFramebuffer)(unsigned int framebuffer);
        void (__stdcall* pglBindFramebuffer)(unsigned int target, unsigned int framebuffer);
        void (__stdcall* pglDeleteFramebuffers)(int n, const unsigned int *framebuffers);
        void (__stdcall* pglGenFramebuffers)(int n, unsigned int *framebuffers);
        unsigned int (__stdcall* pglCheckFramebufferStatus)(unsigned int target);
        void (__stdcall* pglFramebufferTexture1D)(unsigned int target, unsigned int attachment,unsigned int textarget, unsigned int texture,int level);
        void (__stdcall* pglFramebufferTexture2D)(unsigned int target, unsigned int attachment,unsigned int textarget, unsigned int texture,int level);
        void (__stdcall* pglFramebufferTexture3D)(unsigned int target, unsigned int attachment,unsigned int textarget, unsigned int texture,int level, int zoffset);
        void (__stdcall* pglFramebufferRenderbuffer)(unsigned int target, unsigned int attachment, unsigned int renderbuffertarget, unsigned int renderbuffer);
        void (__stdcall* pglGetFramebufferAttachmentParameteriv)(unsigned int target, unsigned int attachment, unsigned int pname, int *params);
        void (__stdcall* pglGenerateMipmap)(unsigned int target);

        // shader management
        unsigned int (__stdcall* pglCreateShader)(unsigned int type);
        unsigned int (__stdcall* pglCreateProgram)(void);
        void (__stdcall* pglDeleteShader)(unsigned int shader);
        void (__stdcall* pglDeleteProgram)(unsigned int program);
        void (__stdcall* pglValidateProgram)(unsigned int program);
        void (__stdcall* pglCompileShader)(unsigned int shader);
        void (__stdcall* pglLinkProgram)(unsigned int program);
        void (__stdcall* pglUseProgram)(unsigned int program);
        void (__stdcall* pglAttachShader)(unsigned int program, unsigned int shader);
        void (__stdcall* pglDetachShader)(unsigned int program, unsigned int shader);
        void (__stdcall* pglShaderSource)(unsigned int shader, int nstrings, char** strings, const int* lengths);
        void (__stdcall* pglGetProgramInfoLog)(unsigned int program, int maxlen, int* length, char* infolog);
        void (__stdcall* pglGetShaderInfoLog)(unsigned int shader, int maxlen, int* length, char* infolog);

        int (__stdcall* pglGetUniformLocation)(unsigned int program, const char *name);
        int (__stdcall* pglGetAttribLocation) (unsigned int program, const char *name);
        void (__stdcall* pglBindAttribLocation) (unsigned int program, unsigned int index, const char *name);

        // vertex attribs
        void (__stdcall* pglVertexAttrib4f) (unsigned int index, float x, float y, float z, float w);
        void (__stdcall* pglVertexAttrib3f) (unsigned int index, float x, float y, float z);
        void (__stdcall* pglVertexAttrib2f) (unsigned int index, float x, float y);
        void (__stdcall* pglVertexAttrib1f) (unsigned int index, float x);

        void (__stdcall* pglVertexAttrib4i) (unsigned int index, int x, int y, int z, int w);
        void (__stdcall* pglVertexAttrib3i) (unsigned int index, int x, int y, int z);
        void (__stdcall* pglVertexAttrib2i) (unsigned int index, int x, int y);
        void (__stdcall* pglVertexAttrib1i) (unsigned int index, int x);

        // uniforms
        void (__stdcall* pglUniform4i)(int location, int v0, int v1, int v2, int v3);
        void (__stdcall* pglUniform3i)(int location, int v0, int v1, int v2);
        void (__stdcall* pglUniform2i)(int location, int v0, int v1);
        void (__stdcall* pglUniform1i)(int location, int v0);

        void (__stdcall* pglUniform4f)(int location, float v0, float v1, float v2, float v3);
        void (__stdcall* pglUniform3f)(int location, float v0, float v1, float v2);
        void (__stdcall* pglUniform2f)(int location, float v0, float v1);
        void (__stdcall* pglUniform1f)(int location, float v0);

        // WGL_EXT_swap_control for vsync
        void (__stdcall* pwglSwapInterval)(unsigned int val);

        // static null functions
        static void __stdcall nDrawBuffers(int n, const unsigned int *bufs) { return; }

        static void __stdcall nConvolutionFilter1D(unsigned int target,unsigned int internalformat,int width,unsigned int format,unsigned int type,const void* image) { return; }
        static void __stdcall nConvolutionFilter2D(unsigned int target,unsigned int internalformat,int width,int height,unsigned int format,unsigned int type,const void* image) { return; }
        static void __stdcall nCopyConvolutionFilter1D(unsigned int target,unsigned int internalformat,int x,int y,int width) { return; }
        static void __stdcall nCopyConvolutionFilter2D(unsigned int target,unsigned int internalformat,int x,int y,int width,int height) { return; }
        static void __stdcall nGetConvolutionFilter(unsigned int target,unsigned int format,unsigned int type,void* image) { return; }
        static void __stdcall nSeparableFilter2D(unsigned int target,unsigned int internalformat,int width,int height,unsigned int format,unsigned int type,const void* row,const void* column) { return; }
        static void __stdcall nGetSeparableFilter(unsigned int target,unsigned int format,unsigned int type,void* row,void* column,void* span) { return; }
        static void __stdcall nConvolutionParameteri(unsigned int target,unsigned int pname,int param) { return; }
        static void __stdcall nConvolutionParameteriv(unsigned int target,unsigned int pname,const int* params) { return; }
        static void __stdcall nConvolutionParameterf(unsigned int target,unsigned int pname,float param) { return; }
        static void __stdcall nConvolutionParameterfv(unsigned int target,unsigned int pname,const float* params) { return; }
        static void __stdcall nGetConvolutionParameteriv(unsigned int target,unsigned int pname,int* params) { return; }
        static void __stdcall nGetConvolutionParameterfv(unsigned int target,unsigned int pname,float* params) { return; }

	    static void __stdcall nActiveTexture(unsigned int t) { return; }
	    static void __stdcall nClientActiveTexture(unsigned int t) { return; }
	    static void __stdcall nMultiTexCoord1i(unsigned int texunit, int s) { glTexCoord1i(s); }
        static void __stdcall nMultiTexCoord2i(unsigned int texunit, int s, int t) { glTexCoord2i(s, t); }
        static void __stdcall nMultiTexCoord3i(unsigned int texunit, int s, int t, int u) { glTexCoord3i(s, t, u); }
        static void __stdcall nMultiTexCoord4i(unsigned int texunit, int s, int t, int u, int v) { glTexCoord4i(s, t, u, v); }
        static void __stdcall nMultiTexCoord1f(unsigned int texunit, float s) { glTexCoord1f(s); }
        static void __stdcall nMultiTexCoord2f(unsigned int texunit, float s, float t) { glTexCoord2f(s, t); }
        static void __stdcall nMultiTexCoord3f(unsigned int texunit, float s, float t, float u) { glTexCoord3f(s, t, u); }
        static void __stdcall nMultiTexCoord4f(unsigned int texunit, float s, float t, float u, float v) { glTexCoord4f(s, t, u, v); }

        static void __stdcall nPointParameterf(unsigned int pname, float param) { return; }
        static void __stdcall nPointParameterfv(unsigned int pname, const float *params) { return; }

        static unsigned char __stdcall nIsRenderbuffer(unsigned int renderbuffer) { return 0; }
        static void __stdcall nBindRenderbuffer(unsigned int target, unsigned int renderbuffer) { return; }
        static void __stdcall nDeleteRenderbuffers(int n, const unsigned int *renderbuffers) { return; }
        static void __stdcall nGenRenderbuffers(int n, unsigned int *renderbuffers) { return; }
        static void __stdcall nRenderbufferStorage(unsigned int target, unsigned int internalformat,int width, int height) { return; }
        static void __stdcall nGetRenderbufferParameteriv(unsigned int target, unsigned int pname, int *params) { return; }
        static unsigned char __stdcall nIsFramebuffer(unsigned int framebuffer) { return 0; }
        static void __stdcall nBindFramebuffer(unsigned int target, unsigned int framebuffer) { return; }
        static void __stdcall nDeleteFramebuffers(int n, const unsigned int *framebuffers) { return; }
        static void __stdcall nGenFramebuffers(int n, unsigned int *framebuffers) { return; }
        static unsigned int __stdcall nCheckFramebufferStatus(unsigned int target) { return 0; }
        static void __stdcall nFramebufferTexture1D(unsigned int target, unsigned int attachment,unsigned int textarget, unsigned int texture,int level) { return; }
        static void __stdcall nFramebufferTexture2D(unsigned int target, unsigned int attachment,unsigned int textarget, unsigned int texture,int level) { return; }
        static void __stdcall nFramebufferTexture3D(unsigned int target, unsigned int attachment,unsigned int textarget, unsigned int texture,int level, int zoffset) { return; }
        static void __stdcall nFramebufferRenderbuffer(unsigned int target, unsigned int attachment, unsigned int renderbuffertarget, unsigned int renderbuffer) { return; }
        static void __stdcall nGetFramebufferAttachmentParameteriv(unsigned int target, unsigned int attachment, unsigned int pname, int *params) { return; }
        static void __stdcall nGenerateMipmap(unsigned int target) { return; }

        static unsigned int __stdcall nCreateShader(unsigned int type) { return 0; }
        static unsigned int __stdcall nCreateProgram() { return 0; }
        static void __stdcall nDeleteShader(unsigned int shader) { }
        static void __stdcall nDeleteProgram(unsigned int program) { }
        static void __stdcall nValidateProgram(unsigned int program) { }
        static void __stdcall nCompileShader(unsigned int shader) { }
        static void __stdcall nLinkProgram(unsigned int program) { }
        static void __stdcall nUseProgram(unsigned int program) { }
        static void __stdcall nAttachShader(unsigned int program, unsigned int shader) { }
        static void __stdcall nShaderSource(unsigned int shader, int nstrings, char **strings, const int *lengths) { }
        static void __stdcall nGetProgramInfoLog(unsigned int program, int maxlen, int* length, char* infolog) { }
        static void __stdcall nGetShaderInfoLog(unsigned int shader, int maxlen, int* length, char* infolog) { }

        static int __stdcall nGetUniformLocation(unsigned int program, const char *name) { return 0; }
        static int __stdcall nGetAttribLocation(unsigned int program, const char *name) { return 0; }
        static void __stdcall nBindAttribLocation(unsigned int program, unsigned int index, const char *name) { }

        static void __stdcall nVertexAttrib4f(unsigned int index, float x, float y, float z, float w) { }
        static void __stdcall nVertexAttrib3f(unsigned int index, float x, float y, float z) { }
        static void __stdcall nVertexAttrib2f(unsigned int index, float x, float y) { }
        static void __stdcall nVertexAttrib1f(unsigned int index, float x) { }
        static void __stdcall nVertexAttrib4i(unsigned int index, int x, int y, int z, int w) { }
        static void __stdcall nVertexAttrib3i(unsigned int index, int x, int y, int z) { }
        static void __stdcall nVertexAttrib2i(unsigned int index, int x, int y) { }
        static void __stdcall nVertexAttrib1i(unsigned int index, int x) { }
        static void __stdcall nUniform4i(int location, int v0, int v1, int v2, int v3) { }
        static void __stdcall nUniform3i(int location, int v0, int v1, int v2) { }
        static void __stdcall nUniform2i(int location, int v0, int v1) { }
        static void __stdcall nUniform1i(int location, int v0) { }
        static void __stdcall nUniform4f(int location, float x, float y, float z, float w) { }
        static void __stdcall nUniform3f(int location, float x, float y, float z) { }
        static void __stdcall nUniform2f(int location, float x, float y) { }
        static void __stdcall nUniform1f(int location, float x) { }

        static void __stdcall nSwapInterval(unsigned int val) { }

	    void set120ext();
        void setshadext();
        void setfboext();
        void set14exts();
        void set14base();
        void setmtbase();
        void setmtext();
        void setvsext();
    public:
        glFuncs();

        bool SupportsFramebufferObjects;
        bool SupportsShaders;
        bool SupportsRectangleTextures;
        bool SupportsDepthTextures;
        bool SupportsMultitexture;
        bool SupportsPointParameters;
        bool SupportsMipmapGeneration;
        bool SupportsLODBias;
        bool Supports200;
        bool Supports140;
        bool Supports130;
        bool Supports120;

        void Finish() { glFinish(); }
        void Flush() { glFlush(); }

        void Begin(const unsigned int& en) const { glBegin(en); }
        void End() const { glEnd(); }
        void Disable(const unsigned int& cap) const { glDisable(cap); }
        void Enable(const unsigned int& cap) const { glEnable(cap); }
        void Hint(unsigned int target, unsigned int mode) const { glHint(target, mode); }

        void Clear(const unsigned int& mask) const { glClear(mask); }
        void ClearColor(const float& r, const float& g, const float& b, const float& a) const { glClearColor(r, g, b, a); }
        void ClearDepth(double d) const { glClearDepth(d); }

        void Translate(const float& x, const float& y, const float& z) const { glTranslatef(x, y, z); }
        void Translate(const double& x, const double& y, const double& z) const { glTranslated(x, y, z); }

        void Rotate(const float& a, const float& x, const float& y, const float& z) const { glRotatef(a, x, y, z); }
        void Rotate(const double& a, const double& x, const double& y, const double& z) const { glRotated(a, x, y, z); }

        void Vertex(const int& x, const int& y) const { glVertex2i(x, y); }
        void Vertex(const int& x, const int& y, const int& z) const { glVertex3i(x, y, z); }
        void Vertex(const int& x, const int& y, const int& z, int w) const { glVertex4i(x, y, z, w); }
        void Vertex(const float& x, const float& y) const { glVertex2f(x, y); }
        void Vertex(const float& x, const float& y, const float& z) const { glVertex3f(x, y, z); }
        void Vertex(const float& x, const float& y, const float& z, float w) const { glVertex4f(x, y, z, w); }
        void Vertex(mVector3d& v) const;

        void Normal(const int& x, const int& y, const int& z) const { glNormal3i(x, y, z); }
        void Normal(const float& x, const float& y, const float& z) const { glNormal3f(x, y, z); }
        void Normal(mVector3d& v) const;

        void TexCoord(const int& x, const int& y) const { glTexCoord2i(x, y); }
        void TexCoord(const int& x, const int& y, const int& z) const { glTexCoord3i(x, y, z); }
        void TexCoord(const int& x, const int& y, const int& z, const int& w) const { glTexCoord4i(x, y, z, w); }
        void TexCoord(const float& x, const float& y) const { glTexCoord2f(x, y); }
        void TexCoord(const float& x, const float& y, const float& z) const { glTexCoord3f(x, y, z); }
        void TexCoord(const float& x, const float& y, const float& z, const float& w) const { glTexCoord4f(x, y, z, w); }

        void Color(const int& r, const int& g, const int& b) const { glColor3i(r, g, b); }
        void Color(const int& r, const int& g, const int& b, const int& a) const { glColor4i(r, g, b, a); }
        void Color(unsigned char r, unsigned char g, unsigned char b) const { glColor3ub(r, g, b); }
        void Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) const { glColor4ub(r, g, b, a); }
        void Color(const float& r, const float& g, const float& b) const { glColor3f(r, g, b); }
        void Color(const float& r, const float& g, const float& b, const float& a) const { glColor4f(r, g, b, a); }
        void Color(mVector3d& v) const;

        void BlendFunc(const unsigned int& sfactor, const unsigned int& dfactor) const { glBlendFunc(sfactor, dfactor); }
        void DepthFunc(const unsigned int& func) const { glDepthFunc(func); }
        void CullFace(const unsigned int& mode) const { glCullFace(mode); }
        void FrontFace(const unsigned int& mode) const { glFrontFace(mode); }

        void LoadIdentity() const { glLoadIdentity(); }
        void MatrixMode(const unsigned int& mode) const { glMatrixMode(mode); }
        void PopAttrib() const { glPopAttrib(); }
        void PopMatrix() const { glPopMatrix(); }
        void PushAttrib(const unsigned int& mask) const { glPushAttrib(mask); }
        void PushMatrix() const { glPushMatrix(); }

        void Ortho(double left, double right, double bottom, double top, double znear, double zfar) { glOrtho(left, right, bottom, top, znear, zfar); }
        void Perspective(double fovy = 90.0, float asp = 1.333f, double znear = 0.1, double zfar = 1000) { gluPerspective(fovy, (double)asp, znear, zfar); }

        // extensions
        void DrawBuffers(int n, const unsigned int *bufs);

        void ConvolutionFilter1D(unsigned int target,unsigned int internalformat,int width,unsigned int format,unsigned int type,const void* image);
        void ConvolutionFilter2D(unsigned int target,unsigned int internalformat,int width,int height,unsigned int format,unsigned int type,const void* image);
        void CopyConvolutionFilter1D(unsigned int target,unsigned int internalformat,int x,int y,int width);
        void CopyConvolutionFilter2D(unsigned int target,unsigned int internalformat,int x,int y,int width,int height);
        void GetConvolutionFilter(unsigned int target,unsigned int format,unsigned int type,void* image);
        void SeparableFilter2D(unsigned int target,unsigned int internalformat,int width,int height,unsigned int format,unsigned int type,const void* row,const void* column);
        void GetSeparableFilter(unsigned int target,unsigned int format,unsigned int type,void* row,void* column,void* span);
        void ConvolutionParameter(unsigned int target,unsigned int pname,int param);
        void ConvolutionParameter(unsigned int target,unsigned int pname,const int* params);
        void ConvolutionParameter(unsigned int target,unsigned int pname,float param);
        void ConvolutionParameter(unsigned int target,unsigned int pname,const float* params);
        void GetConvolutionParameter(unsigned int target,unsigned int pname,int* params);
        void GetConvolutionParameter(unsigned int target,unsigned int pname,float* params);

	    void ActiveTexture(unsigned int t);
	    void ClientActiveTexture(unsigned int t);
	    void MultiTexCoord(unsigned int texunit, int s); 
        void MultiTexCoord(unsigned int texunit, int s, int t); 
        void MultiTexCoord(unsigned int texunit, int s, int t, int u); 
        void MultiTexCoord(unsigned int texunit, int s, int t, int u, int v); 
        void MultiTexCoord(unsigned int texunit, float s); 
        void MultiTexCoord(unsigned int texunit, float s, float t); 
        void MultiTexCoord(unsigned int texunit, float s, float t, float u); 
        void MultiTexCoord(unsigned int texunit, float s, float t, float u, float v);  

        void PointParameter(unsigned int pname, float param);
        void PointParameter(unsigned int pname, const float *params);

        unsigned char IsRenderbuffer(unsigned int renderbuffer);
        void BindRenderbuffer(unsigned int target, unsigned int renderbuffer);
        void DeleteRenderbuffers(int n, const unsigned int *renderbuffers);
        void GenRenderbuffers(int n, unsigned int *renderbuffers);
        void RenderbufferStorage(unsigned int target, unsigned int internalformat,int width, int height);
        void GetRenderbufferParameteriv(unsigned int target, unsigned int pname, int *params);
        unsigned char IsFramebuffer(unsigned int framebuffer);
        void BindFramebuffer(unsigned int target, unsigned int framebuffer);
        void DeleteFramebuffers(int n, const unsigned int *framebuffers);
        void GenFramebuffers(int n, unsigned int *framebuffers);
        unsigned int CheckFramebufferStatus(unsigned int target);
        void FramebufferTexture1D(unsigned int target, unsigned int attachment,unsigned int textarget, unsigned int texture,int level);
        void FramebufferTexture2D(unsigned int target, unsigned int attachment,unsigned int textarget, unsigned int texture,int level);
        void FramebufferTexture3D(unsigned int target, unsigned int attachment,unsigned int textarget, unsigned int texture,int level, int zoffset);
        void FramebufferRenderbuffer(unsigned int target, unsigned int attachment, unsigned int renderbuffertarget, unsigned int renderbuffer);
        void GetFramebufferAttachmentParameteriv(unsigned int target, unsigned int attachment, unsigned int pname, int *params);
        void GenerateMipmap(unsigned int target);

	    unsigned int CreateShader(unsigned int type);
	    unsigned int CreateProgram();
	    void DeleteShader(unsigned int shader);
	    void DeleteProgram(unsigned int program);
	    void ValidateProgram(unsigned int program);
	    void CompileShader(unsigned int shader);
	    void LinkProgram(unsigned int program);
	    void UseProgram(unsigned int program);
	    void AttachShader(unsigned int program, unsigned int shader);
	    void ShaderSource(unsigned int shader, int nstrings, char **strings, const int *lengths);
	    void GetProgramInfoLog(unsigned int program, int maxlen, int* length, char* infolog);
	    void GetShaderInfoLog(unsigned int shader, int maxlen, int* length, char* infolog);

	    int GetUniformLocation(unsigned int program, const char *name);
	    int GetAttribLocation(unsigned int program, const char *name);
	    void BindAttribLocation(unsigned int program, unsigned int index, const char *name);

        void VertexAttrib(unsigned int index, float x, float y, float z, float w);
        void VertexAttrib(unsigned int index, float x, float y, float z);
        void VertexAttrib(unsigned int index, float x, float y);
        void VertexAttrib(unsigned int index, float x);
        void VertexAttrib(unsigned int index, int x, int y, int z, int w);
        void VertexAttrib(unsigned int index, int x, int y, int z);
        void VertexAttrib(unsigned int index, int x, int y);
        void VertexAttrib(unsigned int index, int x);
	    void Uniform(int location, int v0, int v1, int v2, int v3);
        void Uniform(int location, int v0, int v1, int v2);
        void Uniform(int location, int v0, int v1);
        void Uniform(int location, int v0);
	    void Uniform(int location, float x, float y, float z, float w);
        void Uniform(int location, float x, float y, float z);
        void Uniform(int location, float x, float y);
        void Uniform(int location, float x);

        void SwapInterval(const unsigned int& val) const { pwglSwapInterval(val); }
    };

   glFuncs* f;
};

#endif