/* console class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#include "console.h"

#include "cCom.h"
#include "cVar.h"

#include "log.h"
#include "gameSystem.h"
#include "menuSystem.h"

void console::registerCcoms()
{
	addCom(L"Clear",			L"clears the console history",												console::ccomClear);
	addCom(L"CmdList",			L"lists console commands",													console::ccomListCcoms);
	addCom(L"CVarList",			L"lists console variables",													console::ccomListCvars);
	addCom(L"Dump",				L"dumps the console history to a file",										console::ccomDump);
	addCom(L"Echo",				L"outputs the parameter string to the console",								console::ccomEcho);
	addCom(L"Exit",				L"quits without any prompt",												console::ccomQuit);
	addCom(L"Help",				L"provides help - usage: Help <command/variable name>",						console::ccomHelp);
	addCom(L"ListCComs",		L"lists console commands",													console::ccomListCcoms);
	addCom(L"ListCVars",		L"lists console variables",													console::ccomListCvars);
	addCom(L"LoadMenu",			L"loads a menu - usage: LoadMenu <menu name>",								menuSystem::ccomLoadMenu);
	addCom(L"Quit",				L"quits without any prompt",												console::ccomQuit);
	addCom(L"rRestart",			L"restarts the renderer to apply new graphics settings",					console::ccomrRestart);
    addCom(L"SwitchScene",	    L"switches the current renderer scene - usage: SwitchScene <scene>",	    gameSystem::ccomSwitchScene);
	addCom(L"ToggleConsole",	L"hides/shows this debug console",											console::ccomToggleConsole);

#ifdef _DEBUG
	LogWriteLine(L"\x25CB\x25CB\x25CF Registered console commands");
#endif
}