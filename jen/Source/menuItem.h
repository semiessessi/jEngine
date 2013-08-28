/* menuItem class by Semi Essessi
 *
 * Base menu item
 *
 */

#ifndef __MENUITEM_H
#define __MENUITEM_H

#include "glRenderable.h"
#include "inTextReciever.h"
#include "inMouseReciever.h"

#include "jStack.h"

class glTextRender;
class menuCom;
class menuScriptRoutine;
class menuSystem;
class menuVar;

#define MENUITEM_Z 0.5f
#define CLIP_Z 1.0f
#define BEVEL_WIDTH 3

extern unsigned char stip50[128];

typedef enum miBorderVisualStyle
{
	BVS_NONE,
	BVS_SINGLE_LINE,
	BVS_DOUBLE_LINE,
	BVS_BEVEL_IN,
	BVS_BEVEL_OUT,
	BVS_BEVEL_BOTH_IN,
	BVS_BEVEL_BOTH_OUT,
	BVS_SKIN
};

typedef enum miBorderShapeStyle
{
	BSS_RECTANGLE,
	BSS_CURVED_RECTANGLE,
	BSS_CIRCLE,
	BSS_ELLIPSE
};

typedef enum miBorderResizeStyle
{
	BRS_FIXED,
	BRS_RESIZABLE
};

typedef enum miBackgroundStyle
{
	BGS_NONE,
	BGS_PLAIN,
	BGS_DITHER,
	BGS_TEXTURE,
	BGS_SKIN
};

class menuItem : public glRenderable, public inTextReciever, public inMouseReciever
{
protected:
	menuSystem*			m;

	menuItem*			parent;

	menuItem**			children;
	int					numChildren;

	glTextRender*		textRender;

	wchar_t*			name;
	wchar_t*			text;
	wchar_t*			font;

	float				xPos;
	float				yPos;

	float				xSize;
	float				ySize;

	float				bgColor[4];
	float				fgColor[4];

	miBorderVisualStyle styleVisual;
	miBorderShapeStyle	styleShape;
	miBorderResizeStyle styleResize;
	miBackgroundStyle	styleBackground;

	bool				anchTop;
	bool				anchLeft;
	bool				anchBottom;
	bool				anchRight;

	bool				visible;

	menuScriptRoutine*	onInit;
	menuScriptRoutine*	onShutdown;

	menuScriptRoutine*	onTimer;

	menuScriptRoutine*	onClick;
	menuScriptRoutine*	onMouseMove;

	menuScriptRoutine*	onMouseDown;
	menuScriptRoutine*	onMouseUp;

	menuScriptRoutine*	onMouseEnter;
	menuScriptRoutine*	onMouseExit;

	menuVar*			dt;

	bool				lastMouseTest;
	bool				clickFlag;

	// render funcs for different styles
	void				(*ren)(menuItem* p);

	static void			renNoneNoneRect(menuItem* p);
	static void			renSingleNoneRect(menuItem* p);
	static void			renDoubleNoneRect(menuItem* p);
	static void			renBevelInNoneRect(menuItem* p);
	static void			renBevelOutNoneRect(menuItem* p);
	static void			renBevelBothInNoneRect(menuItem* p);
	static void			renBevelBothOutNoneRect(menuItem* p);
	
	static void			renNonePlainRect(menuItem* p);
	static void			renSinglePlainRect(menuItem* p);
	static void			renDoublePlainRect(menuItem* p);
	static void			renBevelInPlainRect(menuItem* p);
	static void			renBevelOutPlainRect(menuItem* p);
	static void			renBevelBothInPlainRect(menuItem* p);
	static void			renBevelBothOutPlainRect(menuItem* p);

	static void			renNoneDitherRect(menuItem* p);
	static void			renSingleDitherRect(menuItem* p);
	static void			renDoubleDitherRect(menuItem* p);
	static void			renBevelInDitherRect(menuItem* p);
	static void			renBevelOutDitherRect(menuItem* p);
	static void			renBevelBothInDitherRect(menuItem* p);
	static void			renBevelBothOutDitherRect(menuItem* p);

	void setStyles(miBorderVisualStyle vs, miBorderShapeStyle ss, miBorderResizeStyle rs, miBackgroundStyle bs);
public:
	menuItem(menuSystem* ms);
	menuItem(menuSystem* ms, const wchar_t* code);
	virtual ~menuItem();

	menuSystem* getMenuSystemPointer() { return m; }

	virtual void addChild(menuItem* i);
	bool isChild(const wchar_t* itemName);
	void removeChild(const wchar_t* itemName);
	void removeChildEntry(const wchar_t* itemName);

	bool onMouseClick();
	bool onMouseMovement();

	void setParent(menuItem* i);

	wchar_t* getName() { return name; }
	wchar_t* getText() { return text; }

	float getXPos() { return xPos; }
	float getYPos() { return yPos; }
	float getAbsoluteXPos();
	float getAbsoluteYPos();
	float getXSize() { return xSize; }
	float getYSize() { return ySize; }

	float* getFGColor() { return fgColor; }
	float* getBGColor() { return bgColor; }

	glTextRender* getTextRender() { return textRender; }

	menuItem* getParent() { return parent; }

	void setText(wchar_t* iStr);
	void setPosition(float x, float y) { xPos = x; yPos = y; }
	void setSize(float x, float y) { xSize = x; ySize = y; }
	void setFGColor(float r, float g, float b, float a) { fgColor[0] = r; fgColor[1] = g; fgColor[2] = b; fgColor[3] = a; }
	void setBGColor(float r, float g, float b, float a) { bgColor[0] = r; bgColor[1] = g; bgColor[2] = b; bgColor[3] = a;  }

	virtual void setVisualStyle(miBorderVisualStyle s) { setStyles(s, styleShape, styleResize, styleBackground); }
	virtual void setShapeStyle(miBorderShapeStyle s) { setStyles(styleVisual, s, styleResize, styleBackground); }
	virtual void setResizeStyle(miBorderResizeStyle s) { setStyles(styleVisual, styleShape, s, styleBackground); }
	virtual void setBackgroundStyle(miBackgroundStyle s) { setStyles(styleVisual, styleShape, styleResize, s); }

	virtual void eventOnInit();
	virtual void eventOnShutdown();
	virtual void eventOnTimer();
	virtual void eventOnClick();
	virtual void eventOnMouseMove();
	virtual void eventOnMouseDown();
	virtual void eventOnMouseUp();
	virtual void eventOnMouseEnter();
	virtual void eventOnMouseExit();

	bool onMouseMovement(float x, float y, float dx, float dy);
	bool onMouseButtonDown(int button, float x, float y);
	bool onMouseButtonUp(int button, float x, float y);

	// render stuff
	void initGraphics();
	void shutdownGraphics();
	void render();

	static void comBackColor(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam);
	static void comBackgroundStyle(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam);
	static void comBorderStyle(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam);
	static void comForeColor(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam);
	static void comPosition(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam);
	static void comSize(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam);
	static void comText(menuItem* parent, jStack<menuVar>* tempStack, jStack<menuVar>* varStack, menuCom*& comParam, menuVar*& varParam);
};

#endif