/* alSystem class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#ifndef __ALSYSTEM_H
#define __ALSYSTEM_H

#pragma comment(lib, "alut.lib")
#pragma comment(lib, "openal32.lib")

class jEngine;
class alMusic;

class alSystem
{
private:
	// pointer to top-level class
	jEngine*			e;

	// pointer to single instance
	static alSystem*	p;

	alMusic*			music;

	// make constructors and assignment private to enforce single instance
	alSystem(jEngine* j);
	alSystem(const alSystem& a);
	alSystem& operator=(const alSystem& a);
public:
	~alSystem();

	// return pointer to our single instance, or create it
	static alSystem* getPointer(jEngine* j);

	alMusic* getMusic() { return music; }
};

#endif