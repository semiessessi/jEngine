#include "mVector.h"

mVector3d::mVector3d(float f)
{
    c[0] = f;
    c[1] = f;
    c[2] = f;
    c[3] = 0;
}

mVector3d::mVector3d(mVector3d& v)
{
    c[0] = v.c[0]; c[1] = v.c[1]; c[2] = v.c[2]; c[3] = 0;
}

mVector3d::~mVector3d()
{
}

mVector3d& mVector3d::operator =(mVector3d& v)
{
    c[0] = v.c[0]; c[1] = v.c[1]; c[2] = v.c[2];
    return (*this);
}

mVector3d& mVector3d::operator =(const float& f)
{
    c[0] = f; c[1] = f; c[2] = f;
    return (*this);
}

float& mVector3d::operator [](int i)
{
    return c[i];
}

float mVector3d::Get(int i) const
{
    return c[i];
}

mVector3d mVector3d::operator +(mVector3d& v)
{
    mVector3d ret = mVector3d();
    Add4f(c, v.c, ret.c);
    return ret;
}

mVector3d mVector3d::operator -(mVector3d& v)
{
    mVector3d ret = mVector3d();
    Sub4f(c, v.c, ret.c);
    return ret;
}

mVector3d mVector3d::operator +=(mVector3d& v)
{
    Add4f(c, v.c, c);
    return (*this);
}

mVector3d mVector3d::operator -=(mVector3d& v)
{
    Sub4f(c, v.c, c);
    return (*this);
}

mVector3d mVector3d::operator *(float f)
{
    mVector3d ret = mVector3d();
    mVector3d v = mVector3d(f);
    Mul4f(c, v.c, ret.c);
    return ret;
}

mVector3d mVector3d::operator *=(float f)
{
    mVector3d v = mVector3d(f);
    Mul4f(c, v.c, c);
    return (*this);
}

float mVector3d::operator *(mVector3d &v)
{
    mVector3d w = mVector3d();
    Mul4f(c, v.c, w.c);
    return w.c[0] + w.c[1] + w.c[2];
}

mVector3d mVector3d::operator ^(mVector3d &v)
{
    mVector3d w = mVector3d();
    w.c[0] = c[1]*v.c[2] - c[2]*v.c[1];
    w.c[1] = c[2]*v.c[0] - c[0]*v.c[2];
    w.c[2] = c[0]*v.c[1] - c[1]*v.c[0];
    return w;
}

mVector3d& mVector3d::Normalise()
{
    float f = 1/sqrt(*this * *this);
    c[0] *= f; c[1] *= f; c[2] *= f;
    return *this;
}