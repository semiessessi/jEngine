/* alSystem class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#include "alSystem.h"
#include "jEngine.h"

#include "alMusic.h"

#include "log.h"

#include <al\al.h>
#include <al\alc.h>
#include <al\alut.h>

alSystem* alSystem::p = 0;

alSystem::alSystem(jEngine* j)
{
	// constructor code for alSystem
	e=j;

	// initialise alut
	alutInit(0,0);

	music = alMusic::getPointer();

#ifdef _DEBUG
	LogWriteLine(L"\x25CB\x25CB\x25CB alSystem constructor complete");
#endif
}

alSystem::alSystem(const alSystem& a)
{
	// copy constructor code for alSystem
}

alSystem& alSystem::operator=(const alSystem& a)
{
	// since we only want a single instance always return this 
	return (*this);
}

alSystem::~alSystem()
{
	// shutdown alut
	alutExit();

	delete music;

	// destructor code for alSystem
#ifdef _DEBUG
	LogWriteLine(L"\x25CB\x25CB\x25CB alSystem destructor complete");
#endif
}

alSystem* alSystem::getPointer(jEngine* j)
{
	// return our instance of alSystem if its created 
	if(!p) p = new alSystem(j);
	return p;
}
