/* a gameScene derived class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#include "sceneTerrain.h"

#include "gameCamera.h"
#include "gameSystem.h"
#include "jEngine.h"
#include "inMouseReciever.h"
#include "inSystem.h"
#include "jMath.h"
#include "mVector.h"

sceneTerrain::sceneTerrain() : gameScene()
{
    name = new wchar_t[wcslen(L"terraintest") + 1];
    name = wcscpy(name, L"terraintest");
}

static inMouseReciever* old = 0;

void sceneTerrain::onInit()
{
    gameTerrainFlags f;
    f |= GTF_HAS_ICE_SURFACE;
    //f |= GTF_IS_GRASSLAND;
    //f |= GTF_IS_ISLAND;
    f.setSeed(127);
    terrain = new gameTerrain(f);
    terrain->setCacheArea(64,64, 1.0f);
    terrain->initGraphics();

    old = jEngine::getPointer()->in->getMouseReciever();
    jEngine::getPointer()->in->setMouseReciever(terrain);
}

void sceneTerrain::onShutdown()
{
    terrain->shutdownGraphics();
    jEngine::getPointer()->in->setMouseReciever(old);
    delete terrain;
}

void sceneTerrain::onRender()
{
    // clear screen that works well with blending
    gl->Color(0.0f, 0.0f, 0.0f, blendFactor);
    gl->Begin(GL_QUADS);
        gl->Vertex(1,1);
        gl->Vertex(1,-1);
        gl->Vertex(-1,-1);
        gl->Vertex(-1,1);
    gl->End();

    // timer
    static float t = 0.0f;
    if((g->isBloomRender() && g->isBloomed()) || !(g->isBloomed()))
    {
        t += gls->getLastDT();
    }
	
    // terrain->getCamera()->setYaw(30.0f*t);

    mVector3d lx;
    lx[0] = 0.5f;//*cos(cDEG2RAD((30*t+225)));
    lx[1] = -0.7f;
    lx[2] = 0.5f;//*sin(cDEG2RAD((30*t+225)));

    terrain->RenderTestTextured(lx.Normalise());
}

void sceneTerrain::onSwitch()
{
    jEngine::getPointer()->in->setMouseReciever(terrain);
}