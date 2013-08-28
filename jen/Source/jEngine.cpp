/* jEngine class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#define WM_MOUSEWHEEL 0x020A

#include "jEngine.h"

#include "console.h"
#include "alSystem.h"
#include "gameSystem.h"
#include "glSystem.h"
#include "inSystem.h"
#include "menuSystem.h"
#include "physicsSystem.h"
#include "strSystem.h"

#include "cVar.h"
#include "glScreenMode.h"
#include "glTextRender.h"
#include "glTextureManager.h"
#include "jMath.h"
#include "jProfiler.h"
#include "log.h"
#include "resource.h"

#include "sceneAirHockey.h"
#include "sceneBlack.h"
#include "sceneDodecahedron.h"
//#include "sceneTest1.h"
#include "sceneTest2.h"
#include "sceneTerrain.h"

#include <malloc.h>
#include <wchar.h>
#include <windows.h>

#include <jShared.h>

jEngine* jEngine::p = 0;
bool jEngine::constructed = false;

jEngine::jEngine()
{
	// constructor code for jEngine
	str = strSystem::getPointer(this);
	con = console::getPointer(this);
	al = alSystem::getPointer(this);
	game = gameSystem::getPointer(this);
	gl = glSystem::getPointer(this);
	in = inSystem::getPointer(this);
    phys = physicsSystem::getPointer(this);
	menu = menuSystem::getPointer(this);

	basePath = 0;

	handle = 0;
	inst = 0;

	timer = new jTimer();

	fullscreen = false;

    constructed = true;
#ifdef _DEBUG
	LogWriteLine(L"\x25CB\x25CB\x25CB jEngine constructor complete");
#endif
}

jEngine::jEngine(const jEngine& a)
{
	// copy constructor code for jEngine
}

jEngine& jEngine::operator=(const jEngine& a)
{
	// since we only want a single instance always return this 
	return (*this);
}

jEngine::~jEngine()
{
	// clean up path string, window etc
	if(basePath) free(basePath);

	if(handle) 
	{
		DestroyWindow(handle);

		ChangeDisplaySettings(0,0);
		ShowCursor(true);
	}

	// clean up systems
	delete con;
	delete al;
	delete game;
	delete gl;
	delete in;
	delete menu;
    delete phys;
	delete str;

	delete timer;

	// unregister class
	UnregisterClass(L"jEngine Window", inst);

#ifdef _DEBUG
	LogWriteLine(L"\x25CB\x25CB\x25CB jEngine destructor complete");
#endif
}

jEngine* jEngine::getPointer()
{
	// return our instance of jEngine if its created 
	if(!p) p = new jEngine();
	return p;
}

int jEngine::Run(HINSTANCE hInstance, const wchar_t* cmdLine)
{
#ifdef _DEBUG
	jProfiler profiler = jProfiler(L"jEngine::Run");
#endif
	inst = hInstance;

	jMathInit();

#ifdef _RDEBUG
	MessageBox(0, L"jEngine::Run 118", L"jEngine.cpp", MB_OK);
#endif

	// register cvars since we need some to init graphics and sound
	con->registerCvars();
	if(!con->execConfig(L"autoexec.cfg")) 
		con->saveConfig(L"autoexec.cfg");

	// get config and load, store basePath
	basePath = _wgetcwd(basePath, 1024);

	// initialise stuff
	glTextRender::initFreeType();
	
	// register window class
	memset(&wc, 0, sizeof(wc));
	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc		= WinProc;
	wc.hInstance		= inst;
	wc.hIcon			= LoadIcon(inst, MAKEINTRESOURCE(IDI_GAMEICON));
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hIconSm			= LoadIcon(inst, MAKEINTRESOURCE(IDI_GAMEICON));
	wc.lpszClassName	= L"jEngine Window";

	if(!RegisterClassEx(&wc)) return -1;

	handle = createWindow(L"jEngine");

	bool dblbuf	= *((bool*)con->getVariable(L"rDoubleBuffer")->getPointer());

	if(!gl->init(handle, dblbuf))
	{
		glTextRender::shutdownFreeType();

		if(handle) 
		{
			DestroyWindow(handle);

			ChangeDisplaySettings(0,0);
			ShowCursor(true);
		}

		handle = 0;

		wchar_t* msgStr = str->getString(L"The graphics capabilities of this system do not meet the minimum requirements");
		wchar_t* titStr = str->getString(L"Error");
		MessageBox(0, msgStr, titStr, MB_OK);
		return -2;
	}

	// register ccoms
	con->registerCcoms();

	con->clear();

	// build menu file list
	menu->buildMenuFileList();

	// start window loop
	MSG msg;

	// TODO:init render stuff
	gl->setConsole(con);
	gl->setMenu(menu);
	gl->setGame(game);

    // TODO:add game scenes
    game->addScene(new sceneBlack());
    game->addScene(new sceneAirHockey());
    //game->addScene(new sceneTest1());
    game->addScene(new sceneTest2());
    game->addScene(new sceneDodecahedron());
    game->addScene(new sceneTerrain());

    // build texture list
    gl->tex->buildTextureList();

    // init innit
	gl->initGraphics();

	con->execConfig(L"gamestart.cfg");

	while(true)											
	{													
		if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{ 
			if(msg.message == WM_QUIT) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		in->inputCycle();
		timer->getDT();
		gl->render();
	}

	gl->shutdownGraphics();

	// save config and quit
	con->saveConfig(L"autoexec.cfg");

	gl->shutdown(handle);
	
	glTextRender::shutdownFreeType();

	return 0;
}

bool jEngine::goFullscreen(int w, int h, int refresh)
{
#ifdef _DEBUG
	jProfiler profiler = jProfiler(L"jEngine::goFullscreen");
#endif

	// find nearest screenmode
	glScreenMode* sm = gl->getScreenMode(gl->getNearestScreenMode(w,h,refresh));

	// change display settings
	DEVMODE dm;
	memset(&dm,0,sizeof(dm));

	if(!EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm)) return false;

	dm.dmPelsWidth			= sm->getWidth();
	dm.dmPelsHeight			= sm->getHeight();
	dm.dmBitsPerPel			= 32;
	dm.dmDisplayFrequency	= sm->getRefreshRate();
	dm.dmFields				= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFREQUENCY;

	if(ChangeDisplaySettings(&dm, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) return false;

	return true;
}

HWND jEngine::createWindow(const wchar_t* title)
{
#ifdef _DEBUG
	jProfiler profiler = jProfiler(L"jEngine::createWindow");
#endif
	fullscreen	= *((bool*)con->getVariable(L"rFullscreen")->getPointer());
	int*	width		= ((int*)con->getVariable(L"rWidth")->getPointer());
	int*	height		= ((int*)con->getVariable(L"rHeight")->getPointer());
	int		bpp			= 32;
	int*	refresh		= ((int*)con->getVariable(L"rRefresh")->getPointer());

	// set style up depending on if its fullscreen or not
	DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
#ifdef _DEBUG
	DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
#else
	DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE | WS_EX_TOPMOST;
#endif
	
	if(fullscreen)
	{
		dwStyle = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
		dwExStyle = WS_EX_APPWINDOW | WS_EX_TOPMOST;
		// try to go fullscreen, if we can't, revert the window style
		if(!goFullscreen(*width,*height,*refresh))
		{
			fullscreen = false;
			*((bool*)con->getVariable(L"rFullscreen")->getPointer()) = false;
			dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
			dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE | WS_EX_TOPMOST;
		}
	}
	

	// set up window rect
	RECT r;
	r.left  = 0;
	r.right = *width;
	r.top = 0;
	r.bottom = *height;
	AdjustWindowRect(&r, dwStyle, false);

	// create the window
	HWND hWnd = CreateWindowEx( dwExStyle,
								L"jEngine Window",
								title,
								dwStyle,
								0, 0,
								r.right - r.left,
								r.bottom - r.top,
								NULL,
								NULL,
								inst,
								NULL);

	if(!hWnd) return 0;

	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);
	SetFocus(hWnd);
	// hide mouse if we are in fullscreen
	ShowCursor(!(fullscreen));
	menu->setMouseRenderState(fullscreen);
	
#ifdef _DEBUG
	LogWriteLine(L"\x25CB\x25CB\x25CF Window creation complete");
#endif
	return hWnd;
}

LRESULT CALLBACK jEngine::WinProc(HWND hWnd, unsigned int msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{ 
		case WM_INPUTLANGCHANGE:
			p->in->changeKeyboardLayout();
			break;
		case WM_SETFOCUS:
			jsCatchKeys(true);
			break;
		case WM_KILLFOCUS:
			jsCatchKeys(false);
			break;
        case WM_MOUSEWHEEL:
            if(p->in->getMouseReciever()) p->in->getMouseReciever()->onMouseWheel(-((int)wParam >> 16)/120);
            break;
		case WM_KEYDOWN:
			p->in->wmKeyHandler((unsigned int)wParam, (long)lParam);
			break;
		case WM_CHAR:
			p->in->wmCharHandler((unsigned int)wParam, (long)lParam);
			break;
		case WM_CLOSE:
			PostQuitMessage(0);								
			break; 
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
	} 
	return 0;
}

void jEngine::rRestart()
{
#ifdef _DEBUG
	jProfiler profiler = jProfiler(L"jEngine::rRestart");
#endif
	// shutdown
	gl->shutdownGraphics();
	gl->shutdown(handle);

	DestroyWindow(handle);

	bool fs = *((bool*)con->getVariable(L"rFullscreen")->getPointer());

	if(fullscreen && !fs)
	{
		ChangeDisplaySettings(0,0);
		ShowCursor(true);
	} else if(!fullscreen && fs)
	{
		ShowCursor(false);
	}

	// restart
	handle = createWindow(L"jEngine Window");

	bool dblbuf	= *((bool*)con->getVariable(L"rDoubleBuffer")->getPointer());

	gl->init(handle, dblbuf);

	gl->initGraphics();
}