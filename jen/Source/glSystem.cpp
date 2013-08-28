/* glSystem class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#include "glSystem.h"
#include "jEngine.h"

#include "glRenderable.h"
#include "glResolution.h"
#include "glScreenMode.h"
#include "glTextRender.h"
#include "glTextureManager.h"

#include "console.h"
#include "cVar.h"
#include "log.h"
#include "mVector.h"

glSystem* glSystem::p = 0;

glSystem::glSystem(jEngine* j)
{
	// constructor code for glSystem
	e = j;

    tex = glTextureManager::getPointer(j);

	sModes = 0;
	numModes = 0;

	resList = 0;
	numRes = 0;
	refList = 0;
	numRef = 0;

	hDC = 0;
	hRC = 0;

	extString = 0;
	renString = 0;
	venString = 0;
	verString = 0;

	consoleRender = 0;
	menuRender = 0;
	gameRender = 0;

	conr = nren;
	menr = nren;
	gamr = nren;

	// enumerate screenmodes and create a list
	int			i = 0;
	int			ret = 0;
	DEVMODE		dm;
	
	while(true)
	{
		memset(&dm,0,sizeof(DEVMODE));
		dm.dmSize = sizeof(DEVMODE);
		ret = EnumDisplaySettings(0,i,&dm);
		if(!ret) break;
		if((dm.dmBitsPerPel==32)&&(!dm.dmDisplayFlags))
		{
			numModes++;
			sModes = (glScreenMode**)realloc(sModes,sizeof(glScreenMode*)*numModes);
			sModes[numModes-1] = new glScreenMode(dm.dmPelsWidth, dm.dmPelsHeight, dm.dmBitsPerPel, dm.dmDisplayFrequency);
		}
		i++;
	}

	// build unique resolution list and refresh rate list for menus and ting
	for(i=0;i<numModes;++i)
	{
		int n=0;
		while(n<numRes)
		{
			if((sModes[i]->getWidth()==resList[n]->getWidth())&&(sModes[i]->getHeight()==resList[n]->getHeight())) break;
			n++;
		}
		if(n==numRes)
		{
			numRes++;
			resList = (glResolution**)realloc(resList,sizeof(glResolution*)*numRes);
			resList[numRes-1] = new glResolution(sModes[i]->getWidth(), sModes[i]->getHeight());
		}

		n=0;
		while(n<numRef)
		{
			if(sModes[i]->getRefreshRate()==refList[n]) break;
			n++;
		}
		if(n==numRef)
		{
			numRef++;
			refList = (int*)realloc(refList,sizeof(int)*numRef);
			refList[numRef-1] = sModes[i]->getRefreshRate();
		}
	}

	// sort lists
	qsort(resList, numRes, sizeof(glResolution*), compRes);
	qsort(refList, numRef, sizeof(int), compInt);

    defaultText = 0;
    f = 0;
#ifdef _DEBUG
	LogWriteLine(L"\x25CB\x25CB\x25CB glSystem constructor complete");
#endif
}

glSystem::glSystem(const glSystem& a)
{
	// copy constructor code for glSystem
}

glSystem& glSystem::operator=(const glSystem& a)
{
	// since we only want a single instance always return this 
	return (*this);
}

glSystem::~glSystem()
{
	// destructor code for glSystem
	// console variable destructor deletes these...
	//if(extString) delete[] extString;
	//if(renString) delete[] renString;
	//if(venString) delete[] venString;
	//if(verString) delete[] verString;

    if(tex) delete tex;

	for(int i=0;i<numModes;i++)
	{
		delete sModes[i];
	}

	if(sModes) free(sModes);

	for(int i=0;i<numRes;i++)
	{
		delete resList[i];
	}

	if(refList) free(refList);

    if(defaultText) delete defaultText;

    if(f) delete f;

#ifdef _DEBUG
	LogWriteLine(L"\x25CB\x25CB\x25CB glSystem destructor complete");
#endif
}

glSystem* glSystem::getPointer(jEngine* j)
{
	// return our instance of glSystem if its created 
	if(!p) p = new glSystem(j);
	return p;
}

int glSystem::compInt(const void* a, const void* b)
{
	int x = *((int*)a);
	int y = *((int*)b);

	if(x<y) return -1;
	if(x>y) return 1;
	return 0;
}

int glSystem::compRes(const void* a, const void* b)
{
	glResolution* x = *((glResolution**)a);
	glResolution* y = *((glResolution**)b);

	if(x->getWidth() < y->getWidth()) return -1;
	if(x->getWidth() > y->getWidth()) return 1;
	
	if(x->getHeight() < y->getHeight()) return -1;
	if(x->getHeight() > y->getHeight()) return 1;

	return 0;
}

bool glSystem::init(HWND hWnd, bool doubleBuffer)
{
	RECT r;
	GetClientRect(hWnd,&r);
	wWidth=r.right;
	wHeight=r.bottom;

	asp = (float)wWidth/(float)wHeight;

	// get dc if possible
	if(!(hDC = GetDC(hWnd))) return false;

	// pixel format id
	int i;

	int bufmode = doubleBuffer ? PFD_DOUBLEBUFFER : 0;
	swb = doubleBuffer ? sb : nsb;

	dblBuf = doubleBuffer;
	
	PIXELFORMATDESCRIPTOR pfd = 
		{
			sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | bufmode,
			PFD_TYPE_RGBA,
			32,							// 32-bit color
			0, 0, 0, 0, 0, 0,
			0, 0,
			0, 0, 0, 0, 0,
			24,							// 24-bit depth
			8,							 // 8-bit stencil
			0,
			PFD_MAIN_PLANE,
			0, 0, 0, 0
		};

	i = ChoosePixelFormat(hDC, &pfd);
	SetPixelFormat(hDC, i, &pfd);

	if(!(hRC = wglCreateContext(hDC))) return false;
	wglMakeCurrent(hDC, hRC);

	// clear screen and update if needed
	glViewport(0,0,wWidth,wHeight);
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClearDepth(1.0);
	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glShadeModel(GL_SMOOTH);
	glFlush();
	swb(hDC);

	// fill strings
	char* tmp = (char*)glGetString(GL_EXTENSIONS);
	int l = (int)strlen(tmp);
	if(extString) delete[] extString;
	extString = new wchar_t[l+1];
	for(int j=0;j<l;j++) extString[j] = (wchar_t)tmp[j];
	extString[l] = 0;

	tmp = (char*)glGetString(GL_RENDERER);
	l = (int)strlen(tmp);
	if(renString) delete[] renString;
	renString = new wchar_t[l+1];
	for(int j=0;j<l;j++) renString[j] = (wchar_t)tmp[j];
	renString[l] = 0;

	tmp = (char*)glGetString(GL_VENDOR);
	l = (int)strlen(tmp);
	if(venString) delete[] venString;
	venString = new wchar_t[l+1];
	for(int j=0;j<l;j++) venString[j] = (wchar_t)tmp[j];
	venString[l] = 0;

	tmp = (char*)glGetString(GL_VERSION);
	l = (int)strlen(tmp);
	if(verString) delete[] verString;
	verString = new wchar_t[l+1];
	for(int j=0;j<l;j++) verString[j] = (wchar_t)tmp[j];
	verString[l] = 0;

#ifdef _DEBUG
	LogWriteLine(L"\x25CB\x25CB\x25CF glSystem initialised succefully");
#endif

    f = new glFuncs();

    defaultText = new glTextRender(e, "default.ttf", 12);

	return true;
}

void glSystem::shutdown(HWND hWnd)
{
	if(hRC)
	{
		wglMakeCurrent(0,0);
		wglDeleteContext(hRC);
		hRC=0;
	}

	if(hDC)
	{
		ReleaseDC(hWnd, hDC);
		hDC=0;
	}
#ifdef _DEBUG
	LogWriteLine(L"\x25CB\x25CB\x25CF glSystem shutdown succefully");
#endif
}

int glSystem::getNearestResolution(int width, int height)
{
	int diff=2000;
	int idx=0;

	for(int i=0;i<numRes;++i)
	{
		int d = resList[i]->calcDifference(width, height);
		if(d==0) return i;
		if(d<diff)
		{
			diff=d;
			idx=i;
		}
	}

	return idx;
}

int glSystem::getNearestScreenMode(glResolution* res, int refreshRate)
{
	int diff=2000;
	int idx=0;

	for(int i=0;i<numModes;++i)
	{
		int d = sModes[i]->calcDifference(res->getWidth(), res->getHeight(), refreshRate);
		if(d==0) return i;
		if(d<diff)
		{
			diff=d;
			idx=i;
		}
	}

	return idx;
}

int glSystem::getNearestScreenMode(int width, int height, int refreshRate)
{
	int diff=2000;
	int idx=0;

	for(int i=0;i<numModes;++i)
	{
		int d = sModes[i]->calcDifference(width, height, refreshRate);
		if(d==0) return i;
		if(d<diff)
		{
			diff=d;
			idx=i;
		}
	}

	return idx;
}

glScreenMode* glSystem::getScreenMode(int idx)
{
	if(idx<numModes) return sModes[idx];
	return 0;
}

glResolution* glSystem::getResolution(int idx)
{
	if(idx<numRes) return resList[idx];
	return 0;
}

wchar_t* glSystem::getScreenModeDescription(int idx)
{
	if(idx<numModes) return sModes[idx]->getDescription();
	return 0;
}

wchar_t* glSystem::getResolutionDescription(int idx)
{
	if(idx<numRes) return resList[idx]->getDescription();
	return 0;
}

void glSystem::initGraphics()
{
	if(consoleRender) consoleRender->initGraphics();
	if(menuRender) menuRender->initGraphics();
	if(gameRender) gameRender->initGraphics();
}

void glSystem::shutdownGraphics()
{
	if(consoleRender) consoleRender->shutdownGraphics();
	if(menuRender) menuRender->shutdownGraphics();
	if(gameRender) gameRender->shutdownGraphics();
}

void glSystem::render()
{
    static bool& rShowFPS = *((bool*)(e->con->getVariable(L"rShowFPS")->getPointer()));

    f->ClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	f->Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    gamr(gameRender);
	menr(menuRender);
	conr(consoleRender);

    f->Ortho(0.0,asp,1.0,0.0,-2.0,2.0);

	f->Disable(GL_DEPTH_TEST);
    f->Disable(GL_TEXTURE_2D);

    f->Color(1.0f,1.0f,1.0f,1.0f);

    if(rShowFPS)
    {       
        defaultText->setPosition(asp - defaultText->getAdjustedWidth(L"fps "), 0.0f);
        defaultText->print(L"fps");

        defaultText->setPosition(asp - defaultText->getAdjustedWidth(L"9999fps "), 0.0f);
        defaultText->printf(L"%d", (int)(1.0f/getLastDT()));
    }

	swb(hDC);
}

void glSystem::ren(glRenderable* r)
{
	r->render();
}

void glSystem::nren(glRenderable* r)
{
}

void glSystem::sb(HDC dev)
{
	SwapBuffers(dev);
}

void glSystem::nsb(HDC dev)
{
	// p->f->Flush();
}

void glSystem::setConsole(glRenderable* c)
{ 
	if(c)
	{
		consoleRender = c;
		conr = ren;
	}
	else
	{
		consoleRender = 0;
		conr = nren;
	}
}

void glSystem::setMenu(glRenderable* m)
{
	if(m)
	{
		menuRender = m;
		menr = ren;
	}
	else
	{
		menuRender = 0;
		menr = nren;
	}
}

void glSystem::setGame(glRenderable* g)
{
	if(g)
	{
		gameRender = g;
		gamr = ren;
	}
	else
	{
		gameRender = 0;
		gamr = nren;
	}
}

float glSystem::getLastDT()
{
	return e->timer->getLastDT();
}

// from http://www.opengl.org/resources/features/OGLextensions/
bool HasExt(const char *extension)
{
  const GLubyte *extensions = NULL;
  const GLubyte *start;
  GLubyte *_where, *terminator;

  /* Extension names should not have spaces. */
  _where = (GLubyte *) strchr(extension, ' ');
  if (_where || *extension == '\0')
    return false;
  extensions = glGetString(GL_EXTENSIONS);
  /* It takes a bit of care to be fool-proof about parsing the
     OpenGL extensions string. Don't be fooled by sub-strings,
     etc. */
  start = extensions;
  for (;;) {
    _where = (GLubyte *) strstr((const char *) start, extension);
    if (!_where)
      break;
    terminator = _where + strlen(extension);
    if (_where == start || *(_where - 1) == ' ')
      if (*terminator == ' ' || *terminator == '\0')
        return true;
    start = terminator;
  }
  return false;
}


glSystem::glFuncs::glFuncs()
{
    // set up default functions
    pglActiveTexture = nActiveTexture;
    pglAttachShader = nAttachShader;
    pglBindAttribLocation = nBindAttribLocation;
    pglBindFramebuffer = nBindFramebuffer;
    pglBindRenderbuffer = nBindRenderbuffer;
    pglCheckFramebufferStatus = nCheckFramebufferStatus;
    pglClientActiveTexture = nClientActiveTexture;
    pglCompileShader = nCompileShader;
    pglConvolutionFilter1D = nConvolutionFilter1D;
    pglConvolutionFilter2D = nConvolutionFilter2D;
    pglConvolutionParameteri = nConvolutionParameteri;
    pglConvolutionParameteriv = nConvolutionParameteriv;
    pglConvolutionParameterf = nConvolutionParameterf;
    pglConvolutionParameterfv = nConvolutionParameterfv;
    pglCopyConvolutionFilter1D = nCopyConvolutionFilter1D;
    pglCopyConvolutionFilter2D = nCopyConvolutionFilter2D;
    pglCreateProgram = nCreateProgram;
    pglCreateShader = nCreateShader;
    pglDeleteFramebuffers = nDeleteFramebuffers;
    pglDeleteProgram = nDeleteProgram;
    pglDeleteRenderbuffers = nDeleteRenderbuffers;
    pglDeleteShader = nDeleteShader;
    pglDrawBuffers = nDrawBuffers;
    pglFramebufferRenderbuffer = nFramebufferRenderbuffer;
    pglFramebufferTexture1D = nFramebufferTexture1D;
    pglFramebufferTexture2D = nFramebufferTexture2D;
    pglFramebufferTexture3D = nFramebufferTexture3D;
    pglGenerateMipmap = nGenerateMipmap;
    pglGenFramebuffers = nGenFramebuffers;
    pglGenRenderbuffers = nGenRenderbuffers;
    pglGetAttribLocation = nGetAttribLocation;
    pglGetConvolutionFilter = nGetConvolutionFilter;
    pglGetConvolutionParameteriv = nGetConvolutionParameteriv;
    pglGetConvolutionParameterfv = nGetConvolutionParameterfv;
    pglGetFramebufferAttachmentParameteriv = nGetFramebufferAttachmentParameteriv;
    pglGetProgramInfoLog = nGetProgramInfoLog;
    pglGetRenderbufferParameteriv = nGetRenderbufferParameteriv;
    pglGetSeparableFilter = nGetSeparableFilter;
    pglGetShaderInfoLog = nGetShaderInfoLog;
    pglGetUniformLocation = nGetUniformLocation;
    pglIsFramebuffer = nIsFramebuffer;
    pglIsRenderbuffer = nIsRenderbuffer;
    pglLinkProgram = nLinkProgram;
    pglMultiTexCoord1f = nMultiTexCoord1f;
    pglMultiTexCoord2f = nMultiTexCoord2f;
    pglMultiTexCoord3f = nMultiTexCoord3f;
    pglMultiTexCoord4f = nMultiTexCoord4f;
    pglMultiTexCoord1i = nMultiTexCoord1i;
    pglMultiTexCoord2i = nMultiTexCoord2i;
    pglMultiTexCoord3i = nMultiTexCoord3i;
    pglMultiTexCoord4i = nMultiTexCoord4i;
    pglPointParameterf = nPointParameterf;
    pglPointParameterfv = nPointParameterfv;
    pglRenderbufferStorage = nRenderbufferStorage;
    pglSeparableFilter2D = nSeparableFilter2D;
    pglShaderSource = nShaderSource;
    pglUniform1f = nUniform1f;
    pglUniform2f = nUniform2f;
    pglUniform3f = nUniform3f;
    pglUniform4f = nUniform4f;
    pglUniform1i = nUniform1i;
    pglUniform2i = nUniform2i;
    pglUniform3i = nUniform3i;
    pglUniform4i = nUniform4i;
    pglUseProgram = nUseProgram;
    pglValidateProgram = nValidateProgram;
    pglVertexAttrib1f = nVertexAttrib1f;
    pglVertexAttrib2f = nVertexAttrib2f;
    pglVertexAttrib3f = nVertexAttrib3f;
    pglVertexAttrib4f = nVertexAttrib4f;
    pglVertexAttrib1i = nVertexAttrib1i;
    pglVertexAttrib2i = nVertexAttrib2i;
    pglVertexAttrib3i = nVertexAttrib3i;
    pglVertexAttrib4i = nVertexAttrib4i;

    setvsext();


    // get version
	char* str = _strdup((char*)glGetString(GL_VERSION));
	if(!str) return;

	int ver=0;
	// make the string into a nice number to compare stuff
	if(strlen(str)>3)
	{
		str[1]=0;str[3]=0;
		ver = atoi(&str[0])*100+atoi(&str[2])*10+atoi(&str[4]);
	}
    else
	{
		str[1]=0;
		ver = atoi(&str[0])*100+atoi(&str[2])*10;
	}

    Supports200 = Supports140 = Supports130 = Supports120 = false;
    SupportsFramebufferObjects = false;
    SupportsShaders = false;
    SupportsRectangleTextures = false;
    SupportsDepthTextures = false;
    SupportsMultitexture = false;
    SupportsPointParameters = false;
    SupportsMipmapGeneration = false;
    SupportsLODBias = false;

    if(SupportsFramebufferObjects = HasExt("GL_EXT_framebuffer_object")) setfboext();
    SupportsRectangleTextures = (HasExt("GL_ARB_texture_rectangle")||HasExt("GL_EXT_texture_rectangle")||HasExt("GL_NV_texture_rectangle"));

    if(ver>=200)
	{
		// multitexture related functions
        setmtbase();
		
		// shader related functions
		pglCreateShader = (unsigned int (__stdcall*)(unsigned int))wglGetProcAddress("glCreateShader");
		pglCreateProgram = (unsigned int (__stdcall*)(void))wglGetProcAddress("glCreateProgram");
		pglDeleteShader = (void (__stdcall*)(unsigned int))wglGetProcAddress("glDeleteShader");
		pglDeleteProgram = (void (__stdcall*)(unsigned int))wglGetProcAddress("glDeleteProgram");
		pglValidateProgram = (void (__stdcall*)(unsigned int))wglGetProcAddress("glValidateProgram");
		pglCompileShader = (void (__stdcall*)(unsigned int))wglGetProcAddress("glCompileShader");
		pglLinkProgram = (void (__stdcall*)(unsigned int))wglGetProcAddress("glLinkProgram");
		pglUseProgram = (void (__stdcall*)(unsigned int))wglGetProcAddress("glUseProgram");
		pglAttachShader = (void (__stdcall*)(unsigned int,unsigned int))wglGetProcAddress("glAttachShader");
		pglDetachShader = (void (__stdcall*)(unsigned int,unsigned int))wglGetProcAddress("glDetachShader");
		pglShaderSource = (void (__stdcall*)(unsigned int,int,char**,const int*))wglGetProcAddress("glShaderSource");
		pglGetProgramInfoLog = (void (__stdcall*)(unsigned int,int,int*,char*))wglGetProcAddress("glGetProgramInfoLog");
		pglGetShaderInfoLog = (void (__stdcall*)(unsigned int,int,int*,char*))wglGetProcAddress("glGetShaderInfoLog");

		pglGetUniformLocation = (int (__stdcall*)(unsigned int,const char*))wglGetProcAddress("glGetUniformLocation");
		pglGetAttribLocation = (int (__stdcall*)(unsigned int,const char*))wglGetProcAddress("glGetAttribLocation");
		pglBindAttribLocation = (void (__stdcall*)(unsigned int,unsigned int,const char*))wglGetProcAddress("glBindAttribLocation");
		
        pglVertexAttrib4f = (void (__stdcall*)(unsigned int,float,float,float,float))wglGetProcAddress("glVertexAttrib4f");
		pglVertexAttrib3f = (void (__stdcall*)(unsigned int,float,float,float))wglGetProcAddress("glVertexAttrib3f");
        pglVertexAttrib2f = (void (__stdcall*)(unsigned int,float,float))wglGetProcAddress("glVertexAttrib2f");
        pglVertexAttrib1f = (void (__stdcall*)(unsigned int,float))wglGetProcAddress("glVertexAttrib1f");

        pglVertexAttrib4i = (void (__stdcall*)(unsigned int,int,int,int,int))wglGetProcAddress("glVertexAttrib4i");
		pglVertexAttrib3i = (void (__stdcall*)(unsigned int,int,int,int))wglGetProcAddress("glVertexAttrib3i");
        pglVertexAttrib2i = (void (__stdcall*)(unsigned int,int,int))wglGetProcAddress("glVertexAttrib2i");
        pglVertexAttrib1i = (void (__stdcall*)(unsigned int,int))wglGetProcAddress("glVertexAttrib1i");
		
		pglUniform4f = (void (__stdcall*)(int,float,float,float,float))wglGetProcAddress("glUniform4f");
        pglUniform3f = (void (__stdcall*)(int,float,float,float))wglGetProcAddress("glUniform3f");
        pglUniform2f = (void (__stdcall*)(int,float,float))wglGetProcAddress("glUniform2f");
        pglUniform1f = (void (__stdcall*)(int,float))wglGetProcAddress("glUniform1f");

        pglUniform4i = (void (__stdcall*)(int,int,int,int,int))wglGetProcAddress("glUniform4i");
        pglUniform3i = (void (__stdcall*)(int,int,int,int))wglGetProcAddress("glUniform3i");
        pglUniform2i = (void (__stdcall*)(int,int,int))wglGetProcAddress("glUniform2i");
        pglUniform1i = (void (__stdcall*)(int,int))wglGetProcAddress("glUniform1i");

        pglDrawBuffers = (void (__stdcall*)(int,const unsigned int*))wglGetProcAddress("glDrawBuffers");

        set14base();

        set120ext();

        Supports200 = Supports140 = Supports130 = Supports120 = SupportsShaders = SupportsDepthTextures = SupportsPointParameters = SupportsMipmapGeneration = SupportsLODBias = SupportsMultitexture = true;
	}
    else if (ver>=140) // depth textures as full function in 1.4, no shaders guaranteed
	{
		setmtbase();
        set120ext();
        set14base();

		// check for GLSL shader extensions
		if(SupportsShaders = (HasExt("GL_ARB_draw_buffers")&&HasExt("GL_ARB_shading_language_100")&&HasExt("GL_ARB_vertex_shader")&&HasExt("GL_ARB_fragment_shader")&&HasExt("GL_ARB_shader_objects"))) setshadext();
        
        Supports140 = Supports130 = Supports120 = SupportsDepthTextures = SupportsPointParameters = SupportsMipmapGeneration = SupportsLODBias = SupportsMultitexture = true;
	}
    else if (ver>=130) // multitexture as full function in 1.3, no shaders guaranteed
	{
        setmtbase();
        set120ext();

        // check for 1.4 featured extensions
        if(Supports140 = SupportsDepthTextures = SupportsPointParameters = SupportsMipmapGeneration = SupportsLODBias = HasExt("GL_ARB_depth_texture")&&HasExt("GL_ARB_point_parameters")&&HasExt("GL_SGIS_generate_mipmap")&&HasExt("GL_EXT_texture_lod_bias")) set14exts();
        else
        {
            // check individually
            SupportsDepthTextures = HasExt("GL_ARB_depth_texture");
            SupportsMipmapGeneration = HasExt("GL_SGIS_generate_mipmap");
            SupportsLODBias = HasExt("GL_EXT_texture_lod_bias");
            if(SupportsPointParameters = HasExt("GL_ARB_point_parameters")) set14exts();
        }

		// check for GLSL shader extensions
		if(SupportsShaders = (HasExt("GL_ARB_draw_buffers")&&HasExt("GL_ARB_shading_language_100")&&HasExt("GL_ARB_vertex_shader")&&HasExt("GL_ARB_fragment_shader")&&HasExt("GL_ARB_shader_objects"))) setshadext();

        Supports130 = Supports120 = SupportsMultitexture = true;
	}
}

void glSystem::glFuncs::setvsext()
{
    pwglSwapInterval = (void (__stdcall*)(unsigned int))wglGetProcAddress("wglSwapIntervalEXT");
    if(!pwglSwapInterval) pwglSwapInterval = nSwapInterval;
}

// multitexture extensions
void glSystem::glFuncs::setmtbase()
{
    pglActiveTexture=(void (__stdcall*)(unsigned int))wglGetProcAddress("glActiveTexture");
    pglClientActiveTexture=(void (__stdcall*)(unsigned int))wglGetProcAddress("glClientActiveTexture");
    pglMultiTexCoord1f=(void (__stdcall*)(unsigned int,float))wglGetProcAddress("glMultiTexCoord1f");
    pglMultiTexCoord2f=(void (__stdcall*)(unsigned int,float,float))wglGetProcAddress("glMultiTexCoord2f");
    pglMultiTexCoord3f=(void (__stdcall*)(unsigned int,float,float,float))wglGetProcAddress("glMultiTexCoord3f");
    pglMultiTexCoord4f=(void (__stdcall*)(unsigned int,float,float,float,float))wglGetProcAddress("glMultiTexCoord4f");
    pglMultiTexCoord1i=(void (__stdcall*)(unsigned int,int))wglGetProcAddress("glMultiTexCoord1i");
    pglMultiTexCoord2i=(void (__stdcall*)(unsigned int,int,int))wglGetProcAddress("glMultiTexCoord2i");
    pglMultiTexCoord3i=(void (__stdcall*)(unsigned int,int,int,int))wglGetProcAddress("glMultiTexCoord3i");
    pglMultiTexCoord4i=(void (__stdcall*)(unsigned int,int,int,int,int))wglGetProcAddress("glMultiTexCoord4i");
}

void glSystem::glFuncs::setmtext()
{
    pglActiveTexture=(void (__stdcall*)(unsigned int))wglGetProcAddress("glActiveTextureARB");
    pglClientActiveTexture=(void (__stdcall*)(unsigned int))wglGetProcAddress("glClientActiveTextureARB");
    pglMultiTexCoord1f=(void (__stdcall*)(unsigned int,float))wglGetProcAddress("glMultiTexCoord1fARB");
    pglMultiTexCoord2f=(void (__stdcall*)(unsigned int,float,float))wglGetProcAddress("glMultiTexCoord2fARB");
    pglMultiTexCoord3f=(void (__stdcall*)(unsigned int,float,float,float))wglGetProcAddress("glMultiTexCoord3fARB");
    pglMultiTexCoord4f=(void (__stdcall*)(unsigned int,float,float,float,float))wglGetProcAddress("glMultiTexCoord4fARB");
    pglMultiTexCoord1i=(void (__stdcall*)(unsigned int,int))wglGetProcAddress("glMultiTexCoord1iARB");
    pglMultiTexCoord2i=(void (__stdcall*)(unsigned int,int,int))wglGetProcAddress("glMultiTexCoord2iARB");
    pglMultiTexCoord3i=(void (__stdcall*)(unsigned int,int,int,int))wglGetProcAddress("glMultiTexCoord3iARB");
    pglMultiTexCoord4i=(void (__stdcall*)(unsigned int,int,int,int,int))wglGetProcAddress("glMultiTexCoord4iARB");
}

// set up framebuffer objects from extensions
void glSystem::glFuncs::setfboext()
{
    pglIsRenderbuffer = (unsigned char(__stdcall*)(unsigned int))wglGetProcAddress("glIsRenderbufferEXT");
    pglBindRenderbuffer =(void(__stdcall*)(unsigned int,unsigned int))wglGetProcAddress("glBindRenderbufferEXT");
    pglDeleteRenderbuffers = (void(__stdcall*)(int ,const unsigned int*))wglGetProcAddress("glDeleteRenderbuffersEXT");
    pglGenRenderbuffers = (void(__stdcall*)(int, unsigned int*))wglGetProcAddress("glGenRenderbuffersEXT");
    pglRenderbufferStorage =(void(__stdcall*)(unsigned int,unsigned int,int,int))wglGetProcAddress("glRenderbufferStorageEXT");
    pglGetRenderbufferParameteriv = (void(__stdcall*)(unsigned int,unsigned int, int*))wglGetProcAddress("glGetRenderbufferParameterivEXT");
    
    pglIsFramebuffer = (unsigned char(__stdcall*)(unsigned int))wglGetProcAddress("glIsFramebufferEXT");
    pglBindFramebuffer = (void(__stdcall*)(unsigned int, unsigned int))wglGetProcAddress("glBindFramebufferEXT");
    pglDeleteFramebuffers = (void(__stdcall*)(int, const unsigned int*))wglGetProcAddress("glDeleteFramebuffersEXT");
    pglGenFramebuffers = (void(__stdcall*)(int, unsigned int*))wglGetProcAddress("glGenFramebuffersEXT");
    pglCheckFramebufferStatus = (unsigned int(__stdcall*)(unsigned int))wglGetProcAddress("glCheckFramebufferStatusEXT");
    pglFramebufferTexture1D = (void(__stdcall*)(unsigned int,unsigned int,unsigned int,unsigned int,int))wglGetProcAddress("glFramebufferTexture1DEXT");
    pglFramebufferTexture2D = (void(__stdcall*)(unsigned int,unsigned int,unsigned int,unsigned int,int))wglGetProcAddress("glFramebufferTexture2DEXT");
    pglFramebufferTexture3D = (void(__stdcall*)(unsigned int,unsigned int,unsigned int,unsigned int,int,int))wglGetProcAddress("glFramebufferTexture3DEXT");
    pglFramebufferRenderbuffer = (void(__stdcall*)(unsigned int,unsigned int,unsigned int,unsigned int))wglGetProcAddress("glFramebufferRenderbufferEXT");
    pglGetFramebufferAttachmentParameteriv = (void(__stdcall*)(unsigned int,unsigned int,unsigned int,int*))wglGetProcAddress("glGetFramebufferAttachmentParameterivEXT");
    pglGenerateMipmap = (void(__stdcall*)(unsigned int))wglGetProcAddress("glGenerateMipmapEXT");
}

// shader extensions
void glSystem::glFuncs::setshadext()
{
	pglCreateShader = (unsigned int (__stdcall*)(unsigned int))wglGetProcAddress("glCreateShaderObjectARB");
	pglCreateProgram = (unsigned int (__stdcall*)(void))wglGetProcAddress("glCreateProgramObjectARB");
	pglDeleteShader = (void (__stdcall*)(unsigned int))wglGetProcAddress("glDeleteObjectARB");
	pglDeleteProgram = (void (__stdcall*)(unsigned int))wglGetProcAddress("glDeleteObjectARB");
	pglValidateProgram = (void (__stdcall*)(unsigned int))wglGetProcAddress("glValidateProgramARB");
	pglCompileShader = (void (__stdcall*)(unsigned int))wglGetProcAddress("glCompileShaderARB");
	pglLinkProgram = (void (__stdcall*)(unsigned int))wglGetProcAddress("glLinkProgramARB");
	pglUseProgram = (void (__stdcall*)(unsigned int))wglGetProcAddress("glUseProgramObjectARB");
	pglAttachShader = (void (__stdcall*)(unsigned int,unsigned int))wglGetProcAddress("glAttachObjectARB");
	pglDetachShader = (void (__stdcall*)(unsigned int,unsigned int))wglGetProcAddress("glDetachObjectARB");
	pglShaderSource = (void (__stdcall*)(unsigned int,int,char**,const int*))wglGetProcAddress("glShaderSourceARB");
	pglGetProgramInfoLog = (void (__stdcall*)(unsigned int,int,int*,char*))wglGetProcAddress("glGetInfoLogARB");
	pglGetShaderInfoLog = (void (__stdcall*)(unsigned int,int,int*,char*))wglGetProcAddress("glGetInfoLogARB");

	pglGetUniformLocation = (int (__stdcall*)(unsigned int,const char*))wglGetProcAddress("glGetUniformLocationARB");
	pglGetAttribLocation = (int (__stdcall*)(unsigned int,const char*))wglGetProcAddress("glGetAttribLocationARB");
	pglBindAttribLocation = (void (__stdcall*)(unsigned int,unsigned int,const char*))wglGetProcAddress("glBindAttribLocationARB");
		
	pglVertexAttrib4f = (void (__stdcall*)(unsigned int,float,float,float,float))wglGetProcAddress("glVertexAttrib4fARB");
    pglVertexAttrib3f = (void (__stdcall*)(unsigned int,float,float,float))wglGetProcAddress("glVertexAttrib3fARB");
    pglVertexAttrib2f = (void (__stdcall*)(unsigned int,float,float))wglGetProcAddress("glVertexAttrib2fARB");
    pglVertexAttrib1f = (void (__stdcall*)(unsigned int,float))wglGetProcAddress("glVertexAttrib1fARB");
		
	pglVertexAttrib4i = (void (__stdcall*)(unsigned int,int,int,int,int))wglGetProcAddress("glVertexAttrib4iARB");
	pglVertexAttrib3i = (void (__stdcall*)(unsigned int,int,int,int))wglGetProcAddress("glVertexAttrib3iARB");
    pglVertexAttrib2i = (void (__stdcall*)(unsigned int,int,int))wglGetProcAddress("glVertexAttrib2iARB");
    pglVertexAttrib1i = (void (__stdcall*)(unsigned int,int))wglGetProcAddress("glVertexAttrib1iARB");
	
	pglUniform4f = (void (__stdcall*)(int,float,float,float,float))wglGetProcAddress("glUniform4fARB");
    pglUniform3f = (void (__stdcall*)(int,float,float,float))wglGetProcAddress("glUniform3fARB");
    pglUniform2f = (void (__stdcall*)(int,float,float))wglGetProcAddress("glUniform2fARB");
    pglUniform1f = (void (__stdcall*)(int,float))wglGetProcAddress("glUniform1fARB");

    pglUniform4i = (void (__stdcall*)(int,int,int,int,int))wglGetProcAddress("glUniform4iARB");
    pglUniform3i = (void (__stdcall*)(int,int,int,int))wglGetProcAddress("glUniform3iARB");
    pglUniform2i = (void (__stdcall*)(int,int,int))wglGetProcAddress("glUniform2iARB");
    pglUniform1i = (void (__stdcall*)(int,int))wglGetProcAddress("glUniform1iARB");

    pglDrawBuffers = (void (__stdcall*)(int,const unsigned int*))wglGetProcAddress("glDrawBuffersARB");
}
void glSystem::glFuncs::set14exts()
{
    pglPointParameterf = (void(__stdcall*)(unsigned int, float))wglGetProcAddress("glPointParameterfARB");
    pglPointParameterfv = (void(__stdcall*)(unsigned int, const float*))wglGetProcAddress("glPointParameterfvARB");
}

void glSystem::glFuncs::set14base()
{
    pglPointParameterf = (void(__stdcall*)(unsigned int, float))wglGetProcAddress("glPointParameterf");
    pglPointParameterfv = (void(__stdcall*)(unsigned int, const float*))wglGetProcAddress("glPointParameterfv");
}

// set up extensions we get guaranteed by 1.20 that arent in the ms opengl header... (annoying)
void glSystem::glFuncs::set120ext()
{
    pglConvolutionFilter1D = (void(__stdcall*)(unsigned int,unsigned int,int,unsigned int,unsigned int,const void*))wglGetProcAddress("glConvolutionFilter1D");
    pglConvolutionFilter2D = (void(__stdcall*)(unsigned int,unsigned int,int,int,unsigned int,unsigned int,const void*))wglGetProcAddress("glConvolutionFilter2D");
    pglCopyConvolutionFilter1D = (void(__stdcall*)(unsigned int,unsigned int,int,int,int))wglGetProcAddress("glCopyConvolutionFilter1D");
    pglCopyConvolutionFilter2D = (void(__stdcall*)(unsigned int,unsigned int,int,int,int,int))wglGetProcAddress("glCopyConvolutionFilter2D");
    pglGetConvolutionFilter = (void(__stdcall*)(unsigned int,unsigned int,unsigned int,void*))wglGetProcAddress("glGetConvolutionFilter");
    pglSeparableFilter2D = (void(__stdcall*)(unsigned int,unsigned int,int,int,unsigned int,unsigned int,const void*,const void*))wglGetProcAddress("glSeparableFilter2D");
    pglGetSeparableFilter = (void(__stdcall*)(unsigned int,unsigned int,unsigned int,void*,void*,void*))wglGetProcAddress("glGetSeperableFilter");
    pglConvolutionParameteri = (void(__stdcall*)(unsigned int,unsigned int pname,int param))wglGetProcAddress("glConvolutionParameteri");
    pglConvolutionParameteriv = (void(__stdcall*)(unsigned int,unsigned int,const int*))wglGetProcAddress("glConvolutionParameteriv");
    pglConvolutionParameterf = (void(__stdcall*)(unsigned int,unsigned int,float))wglGetProcAddress("glConvolutionParameterf");
    pglConvolutionParameterfv = (void(__stdcall*)(unsigned int,unsigned int,const float*))wglGetProcAddress("glConvolutionParameterfv");
    pglGetConvolutionParameteriv = (void(__stdcall*)(unsigned int,unsigned int,int*))wglGetProcAddress("glGetConvolutionParameteriv");
    pglGetConvolutionParameterfv = (void(__stdcall*)(unsigned int,unsigned int,float*))wglGetProcAddress("glGetConvolutionParameterfv");
}

// vector normal, color, vertex etc...
void glSystem::glFuncs::Color(mVector3d& v) const
{
    Color(v.Get(0),v.Get(1),v.Get(2));
}

void glSystem::glFuncs::Normal(mVector3d& v) const
{
    Normal(v.Get(0),v.Get(1),v.Get(2));
}

void glSystem::glFuncs::Vertex(mVector3d& v) const
{
    Vertex(v.Get(0),v.Get(1),v.Get(2));
}

// draw buffers

void glSystem::glFuncs::DrawBuffers(int n, const unsigned int *bufs)
{
    pglDrawBuffers(n,bufs);
}

// convolution

void glSystem::glFuncs::ConvolutionFilter1D(unsigned int target,unsigned int internalformat,int width,unsigned int format,unsigned int type,const void* image)
{
    pglConvolutionFilter1D(target,internalformat,width,format,type,image);
}

void glSystem::glFuncs::ConvolutionFilter2D(unsigned int target,unsigned int internalformat,int width,int height,unsigned int format,unsigned int type,const void* image)
{
    pglConvolutionFilter2D(target,internalformat,width,height,format,type,image);
}

void glSystem::glFuncs::CopyConvolutionFilter1D(unsigned int target,unsigned int internalformat,int x,int y,int width)
{
    pglCopyConvolutionFilter1D(target,internalformat,x,y,width);
}

void glSystem::glFuncs::CopyConvolutionFilter2D(unsigned int target,unsigned int internalformat,int x,int y,int width,int height)
{
    pglCopyConvolutionFilter2D(target,internalformat,x,y,width,height);
}

void glSystem::glFuncs::GetConvolutionFilter(unsigned int target,unsigned int format,unsigned int type,void* image)
{
    pglGetConvolutionFilter(target,format,type,image);
}

void glSystem::glFuncs::SeparableFilter2D(unsigned int target,unsigned int internalformat,int width,int height,unsigned int format,unsigned int type,const void* row,const void* column)
{
    pglSeparableFilter2D(target,internalformat,width,height,format,type,row,column);
}
void glSystem::glFuncs::GetSeparableFilter(unsigned int target,unsigned int format,unsigned int type,void* row,void* column,void* span)
{
    pglGetSeparableFilter(target,format,type,row,column,span);
}

void glSystem::glFuncs::ConvolutionParameter(unsigned int target,unsigned int pname,int param)
{
    pglConvolutionParameteri(target,pname,param);
}

void glSystem::glFuncs::ConvolutionParameter(unsigned int target,unsigned int pname,const int* params)
{
    pglConvolutionParameteriv(target,pname,params);
}

void glSystem::glFuncs::ConvolutionParameter(unsigned int target,unsigned int pname,float param)
{
    pglConvolutionParameterf(target,pname,param);
}

void glSystem::glFuncs::ConvolutionParameter(unsigned int target,unsigned int pname,const float* params)
{
    pglConvolutionParameterfv(target,pname,params);
}

void glSystem::glFuncs::GetConvolutionParameter(unsigned int target,unsigned int pname,int* params)
{
    pglGetConvolutionParameteriv(target,pname,params);
}

void glSystem::glFuncs::GetConvolutionParameter(unsigned int target,unsigned int pname,float* params)
{
    pglGetConvolutionParameterfv(target,pname,params);
}

// multitexture

void glSystem::glFuncs::ActiveTexture(unsigned int t)
{
	pglActiveTexture(t);
}

void glSystem::glFuncs::ClientActiveTexture(unsigned int t)
{
	pglClientActiveTexture(t);
}

void glSystem::glFuncs::MultiTexCoord(unsigned int texunit, float s)
{
	pglMultiTexCoord1f(texunit,s);
}

void glSystem::glFuncs::MultiTexCoord(unsigned int texunit, float s, float t)
{
	pglMultiTexCoord2f(texunit,s,t);
}

void glSystem::glFuncs::MultiTexCoord(unsigned int texunit, float s, float t, float u)
{
	pglMultiTexCoord3f(texunit,s,t,u);
}

void glSystem::glFuncs::MultiTexCoord(unsigned int texunit, float s, float t, float u, float v)
{
	pglMultiTexCoord4f(texunit,s,t,u,v);
}

void glSystem::glFuncs::MultiTexCoord(unsigned int texunit, int s)
{
	pglMultiTexCoord1i(texunit,s);
}

void glSystem::glFuncs::MultiTexCoord(unsigned int texunit, int s, int t)
{
	pglMultiTexCoord2i(texunit,s,t);
}

void glSystem::glFuncs::MultiTexCoord(unsigned int texunit, int s, int t, int u)
{
	pglMultiTexCoord3i(texunit,s,t,u);
}

void glSystem::glFuncs::MultiTexCoord(unsigned int texunit, int s, int t, int u, int v)
{
	pglMultiTexCoord4i(texunit,s,t,u,v);
}

// point parameters

void glSystem::glFuncs::PointParameter(unsigned int pname, float param)
{
    pglPointParameterf(pname, param);
}

void glSystem::glFuncs::PointParameter(unsigned int pname, const float* params)
{
    pglPointParameterfv(pname, params);
}

// fbo

unsigned char glSystem::glFuncs::IsRenderbuffer(unsigned int renderbuffer)
{
    return pglIsRenderbuffer(renderbuffer);
}

void glSystem::glFuncs::BindRenderbuffer(unsigned int target, unsigned int renderbuffer)
{
    pglBindRenderbuffer(target,renderbuffer);
}

void glSystem::glFuncs::DeleteRenderbuffers(int n, const unsigned int *renderbuffers)
{
    pglDeleteRenderbuffers(n, renderbuffers);
}

void glSystem::glFuncs::GenRenderbuffers(int n, unsigned int *renderbuffers)
{
    pglGenRenderbuffers(n,renderbuffers);
}

void glSystem::glFuncs::RenderbufferStorage(unsigned int target, unsigned int internalformat,int width, int height)
{
    pglRenderbufferStorage(target, internalformat, width, height);
}

void glSystem::glFuncs::GetRenderbufferParameteriv(unsigned int target, unsigned int pname, int *params)
{
    pglGetRenderbufferParameteriv(target, pname, params);
}

unsigned char glSystem::glFuncs::IsFramebuffer(unsigned int framebuffer)
{
    return pglIsFramebuffer(framebuffer);
}

void glSystem::glFuncs::BindFramebuffer(unsigned int target, unsigned int framebuffer)
{
    pglBindFramebuffer(target, framebuffer);
}

void glSystem::glFuncs::DeleteFramebuffers(int n, const unsigned int *framebuffers)
{
    pglDeleteFramebuffers(n,framebuffers);
}

void glSystem::glFuncs::GenFramebuffers(int n, unsigned int *framebuffers)
{
    pglGenFramebuffers(n,framebuffers);
}

unsigned int glSystem::glFuncs::CheckFramebufferStatus(unsigned int target)
{
    return pglCheckFramebufferStatus(target);
}

void glSystem::glFuncs::FramebufferTexture1D(unsigned int target, unsigned int attachment,unsigned int textarget, unsigned int texture,int level)
{
    pglFramebufferTexture1D(target,attachment,textarget,texture,level);
}
void glSystem::glFuncs::FramebufferTexture2D(unsigned int target, unsigned int attachment,unsigned int textarget, unsigned int texture,int level)
{
    pglFramebufferTexture2D(target,attachment,textarget,texture,level);
}

void glSystem::glFuncs::FramebufferTexture3D(unsigned int target, unsigned int attachment,unsigned int textarget, unsigned int texture,int level, int zoffset)
{
    pglFramebufferTexture3D(target,attachment,textarget,texture,level,zoffset);
}

void glSystem::glFuncs::FramebufferRenderbuffer(unsigned int target, unsigned int attachment, unsigned int renderbuffertarget, unsigned int renderbuffer)
{
    pglFramebufferRenderbuffer(target,attachment, renderbuffertarget, renderbuffer);
}

void glSystem::glFuncs::GetFramebufferAttachmentParameteriv(unsigned int target, unsigned int attachment, unsigned int pname, int *params)
{
    pglGetFramebufferAttachmentParameteriv(target,attachment,pname,params);
}

void glSystem::glFuncs::GenerateMipmap(unsigned int target)
{
    pglGenerateMipmap(target);
}

// shader

unsigned int glSystem::glFuncs::CreateShader(unsigned int type)
{
	return pglCreateShader(type);
}

unsigned int glSystem::glFuncs::CreateProgram()
{
	return pglCreateProgram();
}

void glSystem::glFuncs::DeleteShader(unsigned int shader)
{
	pglDeleteShader(shader);
}

void glSystem::glFuncs::DeleteProgram(unsigned int program)
{
	pglDeleteProgram(program);
}

void glSystem::glFuncs::CompileShader(unsigned int shader)
{
	pglCompileShader(shader);
}

void glSystem::glFuncs::ValidateProgram(unsigned int program)
{
	pglValidateProgram(program);
}

void glSystem::glFuncs::LinkProgram(unsigned int program)
{
	pglLinkProgram(program);
}

void glSystem::glFuncs::UseProgram(unsigned int program)
{
	pglUseProgram(program);
}

void glSystem::glFuncs::AttachShader(unsigned int program, unsigned int shader)
{
	pglAttachShader(program,shader);
}

void glSystem::glFuncs::ShaderSource(unsigned int shader, int nstrings, char** strings, const int *lengths)
{
	pglShaderSource(shader, nstrings, strings, lengths);
}

int glSystem::glFuncs::GetUniformLocation(unsigned int program, const char *name)
{
	return pglGetUniformLocation(program, name);
}

int glSystem::glFuncs::GetAttribLocation(unsigned int program, const char *name)
{
	return pglGetAttribLocation(program, name);
}

void glSystem::glFuncs::BindAttribLocation(unsigned int program, unsigned int index, const char *name)
{
	pglBindAttribLocation(program, index, name);
}

void glSystem::glFuncs::GetProgramInfoLog(unsigned int program, int maxlen, int* length, char* infolog)
{
	pglGetProgramInfoLog(program, maxlen, length, infolog);
}

void glSystem::glFuncs::GetShaderInfoLog(unsigned int shader, int maxlen, int* length, char* infolog)
{
	pglGetShaderInfoLog(shader, maxlen, length, infolog);
}

// vertex attribs
void glSystem::glFuncs::VertexAttrib(unsigned int index, int x, int y, int z, int w)
{
	pglVertexAttrib4i(index, x, y, z, w);
}

void glSystem::glFuncs::VertexAttrib(unsigned int index, int x, int y, int z)
{
	pglVertexAttrib3i(index, x, y, z);
}

void glSystem::glFuncs::VertexAttrib(unsigned int index, int x, int y)
{
	pglVertexAttrib2i(index, x, y);
}

void glSystem::glFuncs::VertexAttrib(unsigned int index, int x)
{
	pglVertexAttrib1i(index, x);
}

void glSystem::glFuncs::VertexAttrib(unsigned int index, float x, float y, float z, float w)
{
	pglVertexAttrib4f(index, x, y, z, w);
}

void glSystem::glFuncs::VertexAttrib(unsigned int index, float x, float y, float z)
{
	pglVertexAttrib3f(index, x, y, z);
}

void glSystem::glFuncs::VertexAttrib(unsigned int index, float x, float y)
{
	pglVertexAttrib2f(index, x, y);
}

void glSystem::glFuncs::VertexAttrib(unsigned int index, float x)
{
	pglVertexAttrib1f(index, x);
}

// uniforms

void glSystem::glFuncs::Uniform(int location, int v0, int v1, int v2, int v3)
{
	pglUniform4i(location, v0, v1, v2, v3);
}

void glSystem::glFuncs::Uniform(int location, int v0, int v1, int v2)
{
	pglUniform3i(location, v0, v1, v2);
}

void glSystem::glFuncs::Uniform(int location, int v0, int v1)
{
	pglUniform2i(location, v0, v1);
}

void glSystem::glFuncs::Uniform(int location, int v0)
{
	pglUniform1i(location, v0);
}

void glSystem::glFuncs::Uniform(int location, float v0, float v1, float v2, float v3)
{
	pglUniform4f(location, v0, v1, v2, v3);
}

void glSystem::glFuncs::Uniform(int location, float v0, float v1, float v2)
{
	pglUniform3f(location, v0, v1, v2);
}

void glSystem::glFuncs::Uniform(int location, float v0, float v1)
{
	pglUniform2f(location, v0, v1);
}

void glSystem::glFuncs::Uniform(int location, float v0)
{
	pglUniform1f(location, v0);
}