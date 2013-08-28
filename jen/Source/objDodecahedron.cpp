/* a gameScene derived class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#include "objDodecahedron.h"

#include "gameScene.h"
#include "jMath.h"

#ifndef PI
#define PI 3.1415926535897932384626433832795f
#endif
#define PHIA 0.91843825753316954191056421394367f
#define PHIB 0.18871054084413369665112800937666f
#define DEG72 1.2566370614359172953850573533118f
#define DEG36 0.6283185307179586476925286766559f

objDodecahedron::objDodecahedron(float radius)
{
    // initialise data
    float a = 0.0f;
    for(int i = 0; i < 5; i++)
    {
        vertices[i][0] = radius*cos(a)*cos(PHIA);
        vertices[i][1] = radius*sin(a)*cos(PHIA);
        vertices[i][2] = radius*sin(PHIA);
        a = a + DEG72;
    }
    a = 0.0f;
    for(int i = 5; i < 10; i++)
    {
        vertices[i][0] = radius*cos(a)*cos(PHIB);
        vertices[i][1] = radius*sin(a)*cos(PHIB);
        vertices[i][2] = radius*sin(PHIB);
        a = a + DEG72;
    }
    a = DEG36;
    for(int i = 10; i < 15; i++)
    {
        vertices[i][0] = radius*cos(a)*cos(-PHIB);
        vertices[i][1] = radius*sin(a)*cos(-PHIB);
        vertices[i][2] = radius*sin(-PHIB);
        a = a + DEG72;
    }
    a = DEG36;
    for(int i = 15; i < 20; i++)
    {
        vertices[i][0] = radius*cos(a)*cos(-PHIA);
        vertices[i][1] = radius*sin(a)*cos(-PHIA);
        vertices[i][2] = radius*sin(-PHIA);
        a = a + DEG72;
    }

    faces[0][0] = 0;
    faces[0][1] = 1;
    faces[0][2] = 2;
    faces[0][3] = 3;
    faces[0][4] = 4;

    faces[1][0] = 0;
    faces[1][1] = 1;
    faces[1][2] = 6;
    faces[1][3] = 10;
    faces[1][4] = 5;

    faces[2][0] = 1;
    faces[2][1] = 2;
    faces[2][2] = 7;
    faces[2][3] = 11;
    faces[2][4] = 6;

    faces[3][0] = 2;
    faces[3][1] = 3;
    faces[3][2] = 8;
    faces[3][3] = 12;
    faces[3][4] = 7;

    faces[4][0] = 3;
    faces[4][1] = 4;
    faces[4][2] = 9;
    faces[4][3] = 13;
    faces[4][4] = 8;

    faces[5][0] = 4;
    faces[5][1] = 0;
    faces[5][2] = 5;
    faces[5][3] = 14;
    faces[5][4] = 9;

    faces[6][0] = 15;
    faces[6][1] = 16;
    faces[6][2] = 11;
    faces[6][3] = 6;
    faces[6][4] = 10;

    faces[7][0] = 16;
    faces[7][1] = 17;
    faces[7][2] = 12;
    faces[7][3] = 7;
    faces[7][4] = 11;

    faces[8][0] = 17;
    faces[8][1] = 18;
    faces[8][2] = 13;
    faces[8][3] = 8;
    faces[8][4] = 12;

    faces[9][0] = 18;
    faces[9][1] = 19;
    faces[9][2] = 14;
    faces[9][3] = 9;
    faces[9][4] = 13;

    faces[10][0] = 19;
    faces[10][1] = 15;
    faces[10][2] = 10;
    faces[10][3] = 5;
    faces[10][4] = 14;

    faces[11][0] = 15;
    faces[11][1] = 16;
    faces[11][2] = 17;
    faces[11][3] = 18;
    faces[11][4] = 19;

    // for each point find the three nearest points and constrain
    float l[3];
    Sub3f(vertices[faces[0][1]], vertices[faces[0][0]], l);
    sideLength = sqrt(l[0]*l[0] + l[1]*l[1] + l[2]*l[2]);
    float lf = sideLength * 1.05f;

    for(int i = 0; i < 20; i++)
    {
        int n = 0;
        for(int j = 0; j < 20; j++)
        {
            Sub3f(vertices[i], vertices[j], l);
            float d = sqrt(l[0]*l[0] + l[1]*l[1] + l[2]*l[2]);
            if(d < lf)
            {
                // add constraint
                constraints[i][n] = j;
                n++;
            }
            if(n == 3) break;
        }    
    }
}

void objDodecahedron::render()
{
    renWireframe();
}

void objDodecahedron::renWireframe()
{
    float color[4];
	glGetFloatv(GL_CURRENT_COLOR, color);

    gl->Color(1.0f, 1.0f, 1.0f, 1.0f);
    gl->Begin(GL_LINES);
    for(int i = 0; i < 12; i++)
    {
        for(int j = 0; j < 5; j++)
        {
            gl->Vertex(vertices[faces[i][j]][0],vertices[faces[i][j]][1], vertices[faces[i][j]][2]);
            gl->Vertex(vertices[faces[i][(j+1)%5]][0],vertices[faces[i][(j+1)%5]][1], vertices[faces[i][(j+1)%5]][2]);
        }
    }
    gl->End();

    glColor4fv(color);
}