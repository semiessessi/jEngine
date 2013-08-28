/* a gameScene derived class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#ifndef __SCENEDODECAHEDRON_H
#define __SCENEDODECAHEDRON_H

#include "gameScene.h"

#include "mVector.h"

#define SCDH_NUM_PARTICLES  1000

class littleParticleSystem
{
private:
    mVector3d   x[SCDH_NUM_PARTICLES];

    mVector3d   attractor;
    mVector3d   emitter;
    mVector3d   axis;
public:
    glSystem::glFuncs*  gl;
    littleParticleSystem();

    void cycle(float dt);
    void render(unsigned int ptex, float blendFactor, float t);
};

class sceneDodecahedron : public gameScene
{
private:
    littleParticleSystem ps;

    float vertices[20][3];
    int faces[12][5];

    unsigned int ptex;
public:
    sceneDodecahedron();

    void onInit();
    void onShutdown();
    void onRender();
};

#endif