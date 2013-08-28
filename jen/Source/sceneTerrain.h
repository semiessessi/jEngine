/* a gameScene derived class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#ifndef __SCENETERRAIN_H
#define __SCENETERRAIN_H

#include "gameScene.h"
#include "gameTerrain.h"

class sceneTerrain : public gameScene
{
private:
    gameTerrain*    terrain;
public:
    sceneTerrain();

    void onInit();
    void onShutdown();

    void onRender();

    void onSwitch();
};

#endif