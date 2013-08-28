/* inMouseReciever class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#include "inMouseReciever.h"

#include "jEngine.h"

inMouseReciever::inMouseReciever()
{
	// constructor code for inMouseReciever
    in = inSystem::getPointer(jEngine::getPointer());
}

inMouseReciever::~inMouseReciever()
{
	// destructor code for inMouseReciever
}


bool inMouseReciever::onMouseMovement(float x, float y, float dx, float dy)
{
	return false;
}

bool inMouseReciever::onMouseButtonDown(int button, float x, float y)
{
	return false;
}

bool inMouseReciever::onMouseButtonUp(int button, float x, float y)
{
	return false;
}

bool inMouseReciever::onMouseWheel(int amount)
{
	return false;
}