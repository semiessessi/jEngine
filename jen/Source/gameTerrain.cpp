/* gameTerrain class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#include "gameTerrain.h"

#include "gameCamera.h"

#include "console.h"
#include "inMouse.h"
#include "jEngine.h"
#include "jNoise.h"
#include "mVector.h"

#include <math.h>

gameTerrain::gameTerrain(gameTerrainFlags terrainFlags) : flags(terrainFlags)
{
    noise = new jNoise((unsigned int)(terrainFlags.getSeed()), 2, 0.8f, 8, 0.45f);
    camera = new gameCamera();
    texture = gameTerrainTexture(gl, this, flags.getSeed() ^ 0x381AC27F);

    initialise();
}

gameTerrain::~gameTerrain()
{
    delete noise;
    delete camera;
    
    if(cachedMap)
    {
        for(unsigned int i = 0; i < cacheWidth; ++i) delete[] cachedMap[i];
        delete[] cachedMap;
    }

    if(cachedNormals)
    {
        for(unsigned int i = 0; i < (cacheWidth - 1); ++i) delete[] cachedNormals[i];
        delete[] cachedNormals;
    }
}

void gameTerrain::initialise()
{
    heightFunc = _hfPlain;
    modFunc = _mfPlain;

    if(!isFlat())
    {
        if(isIsland())
        {
            modFunc = _mfIsland;
        }
        else if(isBeach())
        {
            modFunc = _mfBeach;
        }
        if(isRough())
        {
            heightFunc = _hfNoise;
        }
        else if(hasIceSurface())
        {
            heightFunc = _hfIcyNoise;
        }
        else
        {   
            heightFunc = _hfSmoothedNoise;
        }
    }

    cachedMap = 0;
    cachedNormals = 0;
    cacheWidth = cacheHeight = 0;
}

float gameTerrain::_mfPlain(const gameTerrain* p, const float& x, const float& y)
{
    return (p->heightFunc(p, x, y));
}

float gameTerrain::_mfIsland(const gameTerrain* p, const float& x, const float& y)
{
    float d = 0.3f*(1.0f - cosf(PI*(1.0f - min(1.2f*sqrtf(x*x + y*y), 1.0f))));
    return d + (p->heightFunc(p, x, y))*(0.9f - d);
}

float gameTerrain::_mfBeach(const gameTerrain* p, const float& x, const float& y)
{
    return x + (p->heightFunc(p, x, y));
}

float gameTerrain::_hfPlain(const gameTerrain* p, const float& x, const float& y)
{
    return 0.0f;
}

float gameTerrain::_hfNoise(const gameTerrain* p, const float& x, const float& y)
{
    return p->noise->getFractalCubic(x, y, 8);
}

float gameTerrain::_hfSmoothedNoise(const gameTerrain* p, const float& x, const float& y)
{
    float r = p->noise->getFractalCubic(x, y, 6, 0.2f, 0.8f);
    return 0.5f*r;
}

float gameTerrain::_hfIcyNoise(const gameTerrain* p, const float& x, const float& y)
{
    float r = p->noise->getFractalCosine(x, y, 5, 0.4f, 0.6f);
    return r;
}

float** gameTerrain::generateHeightmap(unsigned int mapHeight, unsigned int mapWidth, float scale)
{
    //console* con = jEngine::getPointer()->con;

    if(cachedMap)
    {
        for(unsigned int i = 0; i < cacheWidth; ++i) delete[] cachedMap[i];
        delete[] cachedMap;
    }

    ++mapHeight;
    ++mapWidth;

    cachedMap = new float*[mapWidth];
    
    if(cachedNormals)
    {
        for(unsigned int i = 0; i < (cacheWidth - 1); ++i) delete[] cachedNormals[i];
        delete[] cachedNormals;
    }

    cachedNormals = new mVector3d*[mapWidth];
    
    
    float x = -1.0f, y = -1.0f;
    float ih = 2.0f/(float)(mapHeight-1);
    float iw = 2.0f/(float)(mapWidth-1);
    for(unsigned int i = 0; i < mapWidth; ++i)
    {
        cachedMap[i] = new float[mapHeight];
        y = -1.0f;
        for(unsigned int j = 0; j < mapHeight; ++j)
        {
            cachedMap[i][j] = getHeight(scale*x, scale*y);
            y += ih;
        }
        x += iw;
    }

    x = -1.0f, y = -1.0f;
    mVector3d v1, v2, v3;
    for(unsigned int i = 0; i < (mapWidth - 1); ++i)
    {
        cachedNormals[i] = new mVector3d[(mapHeight - 1) << 1];
        y = -1.0f;
        for(unsigned int j = 0; j < (mapHeight - 1); ++j)
        {           

            // first tri
            v1[0] = x;
            v1[1] = cachedMap[i][j];
            v1[2] = y;
            
            v2[0] = x + iw;
            v2[1] = cachedMap[i + 1][j];
            v2[2] = y;

            v3[0] = x;
            v3[1] = cachedMap[i][j + 1];
            v3[2] = y + ih;

            cachedNormals[i][j << 1] = ((v1 - v2) ^ (v1 - v3)).Normalise();

            //con->writeLine("normal 1 %d,%d: %f, %f, %f", i, j, cachedNormals[i][j << 1][0], cachedNormals[i][j << 1][1], cachedNormals[i][j << 1][2]);

            // second tri
            v1 = v2;
            v2[0] = x + iw;
            v2[1] = cachedMap[i + 1][j + 1];
            v2[2] = y + ih;

            cachedNormals[i][(j << 1) + 1] = ((v1 - v2) ^ (v1 - v3)).Normalise();

            //con->writeLine("normal 2 %d,%d: %f, %f, %f", i, j, cachedNormals[i][(j << 1) + 1][0], cachedNormals[i][(j << 1) + 1][1], cachedNormals[i][(j << 1) + 1][2]);

            y += ih;
        }
        x += iw;
    }

    cacheHeight = mapHeight;
    cacheWidth = mapWidth;
    return cachedMap;
}

void gameTerrain::initGraphics()
{
    glRenderable::initGraphics();
    texture.initialise();

    camera->setDistance(2.0f);
    camera->setPitch(55);
}

void gameTerrain::shutdownGraphics()
{
    texture.shutdown();
    glRenderable::shutdownGraphics();
}

void gameTerrain::RenderTestWireframe()
{
    if(cachedMap)
    {
        gl->Disable(GL_TEXTURE_2D);

        camera->glTransforms();

        float x = -1.0f, y = -1.0f;
        float ih = 2.0f/(float)(cacheHeight-1);
        float iw = 2.0f/(float)(cacheWidth-1);
        gl->Begin(GL_LINES);
            for(unsigned int i = 0; i < cacheWidth - 1; ++i)
            {
                y = -1.0f;
                for(unsigned int j = 0; j < cacheHeight - 1; ++j)
                {
                    gl->Vertex(x, cachedMap[i][j], y);
                    gl->Vertex(x+iw, cachedMap[i+1][j], y);

                    gl->Vertex(x+iw, cachedMap[i+1][j], y);
                    gl->Vertex(x+iw, cachedMap[i+1][j+1], y+ih);

                    gl->Vertex(x+iw, cachedMap[i+1][j+1], y+ih);
                    gl->Vertex(x, cachedMap[i][j+1], y+ih);

                    gl->Vertex(x, cachedMap[i][j+1], y+ih);
                    gl->Vertex(x, cachedMap[i][j], y);

                    y += ih;
                }
                x += iw;
            }
        gl->End();
    }
}

void gameTerrain::RenderTestShaded(mVector3d l)
{
    if(cachedMap)
    {
        gl->Clear(GL_DEPTH_BUFFER_BIT);

        gl->Enable(GL_DEPTH_TEST);
	    gl->DepthFunc(GL_LESS);

        gl->Disable(GL_TEXTURE_2D);

        gl->FrontFace(GL_CW);
        gl->Enable(GL_CULL_FACE);

        camera->glTransforms();

        float x = -1.0f, y = -1.0f;
        float ih = 2.0f/(float)(cacheHeight-1);
        float iw = 2.0f/(float)(cacheWidth-1);
        float lc = 0.0f;

        gl->Begin(GL_TRIANGLES);
            for(unsigned int i = 0; i < (cacheWidth - 1); ++i)
            {
                y= -1.0f;
                for(unsigned int j = 0; j < (cacheHeight - 1); ++j)
                {
                    // first tri
                    lc = l * cachedNormals[i][j << 1];
                    gl->Color(0.29f*lc, 0.47f*lc, 0.17f*lc, 1.0f);
                    gl->Vertex(x, cachedMap[i][j], y);
                    gl->Vertex(x + iw, cachedMap[i + 1][j], y);
                    gl->Vertex(x, cachedMap[i][j + 1], y + ih);

                    // second tri
                    lc = l * cachedNormals[i][(j << 1) + 1];
                    gl->Color(0.29f*lc, 0.47f*lc, 0.17f*lc, 1.0f);
                    gl->Vertex(x + iw, cachedMap[i + 1][j], y);
                    gl->Vertex(x + iw, cachedMap[i + 1][j + 1], y + ih);
                    gl->Vertex(x, cachedMap[i][j + 1], y + ih);
                    y += ih;
                }
                x += iw;
            }
        gl->End();

        gl->Disable(GL_CULL_FACE);
    }
}

void gameTerrain::RenderTestTextured(mVector3d l)
{
    if(cachedMap)
    {
        gl->Clear(GL_DEPTH_BUFFER_BIT);

        gl->FrontFace(GL_CW);
        gl->Enable(GL_CULL_FACE);

        camera->glTransforms();

        gl->Enable(GL_DEPTH_TEST);
	    gl->DepthFunc(GL_LESS);

        gl->Enable(GL_TEXTURE_2D);
        texture.bindTextures();

        float x = -1.0f, y = -1.0f;
        float ih = 2.0f/(float)(cacheHeight-1);
        float iw = 2.0f/(float)(cacheWidth-1);

        gl->Begin(GL_TRIANGLES);
            gl->Color(1.0f,1.0f,1.0f,1.0f);
            for(unsigned int i = 0; i < (cacheWidth - 1); ++i)
            {
                y= -1.0f;
                for(unsigned int j = 0; j < (cacheHeight - 1); ++j)
                {
                    texture.texCoord(0.5f*(x + 1.0f), 0.5f*(y + 1.0f));
                    gl->Vertex(x, cachedMap[i][j], y);
                    texture.texCoord(0.5f*(x + iw + 1.0f), 0.5f*(y + 1.0f));
                    gl->Vertex(x + iw, cachedMap[i + 1][j], y);
                    texture.texCoord(0.5f*(x + 1.0f), 0.5f*(y + ih + 1.0f));
                    gl->Vertex(x, cachedMap[i][j + 1], y + ih);
                    
                    texture.texCoord(0.5f*(x + iw + 1.0f), 0.5f*(y + 1.0f));
                    gl->Vertex(x + iw, cachedMap[i + 1][j], y);
                    texture.texCoord(0.5f*(x + iw + 1.0f), 0.5f*(y + ih + 1.0f));
                    gl->Vertex(x + iw, cachedMap[i + 1][j + 1], y + ih);
                    texture.texCoord(0.5f*(x + 1.0f), 0.5f*(y + ih + 1.0f));
                    gl->Vertex(x, cachedMap[i][j + 1], y + ih);
                    
                    y += ih;
                }
                x += iw;
            }
        gl->End();

        gl->Disable(GL_CULL_FACE);
        gl->Disable(GL_TEXTURE_2D);
    }
}

bool gameTerrain::onMouseWheel(int amount)
{
    camera->setDistance(min(camera->getDistance() + (float)amount*0.1f, 2.0f));

    return true;
}

bool gameTerrain::onMouseMovement(float x, float y, float dx, float dy)
{
    if(in->getMouse()->getButton3())
    {
        camera->setYaw(camera->getYaw() + 100.0f*dx);
        camera->setPitch(camera->getPitch() + 100.0f*dy);
        return true;
    }
    else
    {
        mVector3d v = camera->getViewPosition();
        float f = gls->getLastDT()*camera->getDistance()*0.1f;
        if(x < 0.1) v[0] += f;
        else if(x > 0.9*gls->getAspectRatio()) v[0] -= f;
        if(y < 0.1) v[2] += f;
        else if(y > 0.9) v[2] -= f;

        camera->setViewPosition(v);
        return true;
    }

    return false;
}

