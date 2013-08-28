/* physicsObject class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#ifndef __PHYSICSOBJECT_H
#define __PHYSICSOBJECT_H

class physicsSystem;

class distanceConstraint
{
public:
    float               d;
    int                 v1;
    int                 v2;
};

class angleConstraint
{
public:
    float               a;
    int                 v1;
    int                 v2;
    int                 v3;
};

class plane
{
public:
    int                 v1;
    int                 v2;
    int                 v3;
    bool                contained;
    int*                borderVertices;
};

class physicsObject
{
private:
    physicsSystem*      p;

    float               kH;         // hardness
    float               kD;         // deformability
    float               kR;         // resititution

    // positions
    float**             x1;
    float**             x2;
    float**             x3;
    float**             x4;
    // last positions
    float**             y1;
    float**             y2;
    float**             y3;
    float**             y4;

    int numPositions;

    float               com1[3];    // centre of masses
    float               com2[3];
    float               com3[3];
    float               com4[3];

    distanceConstraint**    dC;
    angleConstraint**       aC;

    int numDC;
    int numAC;

    plane**             planes;

    int numPlanes;

    void integrate();
    void relaxConstraints();
    void advanceBuffers();
public:
    physicsObject(float hardness = 1.0f, float deformability = 0.0f, float bouncyness = 0.7f);
	~physicsObject();

    float getHardness() { return kH; }
    float getDeformability() { return kD; }
    float getRestitution() { return kR; }

    void updateCentreOfMass();
};

#endif