/* menuItem class by Semi Essessi
 *
 * Base menu item
 *
 */

#include "menuItem.h"

#include "menuButton.h"
#include "menuLabel.h"

#include "menuCom.h"
#include "menuScriptRoutine.h"
#include "menuSystem.h"
#include "menuVar.h"

#include "glSystem.h"
#include "glTextRender.h"
#include "jEngine.h"
#include "jMath.h"
#include "log.h"
#include "strSystem.h"

#include <malloc.h>
#include <wchar.h>
#ifdef _RDEBUG
#include <windows.h>
#endif

unsigned char stip50[128] = 
{
	0xAA, 0XAA, 0XAA, 0XAA,
	0x55, 0X55, 0X55, 0X55,
	0xAA, 0XAA, 0XAA, 0XAA,
	0x55, 0X55, 0X55, 0X55,
	0xAA, 0XAA, 0XAA, 0XAA,
	0x55, 0X55, 0X55, 0X55,
	0xAA, 0XAA, 0XAA, 0XAA,
	0x55, 0X55, 0X55, 0X55,
	0xAA, 0XAA, 0XAA, 0XAA,
	0x55, 0X55, 0X55, 0X55,
	0xAA, 0XAA, 0XAA, 0XAA,
	0x55, 0X55, 0X55, 0X55,
	0xAA, 0XAA, 0XAA, 0XAA,
	0x55, 0X55, 0X55, 0X55,
	0xAA, 0XAA, 0XAA, 0XAA,
	0x55, 0X55, 0X55, 0X55,
	0xAA, 0XAA, 0XAA, 0XAA,
	0x55, 0X55, 0X55, 0X55,
	0xAA, 0XAA, 0XAA, 0XAA,
	0x55, 0X55, 0X55, 0X55,
	0xAA, 0XAA, 0XAA, 0XAA,
	0x55, 0X55, 0X55, 0X55,
	0xAA, 0XAA, 0XAA, 0XAA,
	0x55, 0X55, 0X55, 0X55,
	0xAA, 0XAA, 0XAA, 0XAA,
	0x55, 0X55, 0X55, 0X55,
	0xAA, 0XAA, 0XAA, 0XAA,
	0x55, 0X55, 0X55, 0X55,
	0xAA, 0XAA, 0XAA, 0XAA,
	0x55, 0X55, 0X55, 0X55,
	0xAA, 0XAA, 0XAA, 0XAA,
	0x55, 0X55, 0X55, 0X55,

};

menuItem::menuItem(menuSystem* ms) : glRenderable(), inTextReciever()
{
	// constructor code for menuItem
	m = ms;

	children = 0;
	numChildren = 0;

	parent = 0;

	textRender = 0;

	onInit = new menuScriptRoutine(m,this);
	onShutdown = new menuScriptRoutine(m,this);
	onTimer = new menuScriptRoutine(m,this);
	onClick = new menuScriptRoutine(m,this);
	onMouseMove = new menuScriptRoutine(m,this);
	onMouseDown = new menuScriptRoutine(m,this);
	onMouseUp = new menuScriptRoutine(m,this);
	onMouseEnter = new menuScriptRoutine(m,this);
	onMouseExit = new menuScriptRoutine(m,this);

	dt = new menuVar(L"dt", 'f');

	name = 0;
	text = 0;
	font = 0;

	xPos = 10;
	yPos = 10;

	xSize = 100;
	ySize = 100;

	bgColor[0] = 0.65f;
	bgColor[1] = 0.65f;
	bgColor[2] = 0.65f;
	bgColor[3] = 0.65f;
	
	fgColor[0] = 1.0f;
	fgColor[1] = 1.0f;
	fgColor[2] = 1.0f;
	fgColor[3] = 1.0f;

	anchTop = true;
	anchLeft = true;
	anchBottom = false;
	anchRight = false;
	
	setStyles(BVS_NONE, BSS_RECTANGLE, BRS_FIXED, BGS_PLAIN);
	visible = true;

	lastMouseTest = false;
}


menuItem::menuItem(menuSystem* ms, const wchar_t* code) : glRenderable(), inTextReciever()
{
	// constructor code for menuItem
	m = ms;

	children = 0;
	numChildren = 0;

	parent = 0;

	textRender = 0;

	onInit = 0;
	onShutdown = 0;
	onTimer = 0;
	onClick = 0;
	onMouseMove = 0;
	onMouseDown = 0;
	onMouseUp = 0;
	onMouseEnter = 0;
	onMouseExit = 0;

	dt = new menuVar(L"dt", 'f');

	name = 0;
	text = 0;
	font = 0;

	xPos = 10;
	yPos = 10;

	xSize = 100;
	ySize = 100;

	bgColor[0] = 0.65f;
	bgColor[1] = 0.65f;
	bgColor[2] = 0.65f;
	bgColor[3] = 0.65f;
	
	fgColor[0] = 1.0f;
	fgColor[1] = 1.0f;
	fgColor[2] = 1.0f;
	fgColor[3] = 1.0f;

	anchTop = true;
	anchLeft = true;
	anchBottom = false;
	anchRight = false;

	setStyles(BVS_NONE, BSS_RECTANGLE, BRS_FIXED, BGS_PLAIN);
	visible = true;

	lastMouseTest = false;

	unsigned int pos = 0;

	wchar_t tType[64];
	wchar_t tName[256];

	memset(tType, 0, sizeof(wchar_t)*64);
	memset(tName, 0, sizeof(wchar_t)*256);
	swscanf(code, L"%s %s", tType, tName);

	name = new wchar_t[wcslen(tName)+1];
	name = wcscpy(name, tName);

	pos = (unsigned int)wcslen(tName)+(unsigned int)wcslen(tType)+3;

	bool inQuotes = false;

	// find each sub-scope and create the menu item or script routine for it
	while(code[pos]!=0)
	{
		if(code[pos]==L'"') inQuotes = !inQuotes;
		if(code[pos]==L'{' && !inQuotes) // we reach the beginning of a scope
		{
			// go backwards until we have found 3 spaces, stop on the 3rd space
			unsigned int bPos = pos;
			unsigned int spaceCount = 0;
			while(spaceCount < 3)
			{
				bPos--;
				if(code[bPos]==L'"') inQuotes = !inQuotes;
				if(code[bPos]==L' ' && !inQuotes) spaceCount++;
			}
			bPos++;

			memset(tType, 0, sizeof(wchar_t)*64);
			memset(tName, 0, sizeof(wchar_t)*256);
			swscanf(&(code[bPos]), L"%s %s", tType, tName);

			if(!_wcsicmp(tType, L"event"))
			{
				// add event
				if(!_wcsicmp(tName, L"oninit"))
				{
					unsigned int braceCount = 1;
					unsigned int ePos = pos;
					while(braceCount!=0)
					{
						ePos++;
						if(code[ePos]==L'"') inQuotes = !inQuotes;
						if(code[ePos]==L'{' && !inQuotes) braceCount++;
						if(code[ePos]==L'}' && !inQuotes) braceCount--;
					}

					unsigned int l = ePos-bPos + 1;
					wchar_t* nCode = new wchar_t[l+1];
					nCode[l]=0;
					wcsncpy(nCode, &(code[bPos]), l);
					onInit = new menuScriptRoutine(m, this, nCode);
					delete nCode;

					pos = ePos;
				}
				else if(!_wcsicmp(tName, L"onshutdown"))
				{
					unsigned int braceCount = 1;
					unsigned int ePos = pos;
					while(braceCount!=0)
					{
						ePos++;
						if(code[ePos]==L'"') inQuotes = !inQuotes;
						if(code[ePos]==L'{' && !inQuotes) braceCount++;
						if(code[ePos]==L'}' && !inQuotes) braceCount--;
					}

					unsigned int l = ePos-bPos + 1;
					wchar_t* nCode = new wchar_t[l+1];
					nCode[l]=0;
					wcsncpy(nCode, &(code[bPos]), l);
					onShutdown = new menuScriptRoutine(m, this, nCode);
					delete nCode;

					pos = ePos;
				}
				else if(!_wcsicmp(tName, L"ontimer"))
				{
					unsigned int braceCount = 1;
					unsigned int ePos = pos;
					while(braceCount!=0)
					{
						ePos++;
						if(code[ePos]==L'"') inQuotes = !inQuotes;
						if(code[ePos]==L'{' && !inQuotes) braceCount++;
						if(code[ePos]==L'}' && !inQuotes) braceCount--;
					}

					unsigned int l = ePos-bPos + 1;
					wchar_t* nCode = new wchar_t[l+1];
					nCode[l]=0;
					wcsncpy(nCode, &(code[bPos]), l);
					onTimer = new menuScriptRoutine(m, this, nCode, dt);
					dt->setFloat(m->getLastDT());
					delete nCode;

					pos = ePos;
				}
				else if(!_wcsicmp(tName, L"onclick"))
				{
					unsigned int braceCount = 1;
					unsigned int ePos = pos;
					while(braceCount!=0)
					{
						ePos++;
						if(code[ePos]==L'"') inQuotes = !inQuotes;
						if(code[ePos]==L'{' && !inQuotes) braceCount++;
						if(code[ePos]==L'}' && !inQuotes) braceCount--;
					}

					unsigned int l = ePos-bPos + 1;
					wchar_t* nCode = new wchar_t[l+1];
					nCode[l]=0;
					wcsncpy(nCode, &(code[bPos]), l);
					onClick = new menuScriptRoutine(m, this, nCode);
					delete nCode;

					pos = ePos;
				}
				else if(!_wcsicmp(tName, L"onmousemove"))
				{
					unsigned int braceCount = 1;
					unsigned int ePos = pos;
					while(braceCount!=0)
					{
						ePos++;
						if(code[ePos]==L'"') inQuotes = !inQuotes;
						if(code[ePos]==L'{' && !inQuotes) braceCount++;
						if(code[ePos]==L'}' && !inQuotes) braceCount--;
					}

					unsigned int l = ePos-bPos + 1;
					wchar_t* nCode = new wchar_t[l+1];
					nCode[l]=0;
					wcsncpy(nCode, &(code[bPos]), l);
					onMouseMove = new menuScriptRoutine(m, this, nCode);
					delete nCode;

					pos = ePos;
				}
				else if(!_wcsicmp(tName, L"onmousedown"))
				{
					unsigned int braceCount = 1;
					unsigned int ePos = pos;
					while(braceCount!=0)
					{
						ePos++;
						if(code[ePos]==L'"') inQuotes = !inQuotes;
						if(code[ePos]==L'{' && !inQuotes) braceCount++;
						if(code[ePos]==L'}' && !inQuotes) braceCount--;
					}

					unsigned int l = ePos-bPos + 1;
					wchar_t* nCode = new wchar_t[l+1];
					nCode[l]=0;
					wcsncpy(nCode, &(code[bPos]), l);
					onMouseDown = new menuScriptRoutine(m, this, nCode);
					delete nCode;

					pos = ePos;
				}
				else if(!_wcsicmp(tName, L"onmouseup"))
				{
					unsigned int braceCount = 1;
					unsigned int ePos = pos;
					while(braceCount!=0)
					{
						ePos++;
						if(code[ePos]==L'"') inQuotes = !inQuotes;
						if(code[ePos]==L'{' && !inQuotes) braceCount++;
						if(code[ePos]==L'}' && !inQuotes) braceCount--;
					}

					unsigned int l = ePos-bPos + 1;
					wchar_t* nCode = new wchar_t[l+1];
					nCode[l]=0;
					wcsncpy(nCode, &(code[bPos]), l);
					onMouseUp = new menuScriptRoutine(m, this, nCode);
					delete nCode;

					pos = ePos;
				}
				else if(!_wcsicmp(tName, L"onmouseenter"))
				{
					unsigned int braceCount = 1;
					unsigned int ePos = pos;
					while(braceCount!=0)
					{
						ePos++;
						if(code[ePos]==L'"') inQuotes = !inQuotes;
						if(code[ePos]==L'{' && !inQuotes) braceCount++;
						if(code[ePos]==L'}' && !inQuotes) braceCount--;
					}

					unsigned int l = ePos-bPos + 1;
					wchar_t* nCode = new wchar_t[l+1];
					nCode[l]=0;
					wcsncpy(nCode, &(code[bPos]), l);
					onMouseEnter = new menuScriptRoutine(m, this, nCode);
					delete nCode;

					pos = ePos;
				}
				else if(!_wcsicmp(tName, L"onmouseexit"))
				{
					unsigned int braceCount = 1;
					unsigned int ePos = pos;
					while(braceCount!=0)
					{
						ePos++;
						if(code[ePos]==L'"') inQuotes = !inQuotes;
						if(code[ePos]==L'{' && !inQuotes) braceCount++;
						if(code[ePos]==L'}' && !inQuotes) braceCount--;
					}

					unsigned int l = ePos-bPos + 1;
					wchar_t* nCode = new wchar_t[l+1];
					nCode[l]=0;
					wcsncpy(nCode, &(code[bPos]), l);
					onMouseExit = new menuScriptRoutine(m, this, nCode);
					delete nCode;

					pos = ePos;
				}
			}
			else if(!_wcsicmp(tType, L"item"))
			{
				// add menu item
				unsigned int braceCount = 1;
				unsigned int ePos = pos;
				while(braceCount!=0)
				{
					ePos++;
					if(code[ePos]==L'"') inQuotes = !inQuotes;
					if(code[ePos]==L'{' && !inQuotes) braceCount++;
					if(code[ePos]==L'}' && !inQuotes) braceCount--;
				}

				unsigned int l = ePos-bPos + 1;
				wchar_t* nCode = new wchar_t[l+1];
				nCode[l]=0;
				wcsncpy(nCode, &(code[bPos]), l);
				addChild(new menuItem(m, nCode));
				delete nCode;

				pos = ePos;
			}
			else if(!_wcsicmp(tType, L"button"))
			{
				// add menu item
				unsigned int braceCount = 1;
				unsigned int ePos = pos;
				while(braceCount!=0)
				{
					ePos++;
					if(code[ePos]==L'"') inQuotes = !inQuotes;
					if(code[ePos]==L'{' && !inQuotes) braceCount++;
					if(code[ePos]==L'}' && !inQuotes) braceCount--;
				}

				unsigned int l = ePos-bPos + 1;
				wchar_t* nCode = new wchar_t[l+1];
				nCode[l]=0;
				wcsncpy(nCode, &(code[bPos]), l);
				addChild(new menuButton(m, nCode));
				delete nCode;

				pos = ePos;
			}
			else if(!_wcsicmp(tType, L"label"))
			{
				// add menu item
				unsigned int braceCount = 1;
				unsigned int ePos = pos;
				while(braceCount!=0)
				{
					ePos++;
					if(code[ePos]==L'"') inQuotes = !inQuotes;
					if(code[ePos]==L'{' && !inQuotes) braceCount++;
					if(code[ePos]==L'}' && !inQuotes) braceCount--;
				}

				unsigned int l = ePos-bPos + 1;
				wchar_t* nCode = new wchar_t[l+1];
				nCode[l]=0;
				wcsncpy(nCode, &(code[bPos]), l);
				addChild(new menuLabel(m, nCode));
				delete nCode;

				pos = ePos;
			}
			//TODO::add inputbox/combobox etc...
		}
		pos++;
	}

	if(!onInit) onInit = new menuScriptRoutine(m,this);
	if(!onShutdown) onShutdown = new menuScriptRoutine(m,this);
	if(!onTimer) onTimer = new menuScriptRoutine(m,this);
	if(!onClick) onClick = new menuScriptRoutine(m,this);
	if(!onMouseMove) onMouseMove = new menuScriptRoutine(m,this);
	if(!onMouseDown) onMouseDown = new menuScriptRoutine(m,this);
	if(!onMouseUp) onMouseUp = new menuScriptRoutine(m,this);
	if(!onMouseEnter) onMouseEnter = new menuScriptRoutine(m,this);
	if(!onMouseExit) onMouseExit = new menuScriptRoutine(m,this);
}

menuItem::~menuItem()
{
	// destructor code for menuItem
	if(children) 
	{
		for(int i=0;i<numChildren;i++)
		{
			delete children[i];
		}
		free(children);
	}

	if(parent) if(name) parent->removeChildEntry(name);

	if(onInit) delete onInit;
	if(onShutdown) delete onShutdown;
	if(onTimer) delete onTimer;
	if(onClick) delete onClick;
	if(onMouseMove) delete onMouseMove;
	if(onMouseDown) delete onMouseDown;
	if(onMouseUp) delete onMouseUp;
	if(onMouseEnter) delete onMouseEnter;
	if(onMouseExit) delete onMouseExit;

	if(name) delete[] name;
	if(font) delete[] font;
	if(text) delete[] text;

	//if(textRender) delete textRender;
}

void menuItem::addChild(menuItem* i)
{
	if(i)
	{
		numChildren++;
		children = (menuItem**)realloc(children, sizeof(menuItem*)*numChildren);
		children[numChildren-1]=i;
		i->setParent(this);
	}
}

bool menuItem::isChild(const wchar_t* itemName)
{
	for(int i=0;i<numChildren;i++) if(!_wcsicmp(itemName,children[i]->getName())) return true;
	return false;
}

void menuItem::removeChild(const wchar_t* itemName)
{
	for(int i=0;i<numChildren;i++)
	{
		if(!_wcsicmp(itemName,children[i]->getName()))
		{
			delete children[i];
			numChildren--;
			for(int j=i;j<numChildren;j++) children[j]=children[j+1];
			children = (menuItem**)realloc(children, sizeof(menuItem*)*numChildren);	
			break;
		}
	}
}

// remove the entry but dont delete the child
void menuItem::removeChildEntry(const wchar_t* itemName)
{
	for(int i=0;i<numChildren;i++)
	{
		if(!_wcsicmp(itemName,children[i]->getName()))
		{
			numChildren--;
			for(int j=i;j<numChildren;j++) children[j]=children[j+1];
			children = (menuItem**)realloc(children, sizeof(menuItem*)*numChildren);	
			break;
		}
	}
}

float menuItem::getAbsoluteXPos()
{
	if(parent) return parent->getAbsoluteXPos()+xPos;
	else return xPos;
}

float menuItem::getAbsoluteYPos()
{
	if(parent) return parent->getAbsoluteYPos()+yPos;
	else return yPos;
}

void menuItem::setParent(menuItem* i)
{
	if(i) parent = i;
}

void menuItem::setText(wchar_t* iStr)
{
	if(text) delete[] text;
	int l = (int)wcslen(iStr)+1;
    text = strSystem::getPointer(jEngine::getPointer())->getString(iStr);
}

void menuItem::setStyles(miBorderVisualStyle vs, miBorderShapeStyle ss, miBorderResizeStyle rs, miBackgroundStyle bs)
{
		switch(vs)
		{
			case BVS_NONE:
			default:
				switch(bs)
				{
					case BGS_NONE:
					default:
						ren = renNoneNoneRect;
						break;
					case BGS_PLAIN:
						ren = renNonePlainRect;
						break;
					case BGS_DITHER:
						ren = renNoneDitherRect;
						break;
				}
				break;
			case BVS_SINGLE_LINE:
				switch(bs)
				{
					case BGS_NONE:
					default:
						ren = renSingleNoneRect;
						break;
					case BGS_PLAIN:
						ren = renSinglePlainRect;
						break;
					case BGS_DITHER:
						ren = renSingleDitherRect;
						break;
				}
				break;
			case BVS_DOUBLE_LINE:
				switch(bs)
				{
					case BGS_NONE:
					default:
						ren = renDoubleNoneRect;
						break;
					case BGS_PLAIN:
						ren = renDoublePlainRect;
						break;
					case BGS_DITHER:
						ren = renDoubleDitherRect;
						break;
				}
				break;
			case BVS_BEVEL_IN:
				switch(bs)
				{
					case BGS_NONE:
					default:
						ren = renBevelInNoneRect;
						break;
					case BGS_PLAIN:
						ren = renBevelInPlainRect;
						break;
					case BGS_DITHER:
						ren = renBevelInDitherRect;
						break;
				}
				break;
			case BVS_BEVEL_OUT:
				switch(bs)
				{
					case BGS_NONE:
					default:
						ren = renBevelOutNoneRect;
						break;
					case BGS_PLAIN:
						ren = renBevelOutPlainRect;
						break;
					case BGS_DITHER:
						ren = renBevelOutDitherRect;
						break;
				}
				break;
			case BVS_BEVEL_BOTH_IN:
				switch(bs)
				{
					case BGS_NONE:
					default:
						ren = renBevelBothInNoneRect;
						break;
					case BGS_PLAIN:
						ren = renBevelBothInPlainRect;
						break;
					case BGS_DITHER:
						ren = renBevelBothInDitherRect;
						break;
				}
				break;
			case BVS_BEVEL_BOTH_OUT:
				switch(bs)
				{
					case BGS_NONE:
					default:
						ren = renBevelBothOutNoneRect;
						break;
					case BGS_PLAIN:
						ren = renBevelBothOutPlainRect;
						break;
					case BGS_DITHER:
						ren = renBevelBothOutDitherRect;
						break;
				}
				break;
		}

		styleVisual = vs;
		styleShape = ss;
		styleBackground = bs;
		styleResize = rs;
}

void menuItem::eventOnInit()
{
	onInit->Call();
	for(int i=0;i<numChildren;i++) children[i]->eventOnInit();
}

void menuItem::eventOnShutdown()
{
	onShutdown->Call();
	for(int i=0;i<numChildren;i++) children[i]->eventOnShutdown();
}

void menuItem::eventOnTimer()
{
	dt->setFloat(m->getLastDT());
	onTimer->Call();
	for(int i=0;i<numChildren;i++) children[i]->eventOnTimer();
}

void menuItem::eventOnClick()
{
	onClick->Call();
}

void menuItem::eventOnMouseMove()
{
	onMouseMove->Call();
}

void menuItem::eventOnMouseDown()
{
	clickFlag = true;
	onMouseDown->Call();
}
void menuItem::eventOnMouseUp()
{
	onMouseUp->Call();
	if(clickFlag) eventOnClick();
}

void menuItem::eventOnMouseEnter()
{
	onMouseEnter->Call();
}
void menuItem::eventOnMouseExit()
{
#ifdef _RDEBUG
	MessageBox(0, L"menuItem::eventOnMouseExit line 674", L"menuItem.cpp", MB_OK);
#endif
	clickFlag = false;
	onMouseExit->Call();
}

void menuItem::initGraphics()
{
	glRenderable::initGraphics();

	textRender = m->getTextRenderDefault();

	for(int i=0;i<numChildren;i++) children[i]->initGraphics();
}

void menuItem::shutdownGraphics()
{
	for(int i=0;i<numChildren;i++) children[i]->shutdownGraphics();
	glRenderable::shutdownGraphics();
}

void menuItem::render()
{
	gl->MatrixMode(GL_MODELVIEW);
	gl->PushMatrix();
	
	gl->Translate(xPos, yPos, 0.0f);

	eventOnTimer();

	// do render
	ren(this);

	// render children
	for(int i=0;i<numChildren;i++) children[i]->render();

	gl->MatrixMode(GL_MODELVIEW);
	gl->PopMatrix();
}

bool menuItem::onMouseMovement(float x, float y, float dx, float dy)
{
	//TODO:: do some shape specific collision detection
	float absx = getAbsoluteXPos();
	float absy = getAbsoluteYPos();
	float xtest = INV_MENU_SCALE*x;
	float ytest = INV_MENU_SCALE*y;
	bool last = lastMouseTest;
	lastMouseTest = false;
	if((xtest > absx) && (xtest < (absx + xSize)) && (ytest > absy) && (ytest < (absy + ySize)))
	{
		for(int i = 0;i<numChildren;i++)
		{
			if(children[i]->onMouseMovement(x, y, dx, dy)) return true;
		}
		eventOnMouseMove();
		if(!last) eventOnMouseEnter();
		lastMouseTest = true;
		return true;
	}
	else
	{
		if(last) eventOnMouseExit();
		return false;
	}
}

bool menuItem::onMouseButtonDown(int button, float x, float y)
{
	//TODO:: do some shape specific collision detection
	float absx = getAbsoluteXPos();
	float absy = getAbsoluteYPos();
	float xtest = INV_MENU_SCALE*x;
	float ytest = INV_MENU_SCALE*y;
	if((xtest > absx) && (xtest < (absx + xSize)) && (ytest > absy) && (ytest < (absy + ySize)))
	{
		for(int i = 0;i<numChildren;i++)
		{
			if(children[i]->onMouseButtonDown(button, x, y)) return true;
		}
		eventOnMouseDown();
		return true;
	}
	else return false;
}

bool menuItem::onMouseButtonUp(int button, float x, float y)
{
	//TODO:: do some shape specific collision detection
	float absx = getAbsoluteXPos();
	float absy = getAbsoluteYPos();
	float xtest = INV_MENU_SCALE*x;
	float ytest = INV_MENU_SCALE*y;
	if((xtest > absx) && (xtest < (absx + xSize)) && (ytest > absy) && (ytest < (absy + ySize)))
	{
		for(int i = 0;i<numChildren;i++)
		{
			if(children[i]->onMouseButtonUp(button, x, y)) return true;
		}
		eventOnMouseUp();
		return true;
	}
	else return false;
}

void menuItem::comBackColor(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam)
{
	float v4 = varStack->pop()->getFloat();
	float v3 = varStack->pop()->getFloat();
	float v2 = varStack->pop()->getFloat();
	float v1 = varStack->pop()->getFloat();
	parent->setBGColor(v1,v2,v3,v4);
	return;
}

//TODO::add defines for background styles to script
void menuItem::comBackgroundStyle(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam)
{
	int v = varStack->pop()->getInt();
	v = min(max(v,0),2);
	parent->setBackgroundStyle((miBackgroundStyle)v);
	return;
}

//TODO::add defines for border styles to script
void menuItem::comBorderStyle(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam)
{
	int v = varStack->pop()->getInt();
	v = min(max(v,0),6);
	parent->setVisualStyle((miBorderVisualStyle)v);
	return;
}

void menuItem::comForeColor(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam)
{
	float v4 = varStack->pop()->getFloat();
	float v3 = varStack->pop()->getFloat();
	float v2 = varStack->pop()->getFloat();
	float v1 = varStack->pop()->getFloat();
	parent->setFGColor(v1,v2,v3,v4);
	return;
}

void menuItem::comPosition(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam)
{
	float v2 = varStack->pop()->getFloat();
	float v1 = varStack->pop()->getFloat();
	parent->setPosition(v1,v2);
	return;
}

void menuItem::comSize(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam)
{
	float v2 = varStack->pop()->getFloat();
	float v1 = varStack->pop()->getFloat();
	parent->setSize(v1,v2);
	return;
}

void menuItem::comText(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam)
{
	wchar_t* tmp = varStack->pop()->getLanguageString();
	parent->setText(tmp);
	free(tmp);
	return;
}