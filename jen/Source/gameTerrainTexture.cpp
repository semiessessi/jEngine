/* gameTerrainTexture class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#include "gameTerrain.h"

#include "console.h"
#include "cVar.h"
#include "jEngine.h"
#include "jMath.h"
#include "jNoise.h"
#include "jProfiler.h"
#include "mVector.h"

#include <malloc.h>
#define torgba(r,g,b) (((unsigned int)(r*255.0f)) | (((unsigned int)(g*255.0f)) << 8) | (((unsigned int)(b*255.0f)) << 16) | 0xFF000000)
#define tored(rgba) ((float)(rgba & 0xFF) / 255.0f)
#define togreen(rgba) ((float)((rgba >> 8) & 0xFF) / 255.0f)
#define toblue(rgba) ((float)((rgba >> 16) & 0xFF) / 255.0f)

void buildHalfSizeMipmap(unsigned int* rgb[], unsigned int* rgb1[], const unsigned int& size)
{
    // jProfiler profiler = jProfiler(L"gameTerrainTexture.cpp - buildHalfSizeMipmap");
    float re, gr, bl;
    // build mipmap
    // for the edges use on only a bilinear filter, but use bicubic inside

    // do top/bottom row
    for(unsigned int i = 0; i < (size >> 1); ++i)
    {
        re = 0.25f*(tored(rgb[i << 1][0]) + tored(rgb[i << 1][1]) + tored(rgb[(i << 1) + 1][0]) + tored(rgb[(i << 1) + 1][1]));
        gr = 0.25f*(togreen(rgb[i << 1][0]) + togreen(rgb[i << 1][1]) + togreen(rgb[(i << 1) + 1][0]) + togreen(rgb[(i << 1) + 1][1]));
        bl = 0.25f*(toblue(rgb[i << 1][0]) + toblue(rgb[i << 1][1]) + toblue(rgb[(i << 1) + 1][0]) + toblue(rgb[(i << 1) + 1][1]));
        rgb1[i][0] = torgba(re, gr, bl);

        re = 0.25f*(tored(rgb[i << 1][(size - 2)]) + tored(rgb[i << 1][(size - 1)]) + tored(rgb[(i << 1) + 1][(size - 2)]) + tored(rgb[(i << 1) + 1][(size - 1)]));
        gr = 0.25f*(togreen(rgb[i << 1][(size - 2)]) + togreen(rgb[i << 1][(size - 1)]) + togreen(rgb[(i << 1) + 1][(size - 2)]) + togreen(rgb[(i << 1) + 1][(size - 1)]));
        bl = 0.25f*(toblue(rgb[i << 1][(size - 2)]) + toblue(rgb[i << 1][(size - 1)]) + toblue(rgb[(i << 1) + 1][(size - 2)]) + toblue(rgb[(i << 1) + 1][(size - 1)]));
        rgb1[i][((size >> 1) - 1)] = torgba(re, gr, bl);
    }

    // do left/right column minus top and bottom
    for(unsigned int j = 1; j < ((size >> 1) - 1); ++j)
    {
        re = 0.25f*(tored(rgb[0][j << 1]) + tored(rgb[1][j << 1]) + tored(rgb[0][(j << 1) + 1]) + tored(rgb[1][(j << 1) + 1]));
        gr = 0.25f*(togreen(rgb[0][j << 1]) + togreen(rgb[1][j << 1]) + togreen(rgb[0][(j << 1) + 1]) + togreen(rgb[1][(j << 1) + 1]));
        bl = 0.25f*(toblue(rgb[0][j << 1]) + toblue(rgb[1][j << 1]) + toblue(rgb[0][(j << 1) + 1]) + toblue(rgb[1][(j << 1) + 1]));
        rgb1[0][j] = torgba(re, gr, bl);

        re = 0.25f*(tored(rgb[(size - 2)][j << 1]) + tored(rgb[(size - 1)][j << 1]) + tored(rgb[(size - 2)][(j << 1) + 1]) + tored(rgb[(size - 1)][(j << 1) + 1]));
        gr = 0.25f*(togreen(rgb[(size - 2)][j << 1]) + togreen(rgb[(size - 1)][j << 1]) + togreen(rgb[(size - 2)][(j << 1) + 1]) + togreen(rgb[(size - 1)][(j << 1) + 1]));
        bl = 0.25f*(toblue(rgb[(size - 2)][j << 1]) + toblue(rgb[(size - 1)][j << 1]) + toblue(rgb[(size - 2)][(j << 1) + 1]) + toblue(rgb[(size - 1)][(j << 1) + 1]));
        rgb1[((size >> 1) - 1)][j] = torgba(re, gr, bl);
    }

    // do the remaining square with bicubic
    for(unsigned int i = 1; i < ((size >> 1) - 1); ++i)
    {
        for(unsigned int j = 1; j < ((size >> 1) - 1); ++j)
        {
            float r[16] = { tored(rgb[(i << 1) - 1][(j << 1) - 1]),
                            tored(rgb[(i << 1)][(j << 1) - 1]),
                            tored(rgb[(i << 1) + 1][(j << 1) - 1]),
                            tored(rgb[(i << 1) + 2][(j << 1) - 1]),
                            tored(rgb[(i << 1) - 1][(j << 1)]),
                            tored(rgb[(i << 1)][(j << 1)]),
                            tored(rgb[(i << 1) + 1][(j << 1)]),
                            tored(rgb[(i << 1) + 2][(j << 1)]),
                            tored(rgb[(i << 1) - 1][(j << 1) + 1]),
                            tored(rgb[(i << 1)][(j << 1) + 1]),
                            tored(rgb[(i << 1) + 1][(j << 1) + 1]),
                            tored(rgb[(i << 1) + 2][(j << 1) + 1]),
                            tored(rgb[(i << 1) - 1][(j << 1) + 2]),
                            tored(rgb[(i << 1)][(j << 1) + 2]),
                            tored(rgb[(i << 1) + 1][(j << 1) + 2]),
                            tored(rgb[(i << 1) + 2][(j << 1) + 2]) };
            float g[16] = { togreen(rgb[(i << 1) - 1][(j << 1) - 1]),
                            togreen(rgb[(i << 1)][(j << 1) - 1]),
                            togreen(rgb[(i << 1) + 1][(j << 1) - 1]),
                            togreen(rgb[(i << 1) + 2][(j << 1) - 1]),
                            togreen(rgb[(i << 1) - 1][(j << 1)]),
                            togreen(rgb[(i << 1)][(j << 1)]),
                            togreen(rgb[(i << 1) + 1][(j << 1)]),
                            togreen(rgb[(i << 1) + 2][(j << 1)]),
                            togreen(rgb[(i << 1) - 1][(j << 1) + 1]),
                            togreen(rgb[(i << 1)][(j << 1) + 1]),
                            togreen(rgb[(i << 1) + 1][(j << 1) + 1]),
                            togreen(rgb[(i << 1) + 2][(j << 1) + 1]),
                            togreen(rgb[(i << 1) - 1][(j << 1) + 2]),
                            togreen(rgb[(i << 1)][(j << 1) + 2]),
                            togreen(rgb[(i << 1) + 1][(j << 1) + 2]),
                            togreen(rgb[(i << 1) + 2][(j << 1) + 2]) };
            float b[16] = { toblue(rgb[(i << 1) - 1][(j << 1) - 1]),
                            toblue(rgb[(i << 1)][(j << 1) - 1]),
                            toblue(rgb[(i << 1) + 1][(j << 1) - 1]),
                            toblue(rgb[(i << 1) + 2][(j << 1) - 1]),
                            toblue(rgb[(i << 1) - 1][(j << 1)]),
                            toblue(rgb[(i << 1)][(j << 1)]),
                            toblue(rgb[(i << 1) + 1][(j << 1)]),
                            toblue(rgb[(i << 1) + 2][(j << 1)]),
                            toblue(rgb[(i << 1) - 1][(j << 1) + 1]),
                            toblue(rgb[(i << 1)][(j << 1) + 1]),
                            toblue(rgb[(i << 1) + 1][(j << 1) + 1]),
                            toblue(rgb[(i << 1) + 2][(j << 1) + 1]),
                            toblue(rgb[(i << 1) - 1][(j << 1) + 2]),
                            toblue(rgb[(i << 1)][(j << 1) + 2]),
                            toblue(rgb[(i << 1) + 1][(j << 1) + 2]),
                            toblue(rgb[(i << 1) + 2][(j << 1) + 2]) };

            r[0] = ipCubic(r[0], r[1], r[2], r[3], 0.5f);
            r[1] = ipCubic(r[4], r[5], r[6], r[7], 0.5f);
            r[2] = ipCubic(r[8], r[9], r[10], r[11], 0.5f);
            r[3] = ipCubic(r[12], r[13], r[14], r[15], 0.5f);
            r[0] = ipCubic(r[0], r[1], r[2], r[3], 0.5f);

            g[0] = ipCubic(g[0], g[1], g[2], g[3], 0.5f);
            g[1] = ipCubic(g[4], g[5], g[6], g[7], 0.5f);
            g[2] = ipCubic(g[8], g[9], g[10], g[11], 0.5f);
            g[3] = ipCubic(g[12], g[13], g[14], g[15], 0.5f);
            g[0] = ipCubic(g[0], g[1], g[2], g[3], 0.5f);
            
            b[0] = ipCubic(b[0], b[1], b[2], b[3], 0.5f);
            b[1] = ipCubic(b[4], b[5], b[6], b[7], 0.5f);
            b[2] = ipCubic(b[8], b[9], b[10], b[11], 0.5f);
            b[3] = ipCubic(b[12], b[13], b[14], b[15], 0.5f);
            b[0] = ipCubic(b[0], b[1], b[2], b[3], 0.5f);

            rgb1[i][j] = torgba(max(0.0f,min(1.0f,r[0])),max(0.0f,min(1.0f,g[0])),max(0.0f,min(1.0f,b[0])));
        }
    }
}

void gameTerrainTexture::initialise() const
{
    jProfiler profiler = jProfiler(L"gameTerrainTexture::initialise");

    static int& rTerrainTextureDetail = *((int*)(jEngine::getPointer()->con->getVariable(L"rTerrainTextureDetail")->getPointer()));

    jNoise n = jNoise(seed);

    unsigned int* rgb[4096];
    unsigned int* rgb1[2048];
    unsigned int* rgb2[1024];
    unsigned int* rgb3[512];
    unsigned int* rgb4[256];
    unsigned int* rgb5[128];
    unsigned int* rgb6[64];
    unsigned int* rgb7[32];
    unsigned int* rgb8[16];
    unsigned int* rgb9[8];
    unsigned int* rgb10[4];
    unsigned int* rgb11[2];

    unsigned int* rgbdata;
    unsigned int* rgb1data;
    unsigned int* rgb2data;
    unsigned int* rgb3data;
    unsigned int* rgb4data;
    unsigned int* rgb5data;
    unsigned int* rgb6data;
    unsigned int* rgb7data;
    unsigned int  rgb8data[16*16];
    unsigned int  rgb9data[8*8];
    unsigned int  rgb10data[4*4];
    unsigned int  rgb11data[2*2];

    switch(rTerrainTextureDetail)
    {
        default:
            rgbdata = new unsigned int[4096*4096];
        case 5:
            rgb1data = new unsigned int[2048*2048];
        case 4:
            rgb2data = new unsigned int[1024*1024];
        case 3:
            rgb3data = new unsigned int[512*512];
        case 2:
            rgb4data = new unsigned int[256*256];
        case 1:
            rgb5data = new unsigned int[128*128];
        case 0:
            rgb6data = new unsigned int[64*64];
    }
    rgb7data = new unsigned int[32*32];

    unsigned int size = 4096;
    unsigned int** ptr = rgb;
    switch(rTerrainTextureDetail)
    {
        case 0:
            size = 64;
            ptr = rgb6;
            break;
        case 1:
            size = 128;
            ptr = rgb5;
            break;
        case 2:
            size = 256;
            ptr = rgb4;
            break;
        case 3:
            size = 512;
            ptr = rgb3;
            break;
        case 4:
            size = 1024;
            ptr = rgb2;
            break;
        case 5:
            size = 2048;
            ptr = rgb1;
            break;
    }

    // lots of switch fall through in this function...
    switch(rTerrainTextureDetail)
    {
        default:
            for(unsigned int i = 0; i < 4096; ++i) rgb[i] = &(rgbdata[i*4096]);
        case 5:
            for(unsigned int i = 0; i < 2048; ++i) rgb1[i] = &(rgb1data[i*2048]);
        case 4:
            for(unsigned int i = 0; i < 1024; ++i) rgb2[i] = &(rgb2data[i*1024]);
        case 3:
            for(unsigned int i = 0; i < 512; ++i) rgb3[i] = &(rgb3data[i*512]);
        case 2:
            for(unsigned int i = 0; i < 256; ++i) rgb4[i] = &(rgb4data[i*256]);
        case 1:
            for(unsigned int i = 0; i < 128; ++i) rgb5[i] = &(rgb5data[i*128]);
        case 0:
            for(unsigned int i = 0; i < 64; ++i) rgb6[i] = &(rgb6data[i*64]);
    }

    for(unsigned int i = 0; i < 32; ++i) rgb7[i] = &(rgb7data[i*32]);
    for(unsigned int i = 0; i < 16; ++i) rgb8[i] = &(rgb8data[i*16]);
    for(unsigned int i = 0; i < 8; ++i) rgb9[i] = &(rgb9data[i*8]);
    for(unsigned int i = 0; i < 4; ++i) rgb10[i] = &(rgb10data[i*4]);
    for(unsigned int i = 0; i < 2; ++i) rgb11[i] = &(rgb11data[i*2]);

    float cg[3] = { 0.29f, 0.47f, 0.17f };
    float cd[3] = { 0.24f, 0.21f, 0.09f };

    if(!(terrain->isGrassland()))
    {
        if(terrain->hasIceSurface())
        {
            cg[0] = 0.95f; cg[1] = 0.97f; cg[2] = 0.89f;
            cd[0] = 0.89f; cd[1] = 0.95f; cd[2] = 0.97f;
        }
        else
        {
            cg[0] = 0.7f; cg[1] = 0.65f; cg[2] = 0.65f;
            cd[0] = 0.6f; cd[1] = 0.575f; cd[2] = 0.575f;
        }
    }

    float x = -1.0f, y = -1.0f;
    const float ih = 2.0f/((float)size);
    const float iw = 2.0f/((float)size);
    float pixel[3];
    float& r = pixel[0];
    float& g = pixel[1];
    float& b = pixel[2];
    // for reasons i fail to grasp, this is incredibly slow compared to the renderer, which samples a more expensive noise function
    // hopefully rendering with GL is a good, quick and dirty fix
    int vp[4] = { 0,0,0,0 };
    if(size > 512)
    {
        for(unsigned int i = 0; i < size; ++i)
        {
            y = -1.0f;
            for(unsigned int j = 0; j < size; ++j)
            {
                r = n.getFractalCubic(x, y, 7);
                g = r*cg[1] + (1-r)*cd[1];
                b = r*cg[2] + (1-r)*cd[2];
                r = r*cg[0] + (1-r)*cd[0];
                ptr[i][j] = torgba(r,g,b);
                y += ih;
            }
            x += iw;
        }
    }
    else
    {
        // clear screen and render the top mip level
        glGetIntegerv(GL_VIEWPORT, vp);
        glViewport(0, 0, size, size);
        gl->Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        gl->Enable(GL_TEXTURE_2D);

        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        for(unsigned int i = 0; i < size; ++i)
        {
            y = -1.0f;
            for(unsigned int j = 0; j < size; ++j)
            {
                r = n.getFractalCubic(x, y, 7);
                g = r*cg[1] + (1-r)*cd[1];
                b = r*cg[2] + (1-r)*cd[2];
                r = r*cg[0] + (1-r)*cd[0];
                glRasterPos2f(x, y);
                glDrawPixels(1, 1, GL_RGB, GL_FLOAT, pixel);
                y += ih;
            }
            x += ih;
        }

        glReadPixels(0, 0, size, size, GL_RGBA, GL_UNSIGNED_BYTE, *ptr);
    }

    if(!(terrain->hasDayNight()))
    {
        // precompile some lighting
        mVector3d lx;
        lx[0] = 0.5f;
        lx[1] = -0.7f;
        lx[2] = 0.5f;
        lx.Normalise();
        x = -1.0f, y = -1.0f;
        ///*
        const float ih = 1.0f/((float)size);
        const float iw = 1.0f/((float)size);
        //*/
        mVector3d v1, v2, v3, v4, normal;
        
        // again, here is something which, is really slow... 
        ///*
        float tmp;
        for(unsigned int i = 0; i < size; ++i)
        {
            y = -1.0f;
            for(unsigned int j = 0; j < size; ++j)
            {
                r = tored(ptr[i][j]);
                g = togreen(ptr[i][j]);
                b = toblue(ptr[i][j]);

                // get two normals for tris across the square, average and normalise them
                v1[0] = x - iw;
                v1[1] = terrain->getHeight(x - iw, y - ih);
                v1[2] = y - ih;
            
                v2[0] = x + iw;
                v2[1] = terrain->getHeight(x + iw, y - ih);
                v2[2] = y - ih;

                v3[0] = x + iw;
                v3[1] = terrain->getHeight(x + iw, y + ih);
                v3[2] = y + ih;

                v4[0] = x - iw;
                v4[1] = terrain->getHeight(x - iw, y + ih);
                v4[2] = y + ih;

                normal = (((v3 - v4) ^ (v3 - v1)).Normalise() + ((v1 - v2) ^ (v1 - v3)).Normalise())*0.5f;

                tmp = min(max(normal*lx, 0.0f), 1.0f);
                r *= tmp;
                g *= tmp;
                b *= tmp;
                
                ptr[i][j] = torgba(r,g,b);
                y += ih;
                y += ih;
            }
            x += iw;
            x += iw;
        }
        //*/

        /* fails spectacularly
        glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, size, size, 0);

        gl->Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        gl->MatrixMode(GL_PROJECTION);
        gl->LoadIdentity();
        gl->Ortho(-1,1,-1,1,-1,1);
        gl->MatrixMode(GL_MODELVIEW);
        gl->LoadIdentity();
        
        gl->Begin(GL_TRIANGLES);
        x = -1.0f;
        for(unsigned int i = 0; i < (size - 1); ++i)
        {
            y = -1.0f;
            for(unsigned int j = 0; j < (size - 1); ++j)
            {
                v1[0] = x - iw;
                v1[1] = terrain->getHeight(x - iw, y - ih);
                v1[2] = y - ih;
            
                v2[0] = x + iw;
                v2[1] = terrain->getHeight(x + iw, y - ih);
                v2[2] = y - ih;

                v3[0] = x + iw;
                v3[1] = terrain->getHeight(x + iw, y + ih);
                v3[2] = y + ih;

                v4[0] = x - iw;
                v4[1] = terrain->getHeight(x - iw, y + ih);
                v4[2] = y + ih;

                normal = ((((v3 - v4) ^ (v3 - v1)).Normalise() + ((v1 - v2) ^ (v1 - v3)).Normalise())*0.5f).Normalise();
                normal = normal*lx;

                gl->Color(normal);
                gl->TexCoord(0.5f*(v1[0] + 1.0f), 0.5f*(v1[2] + 1.0f));
                gl->Vertex(v1[0], v1[2]);
                gl->TexCoord(0.5f*(v2[0] + 1.0f), 0.5f*(v2[2] + 1.0f));
                gl->Vertex(v2[0], v2[2]);
                gl->TexCoord(0.5f*(v3[0] + 1.0f), 0.5f*(v3[2] + 1.0f));
                gl->Vertex(v3[0], v3[2]);
                
                gl->TexCoord(0.5f*(v3[0] + 1.0f), 0.5f*(v3[2] + 1.0f));
                gl->Vertex(v3[0], v3[2]);
                gl->TexCoord(0.5f*(v4[0] + 1.0f), 0.5f*(v4[2] + 1.0f));
                gl->Vertex(v4[0], v4[2]);
                gl->TexCoord(0.5f*(v1[0] + 1.0f), 0.5f*(v1[2] + 1.0f));
                gl->Vertex(v1[0], v1[2]);

                y += ih;   
            }
            x += iw;
        }
        gl->End();

        glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, size, size, 0);
        */
    }    

    // build mipmaps
    switch(rTerrainTextureDetail)
    {
        default:
            buildHalfSizeMipmap(rgb, rgb1, 4096);
        case 5:
            buildHalfSizeMipmap(rgb1, rgb2, 2048);
        case 4:
            buildHalfSizeMipmap(rgb2, rgb3, 1024);
        case 3:
            buildHalfSizeMipmap(rgb3, rgb4, 512);
        case 2:
            buildHalfSizeMipmap(rgb4, rgb5, 256);
        case 1:
            buildHalfSizeMipmap(rgb5, rgb6, 128);
        case 0:
            buildHalfSizeMipmap(rgb6, rgb7, 64);
    }
    buildHalfSizeMipmap(rgb7, rgb8, 32);
    buildHalfSizeMipmap(rgb8, rgb9, 16);
    buildHalfSizeMipmap(rgb9, rgb10, 8);
    buildHalfSizeMipmap(rgb10, rgb11, 4);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // sorry, i use fall through here, no breaks!!! :)

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, *ptr);
    unsigned int mipLevel = 1;
    switch(rTerrainTextureDetail)
    {
        default:
            glTexImage2D(GL_TEXTURE_2D, mipLevel, GL_RGB, 2048, 2048, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgb1data);
            ++mipLevel;
        case 5:
            glTexImage2D(GL_TEXTURE_2D, mipLevel, GL_RGB, 1024, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgb2data);
            ++mipLevel;
        case 4:
            glTexImage2D(GL_TEXTURE_2D, mipLevel, GL_RGB, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgb3data);
            ++mipLevel;
        case 3:
            glTexImage2D(GL_TEXTURE_2D, mipLevel, GL_RGB, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgb4data);
            ++mipLevel;
        case 2:
            glTexImage2D(GL_TEXTURE_2D, mipLevel, GL_RGB, 128, 128, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgb5data);
            ++mipLevel;
        case 1:
            glTexImage2D(GL_TEXTURE_2D, mipLevel, GL_RGB, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgb6data);
            ++mipLevel;
        case 0:
            break;
    }

    glTexImage2D(GL_TEXTURE_2D, mipLevel, GL_RGB, 32, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgb7data);
    ++mipLevel;
    glTexImage2D(GL_TEXTURE_2D, mipLevel, GL_RGB, 16, 16, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgb8data);
    ++mipLevel;
    glTexImage2D(GL_TEXTURE_2D, mipLevel, GL_RGB, 8, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgb9data);
    ++mipLevel;
    glTexImage2D(GL_TEXTURE_2D, mipLevel, GL_RGB, 4, 4, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgb10data);
    ++mipLevel;
    glTexImage2D(GL_TEXTURE_2D, mipLevel, GL_RGB, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgb11data);
    ++mipLevel;
    glTexImage2D(GL_TEXTURE_2D, mipLevel, GL_RGB, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgb11data);

    gl->Disable(GL_TEXTURE_2D);

    switch(rTerrainTextureDetail)
    {
        default:
            delete[] rgbdata;
        case 5:
            delete[] rgb1data;
        case 4:
            delete[] rgb2data;
        case 3:
            delete[] rgb3data;
        case 2:
            delete[] rgb4data;
        case 1:
            delete[] rgb5data;
        case 0:
            delete[] rgb6data;
    }
    delete[] rgb7data;

    if(size < 1024) glViewport(vp[0], vp[1], vp[2], vp[3]);

    gl->Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void gameTerrainTexture::shutdown() const
{
    gl->Enable(GL_TEXTURE_2D);
    glDeleteTextures(1, &id);
    gl->Disable(GL_TEXTURE_2D);
}

void gameTerrainTexture::bindTextures() const
{
    glBindTexture(GL_TEXTURE_2D, id);
}

void gameTerrainTexture::texCoord(const float& u, const float& v) const
{
    /*
        u       v       x
        u       1-v     x
        1-u     v       x
        1-u     1-v     x

        v       u       
        v       1-u     
        1-v     u       
        1-v     1-u     
    */

    gl->TexCoord(v, u);
}