/* physicsSystem class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#ifndef __PHYSICSSYSTEM_H
#define __PHYSICSSYSTEM_H

class jEngine;
class physicsObject;

class physicsSystem
{
private:
	// pointer to top-level class
	jEngine*        e;

    physicsObject** objects;
    int numObjects;

	// pointer to single instance
	static physicsSystem* p;

	// make constructors and assignment private to enforce single instance
	physicsSystem(jEngine* j);
	physicsSystem(const physicsSystem& a);
	physicsSystem& operator=(const physicsSystem& a);
public:
	~physicsSystem();

	// return pointer to our single instance, or create it
	static physicsSystem* getPointer(jEngine* j);

    void cycle();
};

#endif