/* gameSystem class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#ifndef __GAMESYSTEM_H
#define __GAMESYSTEM_H

class jEngine;
class gameMission;
class gameScene;

#include "glRenderable.h"

class gameSystem : public glRenderable
{
private:
	// pointer to top-level class
	jEngine*        e;

    gameScene**     scenes;
    int             numScenes;

    int             currentScene;
    int             previousScene;
    static float    fac;
    float           time;

    bool*           bloom;
    unsigned int    bTex;

    bool            bRen;

    gameMission**   missions;
    unsigned int    numMissions;

	// pointer to single instance
	static gameSystem* p;

	// make constructors and assignment private to enforce single instance
	gameSystem(jEngine* j);
	gameSystem(const gameSystem& a);
	gameSystem& operator=(const gameSystem& a);
public:
    static float gaussianCoefficients[];

	~gameSystem();

	// return pointer to our single instance, or create it
	static gameSystem* getPointer(jEngine* j);
    static float* getBlendFactorPointer() { return &fac; }

    void addScene(gameScene* s);
    int getSceneFromName(wchar_t* n);

    void init();

    void initGraphics();
    void shutdownGraphics();
    void render();
    void renBloom();
    void renPlain();

    bool isBloomRender() { return bRen; }
    bool isBloomed() { return *bloom; }

    static void ccomSwitchScene(const wchar_t* cl);
};

#endif