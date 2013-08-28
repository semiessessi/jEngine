/* gameEntity class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#include "gameEntity.h"

#include "gameSystem.h"

gameEntity::gameEntity(gameSystem* g)
{
    game = g;
}

gameEntity::~gameEntity()
{

}

void gameEntity::onInit()
{
}

void gameEntity::onShutdown()
{
}

void gameEntity::onCycle()
{
}