/* inMouse class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#include "inMouse.h"

#include "inMouseReciever.h"

#include "glSystem.h"
#include "jEngine.h"
#include "log.h"

#include <windows.h>

#define VK_XBUTTON1 0x05
#define VK_XBUTTON2 0x06

inMouse* inMouse::p = 0;

inMouse::inMouse()
{
	// constructor code for inMouse
	xPos = 0.0f;
	yPos = 0.0f;
	zPos = 0.0f;

	b1 = false;
	b2 = false;
	b3 = false;
	b4 = false;
	b5 = false;

	mouseReciever = 0;
#ifdef _DEBUG
	LogWriteLine(L"\x25CB\x25CB\x25CB inMouse constructor complete");
#endif
}

inMouse::inMouse(const inMouse& a)
{
	// copy constructor code for inMouse
}

inMouse& inMouse::operator=(const inMouse& a)
{
	// since we only want a single instance always return this 
	return (*this);
}

inMouse::~inMouse()
{
	// destructor code for inMouse
#ifdef _DEBUG
	LogWriteLine(L"\x25CB\x25CB\x25CB inMouse destructor complete");
#endif
}

inMouse* inMouse::getPointer()
{
	// return our instance of inMouse if its created 
	if(!p) p = new inMouse();
	return p;
}

inMouseReciever* inMouse::getMouseReciever()
{
	return mouseReciever;
}

void inMouse::setMouseReciever(inMouseReciever* mr)
{
	mouseReciever = mr;
}

void inMouse::inputCycle()
{
	float oldx = xPos;
	float oldy = yPos;

	bool oldb1 = b1;
	bool oldb2 = b2;
	bool oldb3 = b3;
	bool oldb4 = b4;
	bool oldb5 = b5;

	jEngine* e = jEngine::getPointer();
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(e->getHandle(), &p);
	
	xPos = (float)p.x / (float)e->gl->getHeight();
	yPos = (float)p.y / (float)e->gl->getHeight();
	
	b1 = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) ? true : false;
	b2 = (GetAsyncKeyState(VK_RBUTTON) & 0x8000) ? true : false;
	b3 = (GetAsyncKeyState(VK_MBUTTON) & 0x8000) ? true : false;
	b4 = (GetAsyncKeyState(VK_XBUTTON1) & 0x8000) ? true : false;
	b5 = (GetAsyncKeyState(VK_XBUTTON2) & 0x8000) ? true : false;

	if(mouseReciever)
	{
		mouseReciever->onMouseMovement(xPos,yPos,xPos-oldx,yPos-oldy);
		if(b1 && !oldb1) mouseReciever->onMouseButtonDown(1, xPos, yPos);
		if(b2 && !oldb2) mouseReciever->onMouseButtonDown(2, xPos, yPos);
		if(b3 && !oldb3) mouseReciever->onMouseButtonDown(3, xPos, yPos);
		if(b4 && !oldb4) mouseReciever->onMouseButtonDown(4, xPos, yPos);
		if(b5 && !oldb5) mouseReciever->onMouseButtonDown(5, xPos, yPos);

		if(!b1 && oldb1) mouseReciever->onMouseButtonUp(1, xPos, yPos);
		if(!b2 && oldb2) mouseReciever->onMouseButtonUp(2, xPos, yPos);
		if(!b3 && oldb3) mouseReciever->onMouseButtonUp(3, xPos, yPos);
		if(!b4 && oldb4) mouseReciever->onMouseButtonUp(4, xPos, yPos);
		if(!b5 && oldb5) mouseReciever->onMouseButtonUp(5, xPos, yPos);
	}

}