/* alMusic class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#include "alMusic.h"

#include "log.h"

alMusic* alMusic::p = 0;

alMusic::alMusic()
{
	// constructor code for alMusic
#ifdef _DEBUG
	LogWriteLine(L"\x25CB\x25CB\x25CB alMusic constructor complete");
#endif
}

alMusic::alMusic(const alMusic& a)
{
	// copy constructor code for alMusic
}

alMusic& alMusic::operator=(const alMusic& a)
{
	// since we only want a single instance always return this 
	return (*this);
}

alMusic::~alMusic()
{
	// destructor code for alMusic
#ifdef _DEBUG
	LogWriteLine(L"\x25CB\x25CB\x25CB alMusic destructor complete");
#endif
}

alMusic* alMusic::getPointer()
{
	// return our instance of alMusic if its created 
	if(!p) p = new alMusic();
	return p;
}
