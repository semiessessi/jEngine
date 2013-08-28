/* console class by Semi Essessi
 *
 * Debug console
 *
 */

#include "console.h"
#include "jEngine.h"

#include "cCom.h"
#include "cVar.h"

#include "glSystem.h"
#include "glTextRender.h"
#include "glTriangleList2D.h"
#include "inKeyboard.h"
#include "inSystem.h"
#include "jMath.h"
#include "jProfiler.h"
#include "jStack.h"
#include "jText.h"
#include "log.h"
#include "strSystem.h"

#include <malloc.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>

console* console::p = 0;

console::console(jEngine* j) : glRenderable(), inTextReciever()
{
	// constructor code for console
	e=j;

	numComs = 0;
	numVars = 0;
	comList = 0;
	varList = 0;

	numLines = 0;
	bufLines = 0;

	bufRem = 0;
	numRem = 0;

	txt = 0;
	background = 0;

	ren = nrend;

	targPos = 0.0f;
	yPos = 0.0f;
	downState = 0;
	histPos = 0;
	cPos = 0.0f;
	pCount = 0;
#ifdef _DEBUG
	LogWriteLine(L"\x25CB\x25CB\x25CB console constructor complete");
#endif
}

console::console(const console& a)
{
	// copy constructor code for console
}

console& console::operator=(const console& a)
{
	// since we only want a single instance always return this 
	return (*this);
}

console::~console()
{
	// destructor code for console
	if(txt) delete txt;
	if(background) delete background;

	for(unsigned int i=0;i<numComs;++i)
	{
		delete comList[i];
	}
	for(unsigned int i=0;i<numVars;++i)
	{
		delete varList[i];
	}
	if(comList) free(comList);
	if(varList) free(varList);

	// cleanup output buffer
	for(int i=0;i<numLines;++i) free(bufLines[i]);
	if(bufLines) free(bufLines);

	// cleanup command history
	for(int i=0;i<numRem;++i) free(bufRem[i]);
	if(bufRem) free(bufRem);
#ifdef _DEBUG
	LogWriteLine(L"\x25CB\x25CB\x25CB console destructor complete");
#endif
}

console* console::getPointer(jEngine* j)
{
	// return our instance of console if its created 
	if(!p) p = new console(j);
	return p;
}

void console::writeLine(const char* inStr, ...)
{
	if(!inStr) return;

	wchar_t*	buf = 0;
	va_list	 parg;
	va_start(parg, inStr);
	
	// allocate buffer
	int len = (_vscprintf(inStr, parg)+1);
    unsigned int l2 = (unsigned int)strlen(inStr)+1;
    wchar_t* tmp = (wchar_t*)malloc(l2*sizeof(wchar_t));
    for(unsigned int i = 0; i < l2;++ i)
    {
        tmp[i] = (wchar_t)inStr[i];
    }
	buf = (wchar_t*)malloc(len*sizeof(wchar_t));
	if(!buf) return;
	vswprintf(buf, tmp, parg);
	va_end(parg);

	numLines++;
	bufLines = (wchar_t**)realloc(bufLines,numLines*sizeof(wchar_t*));
	bufLines[numLines-1] = buf;
#ifdef _DEBUG
	LogWriteLine(L"\x25CB\x25CF\x25CF Console output: %s", buf);
#endif
}

void console::writeLine(const wchar_t* inStr, ...)
{
	if(!inStr) return;

	wchar_t*	buf = 0;
	va_list	 parg;
	va_start(parg, inStr);
	
	// allocate buffer
	int len = (_vscwprintf(inStr, parg)+1);
	buf = (wchar_t*)malloc(len*sizeof(wchar_t));
	if(!buf) return;
	vswprintf(buf, inStr, parg);
	va_end(parg);

	numLines++;
	bufLines = (wchar_t**)realloc(bufLines,numLines*sizeof(wchar_t*));
	bufLines[numLines-1] = buf;
#ifdef _DEBUG
	LogWriteLine(L"\x25CB\x25CF\x25CF Console output: %s", buf);
#endif
}

void console::clear()
{
	for(int i=0;i<numLines;++i) free(bufLines[i]);
	if(bufLines) free(bufLines);
	bufLines=0;
	numLines=0;
}

void console::addCom(const wchar_t* name, const wchar_t* help, void (*func)(const wchar_t*))
{
	if(name&&func&&help)
	{
		cCom* com = new cCom(e,name,help,func);
		numComs++;
		
		comList = (cCom**)realloc(comList, sizeof(cCom*)*numComs);
		comList[numComs-1]=com;
	}
}

void console::addCom(const wchar_t* name, void (*func)(const wchar_t*))
{
	if(name&&func)
	{
		cCom* com = new cCom(e,name,func);
		numComs++;
		
		comList = (cCom**)realloc(comList, sizeof(cCom*)*numComs);
		comList[numComs-1]=com;
	}
}

void console::removeCom(const wchar_t* name)
{
	unsigned int i;
	for(i=0;i<(numComs+1);++i)
	{
		if(i==numComs) return;
		if(comList[i]->isCommand(name)) break;
	}

	delete comList[i];

	if(numComs==1)
	{
		numComs=0;
		free(comList);
		comList=0;
		return;
	}

	for(unsigned int j=i;j<(numComs-1);++j)
	{
		comList[j]=comList[j+1];
	}
	numComs--;

	comList = (cCom**)realloc(comList, sizeof(cCom*)*numComs);
}

bool console::isCommand(const wchar_t* name)
{
	for(unsigned int i=0;i<numComs;++i)
	{
		if(comList[i]->isCommand(name)) return true;
	}

	return false;
}

cCom* console::getCommand(const wchar_t* name)
{
	for(unsigned int i=0;i<numComs;++i)
	{
		if(comList[i]->isCommand(name)) return comList[i];
	}

	return 0;
}

void console::addVar(const wchar_t* name, const wchar_t* help, char type, void (*set)(void** p, const wchar_t* str))
{
	if(name&&help)
	{
		cVar* var = new cVar(e,name,help,type,set);
		numVars++;
		
		varList = (cVar**)realloc(varList, sizeof(cVar*)*numVars);
		varList[numVars-1]=var;
	}
}

void console::addVar(const wchar_t* name, char type, void (*set)(void** p, const wchar_t* str))
{
	if(name)
	{
		cVar* var = new cVar(e,name,type,set);
		numVars++;
		
		varList = (cVar**)realloc(varList, sizeof(cVar*)*numVars);
		varList[numVars-1]=var;
	}
}

void console::removeVar(const wchar_t* name)
{
	unsigned int i;
	for(i=0;i<(numVars+1);++i)
	{
		if(i==numVars) return;
		if(varList[i]->isVariable(name)) break;
	}

	delete varList[i];

	if(numVars==1)
	{
		numVars=0;
		free(varList);
		varList=0;
		return;
	}

	for(unsigned int j=i;j<(numVars-1);++j)
	{
		varList[j]=varList[j+1];
	}
	numVars--;

	varList = (cVar**)realloc(varList, sizeof(cVar*)*numVars);
}

bool console::isVariable(const wchar_t* name)
{
	for(unsigned int i=0;i<numVars;++i)
	{
		if(varList[i]->isVariable(name)) return true;
	}

	return false;
}

cVar* console::getVariable(const wchar_t* name)
{
	for(unsigned int i=0;i<numVars;++i)
	{
		if(varList[i]->isVariable(name)) return varList[i];
	}

	return 0;
}

void console::executeLine(const wchar_t* strLine)
{
#ifdef _DEBUG
	jProfiler profiler = jProfiler(L"console::executeLine");
#endif
	if(!strLine) return;
#ifdef _DEBUG
	LogWriteLine(L"\x25CB\x25CF\x25CF Console input: %s", strLine);
#endif	
	// extract command/variable name
	wchar_t* name = 0;
	name = strGetCmd(name, strLine);

	// is it a ccom? if so then run it
	unsigned int i;
	for(i=0;i<(numComs+1);++i)
	{
		if(i<numComs) if(comList[i]->isCommand(name)) break;
	}

	if(i<numComs) 
	{
		comList[i]->Call(strLine);
		free(name);
		return;
	}

	// is it a cvar? if so then handle it
	for(i=0;i<(numVars+1);++i)
	{
		if(i<numVars) if(varList[i]->isVariable(name)) break;
	}

	if(i<numVars)
	{
		wchar_t* pa = 0;
		pa = strGetParams(pa,strLine);
		wchar_t* nam = varList[i]->getName();
		wchar_t* val = varList[i]->getValueString();
		wchar_t* def = varList[i]->getDefaultString();
		wchar_t* lnam = e->str->getString(nam);
		wchar_t* lval = e->str->getString(val);
		wchar_t* ldef = e->str->getString(def);
		wchar_t* lstr = 0;
		if(pa)
		{
			varList[i]->setValue(pa);
			free(val);
			free(lval);
			val = varList[i]->getValueString();
			lval = e->str->getString(val);
			lstr = e->str->getString(L"%1 has been set to %2 (default: %3)", lnam, lval, ldef);
			writeLine(lstr);
		} else
		{
			lstr = e->str->getString(L"%1 is currently set to %2 (default: %3)", lnam, lval, ldef);
			writeLine(lstr);
		}
		if(pa) free(pa);
		if(val) free(val);
		if(def) free(def);
		if(lval) free(lval);
		if(ldef) free(ldef);
		if(lnam) free(lnam);
		if(lstr) free(lstr);
		free(name);
		return;
	}

	// didnt find anything, so lets display a nice error
	wchar_t* errStr = e->str->getString(L"Unrecognised command: %1", name);
	writeLine(errStr);
	if(errStr) free(errStr);
	free(name);
}

void console::executeLineAndRemember(const wchar_t* strLine)
{
#ifdef _DEBUG
	jProfiler profiler = jProfiler(L"console::executeLineAndRemember");
	LogWriteLine(L"\x25CB\x25CF\x25CF Console input: %s", strLine);
#endif
	if(!strLine) return;

	// store in history
	numRem++;
	bufRem = (wchar_t**)realloc(bufRem,sizeof(wchar_t*)*numRem);
	bufRem[numRem-1] = (wchar_t*)malloc(sizeof(wchar_t)*(wcslen(strLine)+1));
	bufRem[numRem-1] = wcscpy(bufRem[numRem-1], strLine);
	
	// execute
	executeLine(strLine);
}

wchar_t* console::getLine(int i)
{
	if(i<numLines) return bufLines[i];
	else return 0;
}

wchar_t* console::getRememberedCommand(int i)
{
	int s = max(i,numRem);
	if(s>=0) return bufRem[s];
	return 0;
}

void console::toggleDown()
{
	static inTextReciever* last = 0;

	// remove any waiting key messages
	while(GetQueueStatus(QS_KEY))
	{
		MSG m;
		PeekMessage(&m,0,0,0,PM_REMOVE);
	}

	if(downState)
	{
		e->in->setTextReciever(last);
		// ren = nrend;
		downState = 0;
		bufTime2 = 0.0f;
	}
	else
	{
		ren = rend;
		if(e->in->getKeyboard()->isKeyHeld(VK_SHIFT))
		{
			downState = 2; // small two line console
			targPos = txt->getAdjustedHeight()*2+1.5f*CONSOLE_LINE_SPACE;
		}
		else if(e->in->getKeyboard()->isKeyHeld(VK_CONTROL))
		{
			downState = 3; // fullscreen console
			targPos = 1.0f;
		}
		else 
		{
			downState = 1; // use about .3 of screen height
			targPos = -0.5f*CONSOLE_LINE_SPACE;
			while(targPos<0.3f)
			{
				targPos+=txt->getAdjustedHeight()+CONSOLE_LINE_SPACE;
			}
		}
		bufTime1 = 0.0f;
		bufTime2 = 0.0f;

		last = e->in->getTextReciever();
		e->in->setTextReciever(this);

		cPos = txt->getAdjustedWidth(inLine) + txt->getAdjustedWidth(STR_PROMPT);
	}
}

bool console::execConfig(const wchar_t* cfgFilename)
{
	if(!cfgFilename) return false;

	int len = (int)wcslen(cfgFilename);
	//FILE* f = 0;
    jTextFile f;

	if(cfgFilename[len-4]==L'.' && cfgFilename[len-3]==L'c' && cfgFilename[len-2]==L'f' && cfgFilename[len-1]==L'g')
	{
		//f = _wfopen(cfgFilename, L"rb");
        if(!(f.Load(cfgFilename))) return false;
	}
	else
	{
		wchar_t* filename = (wchar_t*)malloc((len+13)*sizeof(wchar_t));
		swprintf(filename, L"%s.cfg", cfgFilename);
		//f = _wfopen(filename, L"rb");
        if(!(f.Load(cfgFilename))) return false;
		free(filename);
	}

	//if(!f) return false;

	// skip bom
	// fseek(f, 2L, SEEK_CUR);

    /*
	while(true)
	{
		wchar_t buf[32768];

		// get line if possible
		int ret = fwscanf(f,L"%[^\r\n]\r\n", buf);
		
		if(ret<=0) break;

		// run line through console...
		executeLine(buf);
	}
    */

    wchar_t* line = 0;
    while((line = f.ReadLine()) != 0)
    {
        executeLine(line);
    }

	//fclose(f);

	// TODO::check in mod dir and find and load that x.cfg

	// force resolution int refresh
	wchar_t* str = (wchar_t*)malloc(sizeof(wchar_t)*64);
	str = _itow(e->gl->getNearestResolution(*((int*)getVariable(L"rWidth")->getPointer()),*((int*)getVariable(L"rHeight")->getPointer())), str, 10);
	str = (wchar_t*)realloc(str, sizeof(wchar_t)*(wcslen(str)+1));
	getVariable(L"rResolution")->setDefaultValue(str);
	free(str);

	return true;
}

void console::saveConfig(const wchar_t* cfgFilename)
{
	if(!cfgFilename) return;

	int len = (int)wcslen(cfgFilename);
	//FILE* f = 0;
    jTextFile f;

	if(cfgFilename[len-4]==L'.' && cfgFilename[len-3]==L'c' && cfgFilename[len-2]==L'f' && cfgFilename[len-1]==L'g')
	{
		//f = _wfopen(cfgFilename, L"wb");
        f.New(cfgFilename);
	}
	else
	{
		wchar_t* filename = (wchar_t*)malloc((len+13)*sizeof(wchar_t));
		swprintf(filename, L"%s.cfg", cfgFilename);
		//f = _wfopen(filename, L"wb");
        f.New(cfgFilename);
		free(filename);
	}

	//if(!f) return;

    /*
	// write bom
	unsigned short a = 0xFEFF;
	fwrite(&a, sizeof(unsigned short), 1, f);
    */

	// dump all cvars and values
	for(unsigned int i=0;i<numVars;++i)
	{
		wchar_t* val = 0;

		// handle string slightly differently
		if(varList[i]->getType()=='s')
		{
			val = (wchar_t*)malloc(sizeof(wchar_t)*(wcslen(varList[i]->getValueString())+1));
			val = wcscpy(val,(wchar_t*)varList[i]->getPointer());
		} else
		{
			val = varList[i]->getValueString();
		}

		//fwprintf(f, L"%s %s\r\n", varList[i]->getName(), val);
        f.Append(varList[i]->getName());
        f.Append(L" ");
        f.AppendLine(val);

		free(val);
	}

	//fclose(f);
}

// string helper functions
wchar_t* console::strGetCmd(wchar_t* outStr, const wchar_t* commandLine)
{
	wchar_t* buffer = 0;
	buffer = (wchar_t*)malloc(sizeof(wchar_t)*(wcslen(commandLine)+1));
	
	int i = 0;
	while(commandLine[i] && (commandLine[i] != L' '))
	{
		buffer[i] = commandLine[i];
		++i;
	}
	buffer[i]=L'\0';

	wchar_t* ret = (wchar_t*)realloc(outStr,sizeof(wchar_t)*(wcslen(buffer)+1));

	ret = wcscpy(ret, buffer);
	free(buffer);
	
	return ret;
}

wchar_t* console::strGetParam(wchar_t* outStr, const wchar_t* commandLine, int n)
{
	wchar_t* buffer = 0;
	buffer = (wchar_t*)malloc(sizeof(wchar_t)*(wcslen(commandLine)+1));
	
	int i = 0;
	int j = 0;
	int spacecount = 0;
	
	while(commandLine[i] && spacecount<(n+1))
	{
		if(spacecount==n && (commandLine[i]!=L' ') )
		{
			buffer[j]=commandLine[i];
			++j;
		}
		if (commandLine[i]==L' ') spacecount++;
		++i;
	}
	
	if(!j) 
	{
		free(buffer);
		return 0;
	}

	buffer[j]=L'\0';

	wchar_t* ret = (wchar_t*)realloc(outStr,sizeof(wchar_t)*(wcslen(buffer)+1));

	ret = wcscpy(ret, buffer);
	free(buffer);
	
	return ret;
}

wchar_t* console::strGetParams(wchar_t* outStr, const wchar_t* commandLine)
{
	unsigned int i = 0;
	while(commandLine[i] && (commandLine[i] != L' ')) ++i;
	++i;

	if(i>=wcslen(commandLine)) return 0;

	wchar_t* ret = (wchar_t*)realloc(outStr,sizeof(wchar_t)*(wcslen(&(commandLine[i]))+1));

	ret = wcscpy(ret, &(commandLine[i]));
	
	return ret;
}

// 'inbuilt' console commands
void console::ccomClear(const wchar_t* cl)
{
	p->clear();
}

void console::ccomDump(const wchar_t* cl)
{
	wchar_t* pa = 0;
	pa = strGetParams(pa,cl);
	if(!pa)
	{
		pa = (wchar_t*)realloc(pa,(wcslen(L"condump.txt")+1)*sizeof(wchar_t));
		pa = wcscpy(pa,L"condump.txt");
	}

	// open file and output byte order mark
	FILE* f = _wfopen(pa, L"wb");
	unsigned short a = 0xFEFF;
	fwrite(&a, sizeof(unsigned short), 1, f);

	// output lines to file
	for(int i=0;i<p->getNumLines();++i) 
	{
		fwprintf(f,L"%s\r\n", p->getLine(i));
	}

	fclose(f);

	free(pa);
}

void console::ccomEcho(const wchar_t* cl)
{
	wchar_t* str = 0;
	str = strGetParams(str, cl);
	p->writeLine(str);
	free(str);
}

void console::ccomHelp(const wchar_t* cl)
{
	strSystem* s = strSystem::getPointer(jEngine::getPointer());

	wchar_t* str = 0;
	str = strGetParam(str, cl, 1);
	if(!str)
	{
		ccomHelp(L"help help");
		ccomHelp(L"help listCComs");
		ccomHelp(L"help listCVars");
	}
	else
	{
		if(p->getCommand(str))
		{
			wchar_t* b1 = s->getString(p->getCommand(str)->getName());
			wchar_t* b2 = s->getString(p->getCommand(str)->getHelp());
			p->writeLine(L"%s: %s", b1, b2);
			free(b1);
			free(b2);
		}

		if(p->getVariable(str))
		{
			wchar_t* b1 = s->getString(p->getVariable(str)->getName());
			wchar_t* b2 = s->getString(p->getVariable(str)->getHelp());
			p->writeLine(L"%s: %s", b1, b2);
			free(b1);
			free(b2);
		}
	}
	
	free(str);
}

//TODO:finish up these console commands

void console::ccomListCcoms(const wchar_t* cl)
{
	p->lcoms();
}

void console::ccomListCvars(const wchar_t* cl)
{
	p->lvars();
}

void console::lcoms()
{
	for(unsigned int i=0;i<numComs;++i)
	{
		wchar_t* b = e->str->getString(comList[i]->getName());
		writeLine(b);
		free(b);
	}
}

void console::lvars()
{
	for(unsigned int i=0;i<numVars;++i)
	{
		wchar_t* b = e->str->getString(varList[i]->getName());
		writeLine(b);
		free(b);
	}
}

void console::ccomQuit(const wchar_t* cl)
{
	PostQuitMessage(0);
}

void console::ccomrRestart(const wchar_t* cl)
{
	jEngine::getPointer()->rRestart();
	p->saveConfig(L"autoexec.cfg");
}

void console::ccomToggleConsole(const wchar_t* cl)
{
	p->toggleDown();
}

// renderer/graphics stuff

void console::setCOpacity(void** ptr, const wchar_t* str)
{
	**((float**)ptr) = (float)_wtof(str);
	if(p->getBackgroundQuad()) p->getBackgroundQuad()->setColor(0.5f,0.5f,0.5f,**((float**)ptr));
}

void console::initGraphics()
{
#ifdef _DEBUG
	jProfiler profiler = jProfiler(L"console::initGraphics");
#endif
	glRenderable::initGraphics();

	// OpenGL strings
	*((wchar_t**)getVariable(L"sysGLExtensionString")->getDoublePointer()) = gls->getExtensionString();
	*((wchar_t**)getVariable(L"sysGLRendererString")->getDoublePointer()) = gls->getRendererString();
	*((wchar_t**)getVariable(L"sysGLVendorString")->getDoublePointer()) = gls->getVendorString();
	*((wchar_t**)getVariable(L"sysGLVersionString")->getDoublePointer()) = gls->getVersionString();

	txt = new glTextRender(e, CONSOLE_FONT_NAME, CONSOLE_FONT_SIZE);
	background = new glTriangleList2D(false);

	float asp = gls->getAspectRatio();

	background->addTexCoord(0,0);
	background->addVertex(0,-CONSOLE_LINE_SPACE);
	background->addTexCoord(0,1);
	background->addVertex(0,1);
	background->addTexCoord(1,0);
	background->addVertex(asp,-CONSOLE_LINE_SPACE);

	background->addTexCoord(1,1);
	background->addVertex(asp,1);
	background->addTexCoord(1,0);
	background->addVertex(asp,-CONSOLE_LINE_SPACE);
	background->addTexCoord(0,1);
	background->addVertex(0,1);

	background->setColor(0.5f,0.5f,0.5f,*((float*)getVariable(L"cOpacity")->getPointer()));

	background->initGraphics();
}

void console::shutdownGraphics()
{
#ifdef _DEBUG
	jProfiler profiler = jProfiler(L"console::shutdownGraphics");
#endif
	background->shutdownGraphics();
	delete txt;
	delete background;
	txt = 0;
	background = 0;
	glRenderable::shutdownGraphics();
}

void console::render()
{
	ren();
}

void console::rend()
{
	static float* cbr = (float*)(p->getVariable(L"uiCaretBlinkRate")->getPointer());
    glSystem::glFuncs* gl = p->e->gl->f;

	gl->MatrixMode(GL_PROJECTION);
	gl->PushMatrix();
	gl->LoadIdentity();
	gl->MatrixMode(GL_MODELVIEW);
	gl->PushMatrix();
	gl->LoadIdentity();

	gl->PushAttrib(GL_LIST_BIT | GL_CURRENT_BIT  | GL_ENABLE_BIT | GL_TRANSFORM_BIT);

	float color[4];
	glGetFloatv(GL_CURRENT_COLOR, color);

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	float asp = (float)viewport[2]/(float)viewport[3];

	gl->Ortho(0.0,asp,1.0,0.0,-2.0,2.0);

	gl->Disable(GL_DEPTH_TEST);

	if(p->isDown())
	{
		p->bufTime1 += p->e->timer->getLastDT();
		//HAX::second line makes the interpolation last .5 secs instead of 1
		p->bufTime1 += p->e->timer->getLastDT();
	}
	else
	{
		p->bufTime2 += p->e->timer->getLastDT();
		p->bufTime2 += p->e->timer->getLastDT();
	}

	float t1 = min(p->bufTime1, 1);
	float t2 = min(p->bufTime2, 1);

	p->yPos = (t1-t2)*p->targPos;

	if(p->yPos <= 0.0f)
	{
		p->yPos = 0.0f;
		p->ren = nrend;
	}

	gl->Translate(0.0f,p->yPos,0.0f);

	// render background

	p->getBackgroundQuad()->render();

	p->getTextRenderer()->setPosition(0.0f,p->yPos - p->getTextRenderer()->getAdjustedHeight());
	p->getTextRenderer()->printf(L"%c%s", CHAR_PROMPT, p->getInLine());

	// render caret

	// set blending for caret
	gl->Enable(GL_BLEND);
	glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ZERO);

	if(fmod(p->bufTime1,2*(*cbr))<(*cbr))
	{
		glColor4f(1.0f,1.0f,1.0f,1.0f);

		float cs = 2/(float)viewport[2];
		float iw = (p->insertMode) ? p->getTextRenderer()->getAdjustedWidth(L"O") : cs;
		cs=2*cs;

		gl->Begin(GL_QUADS);
			gl->Vertex(p->cPos+cs, - p->getTextRenderer()->getAdjustedHeight());
			gl->Vertex(p->cPos+iw+cs, - p->getTextRenderer()->getAdjustedHeight());
			gl->Vertex(p->cPos+iw+cs,0.0f);
			gl->Vertex(p->cPos+cs,0.0f);
		gl->End();
	}

	// render history
	int i = 1 + p->pCount;
    int oi = i + 10;
	while((i <= p->getNumLines()) && (i < oi))
	{
		p->getTextRenderer()->setPosition(0.0f,p->yPos - (i-p->pCount)*(p->getTextRenderer()->getAdjustedHeight() + CONSOLE_LINE_SPACE)-p->getTextRenderer()->getAdjustedHeight());
		p->getTextRenderer()->print(p->getLine(p->getNumLines()-i));
		++i;
	}

	glColor4fv(color);

	gl->MatrixMode(GL_MODELVIEW);
	gl->PopMatrix();
	gl->MatrixMode(GL_PROJECTION);
	gl->PopMatrix();
	gl->PopAttrib();
}

// text handling
void console::wmCharHandler(unsigned int wParam, long lParam)
{
	defCharHandler(wParam, lParam);
	
	// update caret position
	wchar_t buf = inLine[caretPos];
	inLine[caretPos] = 0;
	cPos = txt->getAdjustedWidth(inLine) + txt->getAdjustedWidth(STR_PROMPT);
	inLine[caretPos] = buf;
}

void console::wmKeyHandler(unsigned int wParam, long lParam)
{
	static wchar_t lbuf[1024];

	if(wParam==e->in->getConsoleVKCode())
	{
		toggleDown();
	}
	else if((wParam==VK_EXECUTE)||(wParam==VK_RETURN))
	{
		if(wcslen(inLine)>0)
		{
			executeLineAndRemember(inLine);
			memset(inLine,0,sizeof(wchar_t)*1024);
			caretPos = 0;
			histPos =0;
		}
	}
	else if(wParam==VK_UP)
	{
		if(!histPos) memcpy(lbuf,inLine,1024*sizeof(wchar_t));
		histPos=min(histPos+1,numRem);
		if(histPos)
		{
			memset(inLine,0,sizeof(wchar_t)*1024);
			wcscpy(inLine, bufRem[numRem-histPos]);
			caretPos = (int)wcslen(inLine);
		}
	}
	else if(wParam==VK_DOWN)
	{
		histPos=max(histPos-1,0);
		if(histPos)
		{
			memset(inLine,0,sizeof(wchar_t)*1024);
			wcscpy(inLine, bufRem[numRem-histPos]);
		}
		else
		{
			memcpy(inLine,lbuf,1024*sizeof(wchar_t));
		}
		caretPos = (int)wcslen(inLine);
	}
	else if(wParam==VK_PRIOR)
	{
		if(pCount<(unsigned int)(numLines-1)) pCount++;
	}
	else if(wParam==VK_NEXT)
	{
		if(pCount>0) pCount--;
	}
	else 
	{
		defKeyHandler(wParam, lParam);
	}

	// update caret position
	wchar_t buf = inLine[caretPos];
	inLine[caretPos] = 0;
	cPos = txt->getAdjustedWidth(inLine) + txt->getAdjustedWidth(STR_PROMPT);
	inLine[caretPos] = buf;
}