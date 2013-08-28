/* console class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#include "jEngine.h"

#include "console.h"
#include "glSystem.h"

#include "cCom.h"
#include "cVar.h"

#include "jMath.h"
#include "log.h"
#include "strSystem.h"

void console::registerCvars()
{
	// add variables
	addVar(L"cOpacity",						L"the opacity of the console background",														'f',		console::setCOpacity);

    addVar(L"rBloom",				        L"use a bloom for non UI rendering",															'b');
	addVar(L"rDoubleBuffer",				L"use double buffered rendering",																'b');
	addVar(L"rFullscreen",					L"fullscreen (false for windowed)",																'b');
	addVar(L"rHeight",						L"preferred screen height",																		'i');
	addVar(L"rRefresh",						L"preferred refresh rate",																		'i');
	addVar(L"rResolution",					L"number representing preferred fullscreen resolution",											'i');
    addVar(L"rShowFPS", 				    L"show framerate",											                                    'b');
    addVar(L"rTerrainTextureDetail",        L"detail level of terrain texture (0-6)",   				                                    'i');
    addVar(L"rVSync", 				        L"vertical sync",		                                                                        'b',        glSystem::setVSync);
	addVar(L"rWidth",						L"preferred screen width",																		'i');

	addVar(L"sysGLExtensionString",			L"the OpenGL extension string",																	's',		cVar::setNop);
	addVar(L"sysGLRendererString",			L"the OpenGL renderer string",																	's',		cVar::setNop);
	addVar(L"sysGLVendorString",			L"the OpenGL vendor string",																	's',		cVar::setNop);
	addVar(L"sysGLVersionString",			L"the OpenGL version string",																	's',		cVar::setNop);

	addVar(L"sysHasMMX",					L"does the CPU support MMX instructions",														'b',		cVar::setNop);
	addVar(L"sysHasSSE",					L"does the CPU support SSE instructions",														'b',		cVar::setNop);
	addVar(L"sysHasSSE2",					L"does the CPU support SSE2 instructions",														'b',		cVar::setNop);
	addVar(L"sysHasSSE3",					L"does the CPU support SSE3 instructions",														'b',		cVar::setNop);

	addVar(L"sysMemoryAvailable",			L"available memory of system on startup, to the nearest megabyte (rounded down)",				'i',		cVar::setNop);
	addVar(L"sysMemoryAvailablePhysical",	L"available physical memory of system on startup, to the nearest megabyte (rounded down)",		'i',		cVar::setNop);
	addVar(L"sysMemoryTotal",				L"total memory of system to the nearest megabyte (rounded down)",								'i',		cVar::setNop);
	addVar(L"sysMemoryTotalPhysical",		L"total physical memory of system to the nearest megabyte (rounded down)",						'i',		cVar::setNop);

	addVar(L"uiCaretBlinkRate",				L"speed at which the text input caret flashes",													'f');
	addVar(L"uiLanguage",					L"current language preference",																	's',		strSystem::setUILanguage);

	// set up default values if different to normal
	getVariable(L"cOpacity")->setDefaultValue(L"0.4");
	getVariable(L"rFullscreen")->setDefaultValue(L"true");
	getVariable(L"rHeight")->setDefaultValue(L"600");
	getVariable(L"rWidth")->setDefaultValue(L"800");
	getVariable(L"rRefresh")->setDefaultValue(L"75");
	getVariable(L"rDoubleBuffer")->setDefaultValue(L"true");
    getVariable(L"rBloom")->setDefaultValue(L"true");
    getVariable(L"rShowFPS")->setDefaultValue(L"false");
    getVariable(L"rTerrainTextureDetail")->setDefaultValue(L"3");
    getVariable(L"rVSync")->setDefaultValue(L"true");

	// cpu capabilities
	getVariable(L"sysHasMMX")->setDefaultValue(L"false");
	getVariable(L"sysHasSSE")->setDefaultValue(L"false");
	getVariable(L"sysHasSSE2")->setDefaultValue(L"false");
	getVariable(L"sysHasSSE3")->setDefaultValue(L"false");

	*((int*)getVariable(L"sysMemoryAvailable")->getPointer()) = getTotalMemory();
	*((int*)getVariable(L"sysMemoryAvailablePhysical")->getPointer()) = getAvailRAM();
	*((int*)getVariable(L"sysMemoryTotal")->getPointer()) = getAvailMemory();
	*((int*)getVariable(L"sysMemoryTotalPhysical")->getPointer()) = getTotalRAM();

	*((bool*)getVariable(L"sysHasMMX")->getPointer()) = cpuHasMMX();
	*((bool*)getVariable(L"sysHasSSE")->getPointer()) = cpuHasSSE();
	*((bool*)getVariable(L"sysHasSSE2")->getPointer()) = cpuHasSSE2();
	*((bool*)getVariable(L"sysHasSSE3")->getPointer()) = cpuHasSSE3();
	

	// get nearest resolution and store in rResolution, this is to make the menu easier
	wchar_t* str = (wchar_t*)malloc(sizeof(wchar_t)*64);
	str = _itow(e->gl->getNearestResolution(800,600), str, 10);
	str = (wchar_t*)realloc(str, sizeof(wchar_t)*(wcslen(str)+1));
	getVariable(L"rResolution")->setDefaultValue(str);
	free(str);

	getVariable(L"uiCaretBlinkRate")->setDefaultValue(L"1");
	getVariable(L"uiLanguage")->setDefaultValue(L"english");

    // force default values to start with
	getVariable(L"cOpacity")->setToDefaultValue();

	getVariable(L"rFullscreen")->setToDefaultValue();
	getVariable(L"rHeight")->setToDefaultValue();
	getVariable(L"rWidth")->setToDefaultValue();
	getVariable(L"rRefresh")->setToDefaultValue();
	getVariable(L"rResolution")->setToDefaultValue();
	getVariable(L"rDoubleBuffer")->setToDefaultValue();
    getVariable(L"rBloom")->setToDefaultValue();
    getVariable(L"rShowFPS")->setToDefaultValue();
    getVariable(L"rTerrainTextureDetail")->setToDefaultValue();
    getVariable(L"rVSync")->setToDefaultValue();

	getVariable(L"uiCaretBlinkRate")->setToDefaultValue();
	getVariable(L"uiLanguage")->setToDefaultValue();

#ifdef _DEBUG
	LogWriteLine(L"\x25CB\x25CB\x25CF Registered console variables");
#endif
}

void glSystem::setVSync(void** p, const wchar_t* str)
{
    glSystem::p->e->con->getVariable(L"rVSync")->setBool(p, str);
    if(glSystem::p->f)
    {
        glSystem::p->f->SwapInterval((**((bool**)p)) ? 1 : 0);
    }
}