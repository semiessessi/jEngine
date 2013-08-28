/* gameScene class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#ifndef __GAMESCENE_H
#define __GAMESCENE_H

class jEngine;
class gameSystem;

#include "glRenderable.h"
#include "glSystem.h"

class gameScene : public glRenderable
{
protected:
	// pointer to top-level class
	jEngine*            e;
    gameSystem*         g;

    float*              fac;
    float               blendFactor;

    wchar_t*            name;

public:
    bool                blendedIn;

    gameScene();
	virtual ~gameScene();

    void initGraphics();
    void render();
    void shutdownGraphics();

    virtual void onInit();
    virtual void onShutdown();
    virtual void onRender();
    virtual void onSwitch();

    bool isNamed(wchar_t* n);
};

#endif