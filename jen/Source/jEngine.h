/* jEngine class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#ifndef __JENGINE_H
#define __JENGINE_H

class alSystem;
class console;
class gameSystem;
class glSystem;
class inSystem;
class menuSystem;
class physicsSystem;
class strSystem;

#include "jTimer.h"

#include <windows.h>

class jEngine
{
public:
	console*	    con;

	alSystem*	    al;
	gameSystem*	    game;
	glSystem*	    gl;
	inSystem*	    in;
	menuSystem*	    menu;
    physicsSystem*  phys;
	strSystem*	    str;

	jTimer*		    timer;
    
    static bool     constructed;
private:
	// pointer to single instance
	static jEngine* p;

	wchar_t*	basePath;
	HWND		handle;
	HINSTANCE	inst;
	WNDCLASSEX	wc;

	bool fullscreen;

	// make constructors and assignment private to enforce single instance
	jEngine();
	jEngine(const jEngine& a);
	jEngine& operator=(const jEngine& a);

	// window helper functions
	HWND createWindow(const wchar_t* title);
	bool goFullscreen(int w, int h, int refresh);
	
	static LRESULT CALLBACK WinProc(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam);
public:
	~jEngine();

	// return pointer to our single instance, or create it
	static jEngine* getPointer();

	// entry point
	int Run(HINSTANCE hInstance, const wchar_t* cmdLine);

	HWND getHandle() { return handle; }

	void rRestart();
};

#endif