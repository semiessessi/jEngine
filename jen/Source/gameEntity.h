/* gameEntity class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#ifndef __GAMEENTITY_H
#define __GAMEENTITY_H

#include "glRenderable.h"
#include "mVector.h"

class gameSystem;

class gameEntity : public glRenderable
{
private:
    gameSystem*     game;
public:
    gameEntity(gameSystem* g);
    virtual ~gameEntity();
    
    virtual void onInit();
    virtual void onShutdown();

    virtual void onCycle();
};

#endif