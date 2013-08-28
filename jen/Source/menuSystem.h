/* menuSystem class by Semi Essessi
 *
 * UI and menu system
 *
 */

#ifndef __MENUSYSTEM_H
#define __MENUSYSTEM_H

class glTextRender;
class jEngine;
class inMouse;
class menuCom;
class menuItem;
class menuVar;

#include "glRenderable.h"

#include "jStack.h"

#define MENU_SCALE .001f
#define INV_MENU_SCALE 1000.0f

class menuFileLink
{
public:
	wchar_t*			menuName;
	wchar_t*			fileName;
	menuItem*			ptr;

	menuFileLink(const wchar_t* name, const wchar_t* path);
	~menuFileLink();
};

class menuSystem : public glRenderable
{
private:
	// pointer to top-level class
	jEngine* e;
	inMouse* mouse;

	// pointer to single instance
	static menuSystem*	p;

	menuItem*			main;

	menuCom**			comList;
	unsigned int		numComs;

	menuFileLink**		menuFileLinks;
	unsigned int		numMFL;

	glTextRender*		textRenderLarge;
	glTextRender*		textRenderDefault;
	glTextRender*		textRenderMono;
	glTextRender*		textRenderSerif;
	glTextRender*		textRenderSmall;

	bool rMouse;

	// make constructors and assignment private to enforce single instance
	menuSystem(jEngine* j);
	menuSystem(const menuSystem& a);
	menuSystem& operator=(const menuSystem& a);
public:
	~menuSystem();

	// return pointer to our single instance, or create it
	static menuSystem* getPointer(jEngine* j);

	float getLastDT();

	void setMouseRenderState(bool b) { rMouse = b; }
	bool getMouseRenderState() { return rMouse; }

	void addCom(const wchar_t* name, const wchar_t* help, void (*func)(menuItem*, jStack<menuVar>*, jStack<menuVar>*, menuCom*&, menuVar*&), const char* typeList = 0, unsigned int precedence = 1, bool isOp = false);
	bool isCommand(const wchar_t* name);
	menuCom* getCommand(const wchar_t* name);

	void initComs();

	void buildMenuFileList();
	void addMenuFileLink(const wchar_t* name, const wchar_t* path);
	void removeLastMenuFileLink();

	bool loadMenu(const wchar_t* name);

	bool isMenuScriptKeyword(const wchar_t* str);

	glTextRender* getTextRenderDefault() { return textRenderDefault; }
	glTextRender* getTextRenderMono() { return textRenderMono; }
	glTextRender* getTextRenderSerif() { return textRenderSerif; }
	glTextRender* getTextRenderLarge() { return textRenderLarge; }
	glTextRender* getTextRenderSmall() { return textRenderSmall; }

	void initGraphics();
	void shutdownGraphics();
	void render();

	static void ccomLoadMenu(const wchar_t* cl);
};

#endif