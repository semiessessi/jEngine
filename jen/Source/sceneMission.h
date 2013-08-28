/* sceneMission class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#ifndef __SCENEMISSION_H
#define __SCENEMISSION_H

#include "gameScene.h"

class sceneMission : public gameScene
{
private:
public:
    sceneMission() {};

    void onInit();
    void onRender();
    void onShutdown();
};

#endif