/* jPsuedoRandom class by Semi Essessi
 *
 * Psuedo-random number generator
 *
 */

#ifndef __JPSUEDORANDOM_H
#define __JPSUEDORANDOM_H

class jPsuedoRandom
{
private:
    unsigned int seed;
public:
    jPsuedoRandom(unsigned int s = 0);
	
    unsigned int getInt(const unsigned int& i) const;
    float getFloat(const unsigned int& i) const;
    double getDouble(const unsigned int& i) const;

    unsigned int getInt(const unsigned int& i, const unsigned int& j) const;
    float getFloat(const unsigned int& i, const unsigned int& j) const;
    double getDouble(const unsigned int& i, const unsigned int& j) const;
};

#endif