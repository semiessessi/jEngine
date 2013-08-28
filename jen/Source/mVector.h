#ifndef MVECTOR_H
#define MVECTOR_H

#include "jMath.h"

class mVector3d
{
private:
    float   c[4];
public:
    mVector3d(float f = 0.0f);
    mVector3d(mVector3d& v);
    ~mVector3d();

    mVector3d& operator =(mVector3d& v);
    mVector3d& operator =(const float& f);

    float& operator [](int i);
    float Get(int i) const;

    mVector3d operator +(mVector3d& v);
    mVector3d operator +(const float& f) { return (*this) + mVector3d(f); }
    mVector3d operator -() { return 0.0f - *this; }
    mVector3d operator -(mVector3d& v);
    mVector3d operator -(const float& f) { return (*this) - mVector3d(f); }
    mVector3d operator +=(mVector3d& v);
    mVector3d operator -=(mVector3d& v);
    mVector3d operator *(float f);
    mVector3d operator *=(float f);
    friend mVector3d operator +(float f, mVector3d& v) { return v + mVector3d(f); }
    friend mVector3d operator -(float f, mVector3d& v) { return mVector3d(f) - v; }
    friend mVector3d operator *(float f, mVector3d& v) { return v*f; }
    float operator *(mVector3d& v);          // dot
    mVector3d operator ^(mVector3d& v);      // cross

    mVector3d& Normalise();
};

#endif