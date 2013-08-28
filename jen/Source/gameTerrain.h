/* gameTerrain class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#ifndef __GAMETERRAIN_H
#define __GAMETERRAIN_H

#include "glRenderable.h"
#include "inMouseReciever.h"

#define GTF_IS_FLAT                     0               // decides if the terrain in flat or not (overrides most other settings)
#define GTF_IS_ISLAND                   1               // decides if the terrain has a border of water or not (overrides 2)
#define GTF_IS_BEACH                    2               // decides if the terrain has an edge of water or not
#define GTF_IS_HABITABLE                3               // decides if the terrain can have living scenery (overrides 4 and 5 if necessary)
#define GTF_IS_GRASSLAND                4               // decides whether or not there is grass on the ground
#define GTF_IS_FOREST                   5               // decides whether or not there is dense forest
#define GTF_IS_ROUGH                    6               // decides whether or not the terrain is rough

#define GTF_HAS_DAY_NIGHT               24              // decides if there is a day/night cycle or not
#define GTF_HAS_DENSE_ATMOSPHERE        25              // decides if this is on a planet with thick atmosphere or not
#define GTF_HAS_ICE_SURFACE             26              // is the base surface ice, if not then rock or dirt
#define GTF_HAS_MUD                     27              // is there mud on the surface
#define GTF_HAS_DUST                    28              // is the surface dusty
#define GTF_HAS_MOUNTAINS               29              // are there mountains
#define GTF_HAS_RIVERS                  30              // are there rivers
#define GTF_HAS_LAKES                   31              // are there lakes

// top 32-bits are the seed

class glSystem;
class glSystem::glFuncs;
class gameCamera;
class gameTerrain;
class jNoise;
class mVector3d;

class gameTerrainQuadTreeNode
{
public:
    unsigned int                ownIndex;
    unsigned int                indices[6]; // 0 = -x, 1 = +x, 2 = -y, 3 = +y, 4 = 0x, 5 = 0y
    gameTerrainQuadTreeNode*    neighbours[4];
    gameTerrainQuadTreeNode*    children[4];
    float                       sumDetail;
public:
    void recurseSetup(gameTerrainQuadTreeNode* nodes);
    void nullChildren() { children[0] = children[1] = children[2] = children[3] = 0; }
};

class gameTerrainQuadTree
{
private:
    gameTerrainQuadTreeNode*    nodes;
    float**                     heights;
    unsigned int                nodeCount;
public:
    gameTerrainQuadTree(float** heightmap, const unsigned int& size);
    ~gameTerrainQuadTree();
};

class gameTerrainTexture
{
private:
    glSystem::glFuncs*      gl;
    gameTerrain*            terrain;
    mutable unsigned int    id;
    unsigned int            seed;
public:
    gameTerrainTexture() { }
    gameTerrainTexture(glSystem::glFuncs* p, gameTerrain* t, const unsigned int& s) : gl(p), terrain(t), seed(s), id(0) { }
    ~gameTerrainTexture() { }

    void initialise() const;
    void rebuild() const;
    void bindTextures() const;
    void texCoord(const float& u, const float& v) const;
    void shutdown() const;
};

class gameTerrainFlags
{
private:
    unsigned long long flags;
public:
    explicit gameTerrainFlags() : flags(0) {}
    gameTerrainFlags(const unsigned int& i) : flags((unsigned long long)1 << i) {}
    gameTerrainFlags(const gameTerrainFlags& g) : flags(g.flags) {}

    gameTerrainFlags& operator =(const gameTerrainFlags& g) { flags = g.flags; return *this; }
    gameTerrainFlags& operator =(const unsigned int& i) { flags = (unsigned long long)1 << i; return *this; }
    bool operator [](const unsigned int& i) const { return ((flags >> i) & 0x1) != 0; }
    gameTerrainFlags operator |(const unsigned int& i) const { gameTerrainFlags ret = gameTerrainFlags(*this); ret.flags |= ((unsigned long long)1 << i); return ret; }
    gameTerrainFlags& operator |=(const unsigned int& i) { flags |= ((unsigned long long)1 << i); return *this; }

    unsigned int getSeed() const { return (unsigned int)(flags >> 32); }
    void setSeed(const unsigned int& i) { flags = ((flags << 32) >> 32) | ((unsigned long long)i << 32); }
};

class gameTerrain : public glRenderable, public inMouseReciever
{
private:
    gameTerrainFlags        flags;
    jNoise*                 noise;
    gameTerrainTexture      texture;

    gameCamera*             camera;

    float**                 cachedMap;
    mVector3d**             cachedNormals;
    unsigned int            cacheHeight;
    unsigned int            cacheWidth;

    gameTerrainQuadTree*    tree;

    float                   (*heightFunc)(const gameTerrain* p, const float& x, const float& y);
    float                   (*modFunc)(const gameTerrain* p, const float& x, const float& y);

    static float _hfPlain(const gameTerrain* p, const float& x, const float& y);
    static float _hfNoise(const gameTerrain* p, const float& x, const float& y);
    static float _hfSmoothedNoise(const gameTerrain* p, const float& x, const float& y);
    static float _hfIcyNoise(const gameTerrain* p, const float& x, const float& y);

    static float _mfPlain(const gameTerrain* p, const float& x, const float& y);
    static float _mfIsland(const gameTerrain* p, const float& x, const float& y);
    static float _mfBeach(const gameTerrain* p, const float& x, const float& y);

    float** generateHeightmap(unsigned int mapHeight = 256, unsigned int mapWidth = 256, float scale = 1.0f);

    void initialise();
public:
    gameTerrain(gameTerrainFlags terrainFlags = 0);
    ~gameTerrain();

    bool isFlat() const { return flags[GTF_IS_FLAT]; }
    bool isIsland() const { return flags[GTF_IS_ISLAND]; }
    bool isBeach() const { return flags[GTF_IS_BEACH]; }
    bool isHabitable() const { return flags[GTF_IS_HABITABLE]; }
    bool isGrassland() const { return flags[GTF_IS_GRASSLAND]; }
    bool isForest() const { return flags[GTF_IS_FOREST]; }
    bool isRough() const { return flags[GTF_IS_ROUGH]; }
    
    bool hasDayNight() const { return flags[GTF_HAS_DAY_NIGHT]; }
    bool hasDenseAtmosphere() const { return flags[GTF_HAS_DENSE_ATMOSPHERE]; }
    bool hasIceSurface() const { return flags[GTF_HAS_ICE_SURFACE]; }
    bool hasMud() const { return flags[GTF_HAS_MUD]; }
    bool hasDust() const { return flags[GTF_HAS_DUST]; }
    bool hasMountains() const { return flags[GTF_HAS_MOUNTAINS]; }
    bool hasRivers() const { return flags[GTF_HAS_RIVERS]; }
    bool hasLakes() const { return flags[GTF_HAS_LAKES]; }

    float getHeight(const float& x, const float& y) const { return modFunc(this, x, y); }
    void setCacheArea(unsigned int width, unsigned int height, float scale) { generateHeightmap(height, width, scale); }

    gameCamera* getCamera() const { return camera; }
    gameTerrainFlags getFlags() const { return flags; }

    void RenderTestWireframe();
    void RenderTestShaded(mVector3d l);
    void RenderTestTextured(mVector3d l);

    void initGraphics();
    void shutdownGraphics();

    bool onMouseMovement(float x, float y, float dx, float dy);
    //bool onMouseButtonDown(int button, float x, float y);
    //bool onMouseButtonUp(int button, float x, float y);
    bool onMouseWheel(int amount);
};

#endif