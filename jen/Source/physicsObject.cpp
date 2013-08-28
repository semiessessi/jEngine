#include "physicsObject.h"

#include "jMath.h"

#define G       9.80665f
#define DT      0.1f
#define DAMP    0.975f

static float kV1[3] = { 2 - DAMP, 2 - DAMP, 2 - DAMP };
static float kV2[3] = { 1 - DAMP, 1 - DAMP, 1 - DAMP };
static float g_dt[3] = { 0, -G*DT, 0 };

physicsObject::physicsObject(float hardness, float deformability, float bouncyness)
{
    kH = hardness;
    kD = deformability;
    kR = bouncyness;

    numPositions = 0;

    x1 = x2 = x3 = x4 = y1 = y2 = y3 = y4 = 0;
}

void physicsObject::integrate()
{
    // verlet integration with constant time step
    for(int i = 0; i < numPositions; i++)
    {
        // y[] = f(t+dt) after and f(t-dt) before
        // y1[i] = kV1*x1[i] - kV2*y1[i] + g*DT*DT
        float ra[3] = { 0, 0, 0 };
        
        Mul3f(kV1, x1[i], ra);
        Mul3f(kV2, y1[i], y1[i]);
        Sub3f(ra, y1[i], y1[i]);
        Sub3f(y1[i], g_dt, y1[i]);
        
        Mul3f(kV1, x2[i], ra);
        Mul3f(kV2, y2[i], y2[i]);
        Sub3f(ra, y2[i], y2[i]);
        Sub3f(y2[i], g_dt, y2[i]);

        Mul3f(kV1, x3[i], ra);
        Mul3f(kV2, y3[i], y3[i]);
        Sub3f(ra, y3[i], y3[i]);
        Sub3f(y3[i], g_dt, y3[i]);

        Mul3f(kV1, x4[i], ra);
        Mul3f(kV2, y4[i], y4[i]);
        Sub3f(ra, y4[i], y4[i]);
        Sub3f(y4[i], g_dt, y4[i]);
    }

    // swap over because y is now newer than x
    float** b = y1;
    y1 = x1;
    x1 = b;
    b = y2;
    y2 = x2;
    x2 = b;
    b = y3;
    y3 = x3;
    x3 = b;
    b = y4;
    y4 = x4;
    x4 = b;
}

void physicsObject::updateCentreOfMass()
{
    com1[0] = com1[1] = com1[2] = com2[0] = com2[1] = com2[2] = com3[0] = com3[1] = com3[2] = com4[0] = com4[1] = com4[2] = 0.0f;

    for(int i = 0; i < numPositions; i++)
    {
        Add3f(com1, x1[i], com1);
        Add3f(com2, x1[i], com2);
        Add3f(com3, x1[i], com3);
        Add3f(com4, x1[i], com4);
    }

    float d = 1.0f / numPositions;
    float div[3] = { d, d, d };

    Mul3f(div, com1, com1);
    Mul3f(div, com2, com2);
    Mul3f(div, com3, com3);
    Mul3f(div, com4, com4);
}