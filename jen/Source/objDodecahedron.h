/* a gameScene derived class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#ifndef __OBJDODECAHEDRON_H
#define __OBJDODECAHEDRON_H

#include "glRenderable.h"

class objDodecahedron : public glRenderable
{
private:
    float vertices[20][3];
    int faces[12][5];
    int constraints[20][3];

    float sideLength;
public:
    objDodecahedron(float radius);

    void render();

    void renWireframe();

    void constrain();
};

#endif