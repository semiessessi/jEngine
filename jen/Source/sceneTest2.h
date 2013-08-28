/* a gameScene derived class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#ifndef __SCENETEST2_H
#define __SCENETEST2_H

#include "gameScene.h"

#include "jModel.h"

class sceneTest2 : public gameScene
{
private:
    jmodel* model;
public:
    sceneTest2();
    ~sceneTest2();

    void onRender();
};

#endif