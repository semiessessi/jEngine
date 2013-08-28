/* console class by Semi Essessi
 *
 * Debug console
 *
 */

#ifndef __CONSOLE_H
#define __CONSOLE_H

class jEngine;

class cCom;
class cVar;

class glTextRender;
class glTriangleList2D;

#include "glRenderable.h"
#include "inTextReciever.h"

#define CONSOLE_EFAC 0.707f
#define CONSOLE_FONT_NAME "console.ttf"
#define CONSOLE_FONT_SIZE 12
#define CONSOLE_LINE_SPACE 0.005f

class console : public glRenderable, public inTextReciever
{
private:
	// pointer to top-level class
	jEngine*			e;

	// pointer to single instance
	static console*		p;

	cCom**				comList;
	cVar**				varList;

	unsigned int		numComs;
	unsigned int		numVars;

	wchar_t**			bufLines;	
	int					numLines;

	wchar_t**			bufRem;
	int					numRem;

	glTextRender*		txt;
	glTriangleList2D*	background;

	int					downState;
	int					histPos;

	// make constructors and assignment private to enforce single instance
	console(jEngine* j);
	console(const console& a);
	console& operator=(const console& a);

	// 'inbuilt' console commands
	static void ccomClear(const wchar_t* cl);
	static void ccomDump(const wchar_t* cl);
	static void ccomEcho(const wchar_t* cl);
	static void ccomListCcoms(const wchar_t* cl);
	static void ccomListCvars(const wchar_t* cl);
	static void ccomQuit(const wchar_t* cl);
	static void ccomrRestart(const wchar_t* cl);
	static void ccomToggleConsole(const wchar_t* cl);

	static void setCOpacity(void** ptr, const wchar_t* str);

	void lvars();
	void lcoms();

	// render helper functions
	static void rend();
	static void nrend() { return; }

public:
	~console();

	// return pointer to our single instance, or create it
	static console* getPointer(jEngine* j);

	void addCom(const wchar_t* name, const wchar_t* help, void(*func)(const wchar_t*));
	void addCom(const wchar_t* name, void (*func)(const wchar_t*));
	void removeCom(const wchar_t* name);
	bool isCommand(const wchar_t* name);

	cCom* getCommand(const wchar_t* name);

	//TODO::Add overloads for more useful cvars...
	//void addVar(const wchar_t* name, const wchar_t* help, char type);
	//void addVar(const wchar_t* name, const wchar_t* help, char type);
	void addVar(const wchar_t* name, const wchar_t* help, char type, void (*set)(void** p, const wchar_t* str)=0);
	void addVar(const wchar_t* name, char type, void (*set)(void** p, const wchar_t* str)=0);
	void removeVar(const wchar_t* name);
	bool isVariable(const wchar_t* name);

	cVar* getVariable(const wchar_t* name);

	void registerCcoms();
	void registerCvars();

	void executeLine(const wchar_t* strLine);
	void executeLineAndRemember(const wchar_t* strLine);

	int getNumLines() { return numLines; }
	int getNumRememberedCommands() { return numRem; }
	wchar_t* getInLine() { return inLine; }
	wchar_t* getLine(int i);
	wchar_t* getRememberedCommand(int i);
	glTextRender* getTextRenderer() { return txt; }
	glTriangleList2D* getBackgroundQuad() { return background; }

	bool isDown() { return (downState!=0); }
	void toggleDown();

	void writeLine(const wchar_t* inStr, ...);
    void writeLine(const char* inStr, ...);
	void clear();

	bool execConfig(const wchar_t* cfgFilename);
	void saveConfig(const wchar_t* cfgFilename);

	void initGraphics();
	void shutdownGraphics();
	void render();

	void wmCharHandler(unsigned int wParam, long lParam);
	void wmKeyHandler(unsigned int wParam, long lParam);

	// annoying render related nick nacks
	float			targPos;
	float			yPos;
	float			cPos;
	float			bufTime1;
	float			bufTime2;
	unsigned int	pCount;
	void			(*ren)();

	// string helper functions
	static wchar_t* strGetCmd(wchar_t* outStr, const wchar_t* commandLine);
	static wchar_t* strGetParam(wchar_t* outStr, const wchar_t* commandLine, int n);
	static wchar_t* strGetParams(wchar_t* outStr, const wchar_t* commandLine);

	static void ccomHelp(const wchar_t* cl);
};

#endif