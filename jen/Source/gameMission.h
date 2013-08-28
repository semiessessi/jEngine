/* gameMission class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#ifndef __GAMEMISSION_H
#define __GAMEMISSION_H

#include "glRenderable.h"
#include "inMouseReciever.h"

class gameEntity;
class gameTerrain;
class gameSpaceBox;
class gameSystem;

class gameMission : public glRenderable, public inMouseReciever
{
private:
    gameSystem*     game;

    gameSpaceBox*   spaceBox;
    gameTerrain*    terrain;

    gameEntity**    entities;
    unsigned int    numEntities;
    wchar_t*        filename;
public:
    gameMission(gameSystem* g);
    ~gameMission();

    void load();
    void save();
    void loadFromFile(const wchar_t* file);
    void saveToFile(const wchar_t* file);

    void initGraphics();
    void shutdownGraphics();
    void render();
};

#endif