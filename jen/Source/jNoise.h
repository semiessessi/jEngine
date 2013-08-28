/* jNoise class by Semi Essessi
 *
 * Noise generator
 *
 */

#ifndef __JNOISE_H
#define __JNOISE_H

#include "jPsuedoRandom.h"

class jNoise
{
private:
    jPsuedoRandom   rnd;

    unsigned int    scaling;
    unsigned int    depth;
    float           cutoff;
    float           p;

    float getNoiseLinear(const float& x, const unsigned int& scale) const;
    float getNoiseCosine(const float& x, const unsigned int& scale) const;
    float getNoiseCubic(const float& x, const unsigned int& scale) const;

    float getNoiseCosine(const float& x, const float& y, const unsigned int& scale, float lclip = 0.0f, float hclip = 1.0f) const;
    float getNoiseCubic(const float& x, const float& y, const unsigned int& scale, float lclip = 0.0f, float hclip = 1.0f) const;
public:
    jNoise(unsigned int seed = 0, unsigned int factor = 2, float density = 0.8f, unsigned int detail = 8, float roughness = 0.5f) : rnd(seed), scaling(factor), depth(detail), cutoff(1.0f-density), p(roughness) {};

    float getFractalCubic(const float& x, const unsigned int& lod) const;
    float getFractalCubic(const float& x, const float& y, const unsigned int& lod, float lclip = 0.0f, float hclip = 1.0f) const;

    float getFractalCosine(const float& x, const unsigned int& lod) const;
    float getFractalCosine(const float& x, const float& y, const unsigned int& lod, float lclip = 0.0f, float hclip = 1.0f) const;
};

#endif