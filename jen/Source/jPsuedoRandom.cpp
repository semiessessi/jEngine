/* jPsuedoRandom class by Semi Essessi
 *
 * Psuedo-random number generator
 *
 */

#include "jPsuedoRandom.h"

const float iMax = 1.0f / 4294967295.0f;

jPsuedoRandom::jPsuedoRandom(unsigned int s)
{
    // mess up the bits a bit
    seed = s + 0x381AC27F;
    seed = (seed << 12) ^ (seed >> 20);
}

unsigned int jPsuedoRandom::getInt(const unsigned int& i) const
{
    unsigned long long l = ((seed ^ i)*16664525 + 1013904223);
    //unsigned long long l = seed ^ i;
    l *= l;
    return (unsigned int)((l<<16)>>32);
}

float jPsuedoRandom::getFloat(const unsigned int& i) const
{
    return (float)getInt(i) * iMax;
}

double jPsuedoRandom::getDouble(const unsigned int& i) const
{
    return (double)getInt(i) * iMax;
}

unsigned int jPsuedoRandom::getInt(const unsigned int& i, const unsigned int& j) const
{
    unsigned long long l = ((getInt(i) ^ j)*16664525 + 1013904223);
    //unsigned long long l = getInt(i) ^ j
    l *= l;
    return (unsigned int)((l<<16)>>32);
}

float jPsuedoRandom::getFloat(const unsigned int& i, const unsigned int& j) const
{
    return (float)getInt(i, j) * iMax;
}

double jPsuedoRandom::getDouble(const unsigned int& i, const unsigned int& j) const
{
    return (double)getInt(i, j) * iMax;
}