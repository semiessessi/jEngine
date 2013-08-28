#ifndef __SCENEJEINTRO_H
#define __SCENEJEINTRO_H

#include "gameScene.h"

class sceneJEIntro : public gameScene
{
private:
    float t;
public:
    sceneJEIntro() : t(0.0f) {};

    void onInit();
    void onRender();
};

#endif