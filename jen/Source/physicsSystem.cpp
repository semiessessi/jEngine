/* physicsSystem class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#include "physicsSystem.h"

#include "jEngine.h"

#include "jTimer.h"

physicsSystem* physicsSystem::p = 0;

physicsSystem::physicsSystem(jEngine* j)
{
	// constructor code for physicsSystem
	e=j;

    objects = 0;
    numObjects = 0;
}

physicsSystem::physicsSystem(const physicsSystem& a)
{
	// copy constructor code for physicsSystem
}

physicsSystem& physicsSystem::operator=(const physicsSystem& a)
{
	// since we only want a single instance always return this 
	return (*this);
}

physicsSystem::~physicsSystem()
{
	// destructor code for physicsSystem
}

physicsSystem* physicsSystem::getPointer(jEngine* j)
{
	// return our instance of physicsSystem if its created 
	if(!p) p = new physicsSystem(j);
	return p;
}

void physicsSystem::cycle()
{
}