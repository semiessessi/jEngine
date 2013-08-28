/* jNoise class by Semi Essessi
 *
 * Noise generator
 *
 */

#include "jNoise.h"

#include "jMath.h"

#define forceint(x) ((x == 0.0f) ? (unsigned int)0 : *((unsigned int*)&x))
#define LARGE_FLOAT_1 1654.165f
#define LARGE_FLOAT_2 4545.884f

float jNoise::getNoiseLinear(const float& x, const unsigned int& scale) const
{
    float t = x * (float)(scale << 1);
    float r = floor(t);
    float s = ceil(t);
    t = t - r;
    
    return ipLinear(rnd.getFloat(forceint(r)), rnd.getFloat(forceint(s)), t);
}

float jNoise::getNoiseCosine(const float& x, const unsigned int& scale) const
{
    float t = x * (float)(scale << 1);
    float r = floor(t);
    float s = ceil(t);
    t = t - r;
    
    return ipCosine(rnd.getFloat(forceint(r)), rnd.getFloat(forceint(s)), t);
}

float jNoise::getNoiseCosine(const float& x, const float& y, const unsigned int& scale, float lclip, float hclip) const
{
    float t = x * (float)(scale << 1);
    float r = floor(t);
    float s = ceil(t);
    t -= r;

    float u = y * (float)(scale << 1);
    float v = floor(u);
    float w = ceil(u);
    u -= v;
    
    return min(hclip, max(lclip,
                ipCosine(
                    ipCosine(
                        rnd.getFloat(forceint(r), forceint(v)), 
                        rnd.getFloat(forceint(s), forceint(v)), 
                        t), 
                    ipCosine(   
                        rnd.getFloat(forceint(r), forceint(w)), 
                        rnd.getFloat(forceint(s), forceint(w)), 
                    t),
                u)
           ));
}

float jNoise::getNoiseCubic(const float& x, const unsigned int& scale) const
{
    float t = x * (float)(scale << 1);
    float b = floor(t);
    float c = ceil(t);
    t -= b;
    if(b == c) c += 1.0f;
    float a = b - 1.0f;
    float d = c + 1.0f;
    
    return ipCubic(rnd.getFloat(forceint(a)), rnd.getFloat(forceint(b)), rnd.getFloat(forceint(c)), rnd.getFloat(forceint(d)), t);
}

float jNoise::getNoiseCubic(const float& x, const float& y, const unsigned int& scale, float lclip, float hclip) const
{
    float t = x * (float)(scale << 1);
    float bx = floor(t);
    float cx = ceil(t);
    t -= bx;
    if(bx == cx) cx += 1.0f;
    float ax = bx - 1.0f;
    float dx = cx + 1.0f;

    float u = y * (float)(scale << 1);
    float by = floor(u);
    float cy = ceil(u);
    u -= by;
    if(by == cy) cy += 1.0f;
    float ay = by - 1.0f;
    float dy = cy + 1.0f;

    float n[16] = { rnd.getFloat(forceint(ax), forceint(ay)),
                    rnd.getFloat(forceint(bx), forceint(ay)),
                    rnd.getFloat(forceint(cx), forceint(ay)),
                    rnd.getFloat(forceint(dx), forceint(ay)),
                    rnd.getFloat(forceint(ax), forceint(by)),
                    rnd.getFloat(forceint(bx), forceint(by)),
                    rnd.getFloat(forceint(cx), forceint(by)),
                    rnd.getFloat(forceint(dx), forceint(by)),
                    rnd.getFloat(forceint(ax), forceint(cy)),
                    rnd.getFloat(forceint(bx), forceint(cy)),
                    rnd.getFloat(forceint(cx), forceint(cy)),
                    rnd.getFloat(forceint(dx), forceint(cy)),
                    rnd.getFloat(forceint(ax), forceint(dy)),
                    rnd.getFloat(forceint(bx), forceint(dy)),
                    rnd.getFloat(forceint(cx), forceint(dy)),
                    rnd.getFloat(forceint(dx), forceint(dy)) };

    float x1 = ipCubic(n[0], n[1], n[2], n[3], t);
    float x2 = ipCubic(n[4], n[5], n[6], n[7], t);
    float x3 = ipCubic(n[8], n[9], n[10], n[11], t);
    float x4 = ipCubic(n[12], n[13], n[14], n[15], t);

    return min(hclip, max(lclip, ipCubic(x1, x2, x3, x4, u)));
}

float jNoise::getFractalCubic(const float& x, const unsigned int& lod) const
{
    float r = 0.0f;
    unsigned int l = (lod > depth) ? depth : lod;
    unsigned int s = 1;
    float pe = 1.0f;
    for(unsigned int i = 0; i < lod; ++i)
    {
        r += pe * getNoiseCubic(x, s);
        pe *= p;
        s *= scaling;
    }

    return 0.5f*r;
}

float jNoise::getFractalCubic(const float& x, const float& y, const unsigned int& lod, float lclip, float hclip) const
{
    float r = 0.0f, n = 0.0f;
    unsigned int l = (lod > depth) ? depth : lod;
    unsigned int s = 1;
    float pe = 1.0f;
    for(unsigned int i = 0; i < lod; ++i)
    {
        // n = getNoiseCubic(x, y, s);
        // r += pe * ((n > hclip) ? hclip : ((n < lclip) ? lclip : n));
        // TODO: replace dirty fix hiding behaviour at origin
        r += pe * getNoiseCubic(x + LARGE_FLOAT_1, y + LARGE_FLOAT_2, s, lclip, hclip);
        pe *= p;
        s *= scaling;
    }

    return 0.5f*r;
}

float jNoise::getFractalCosine(const float& x, const unsigned int& lod) const
{
    float r = 0.0f;
    unsigned int l = (lod > depth) ? depth : lod;
    unsigned int s = 1;
    float pe = 1.0f;
    for(unsigned int i = 0; i < lod; ++i)
    {
        r += pe * getNoiseCosine(x, s);
        pe *= p;
        s *= scaling;
    }

    return 0.5f*r;
}

float jNoise::getFractalCosine(const float& x, const float& y, const unsigned int& lod, float lclip, float hclip) const
{
    float r = 0.0f, n = 0.0f;
    unsigned int l = (lod > depth) ? depth : lod;
    unsigned int s = 1;
    float pe = 1.0f;
    for(unsigned int i = 0; i < lod; ++i)
    {
        // n = getNoiseCosine(x, y, s);
        // r += pe * ((n > hclip) ? hclip : ((n < lclip) ? lclip : n));
        // TODO: replace dirty fix hiding behaviour at origin
        r += pe * getNoiseCosine(x + LARGE_FLOAT_1, y + LARGE_FLOAT_2, s, lclip, hclip);
        pe *= p;
        s *= scaling;
    }

    return 0.5f*r;
}