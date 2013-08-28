/* gameMission class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#include "gameMission.h"

#include "gameEntity.h"
#include "gameSystem.h"
#include "gameTerrain.h"

#include "jText.h"

#include <stdio.h>

const unsigned int HEADER1 = 100;

gameMission::gameMission(gameSystem* g)
{
    game = g;

    entities = 0;
    numEntities = 0;

    terrain = 0;
    spaceBox = 0;

    filename = 0;
}

gameMission::~gameMission()
{
    for(unsigned int i = 0; i < numEntities; ++i) delete entities[i];
    if(entities) free(entities);
    if(filename) delete[] filename;
}

void gameMission::loadFromFile(const wchar_t* file)
{
    if(filename) delete[] filename;

    int len = wcslen(file) + 1;
    filename = new wchar_t[wcslen(file) + 1];

    for(int i = 0; i < len; ++i) filename[i] = file[i];

    load();
}

void gameMission::saveToFile(const wchar_t* file)
{
    if(filename) delete[] filename;

    int len = wcslen(file) + 1;
    filename = new wchar_t[wcslen(file) + 1];

    for(int i = 0; i < len; ++i) filename[i] = file[i];

    save();
}

void gameMission::load()
{
    jKeyFile f;
    f.Load(filename);
}

void gameMission::save()
{

}

void gameMission::initGraphics()
{
}

void gameMission::shutdownGraphics()
{
}

void gameMission::render()
{
}