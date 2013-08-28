/* gameScene class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#include "jEngine.h"

#include "gameSystem.h"

#include "gameScene.h"

#include "log.h"

gameScene::gameScene() : glRenderable()
{
	// constructor code for gameScene
	e = jEngine::getPointer();
    g = e->game;
    fac = gameSystem::getBlendFactorPointer();
    blendedIn = false;
    name = 0;
}

gameScene::~gameScene()
{
    if(name) delete name;
}

void gameScene::initGraphics()
{
    glRenderable::initGraphics();
    onInit();
}

void gameScene::shutdownGraphics()
{
    onShutdown();
    glRenderable::shutdownGraphics();
}

void gameScene::render()
{
    // blendFactor = blendedIn ? (1.0f - *fac) : *fac;
    blendFactor = blendedIn ? 1.0f : *fac;
    if(*fac >= 1.0f)
    {
        if(!blendedIn) blendedIn = true;
        blendFactor = 1.0f;
    }

    onRender();
}

void gameScene::onInit()
{
}

void gameScene::onShutdown()
{
}

void gameScene::onRender()
{
}

void gameScene::onSwitch()
{
}


bool gameScene::isNamed(wchar_t* n)
{
    return (_wcsicmp(name, n) == 0);
}