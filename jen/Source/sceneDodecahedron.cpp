/* a gameScene derived class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#include "sceneDodecahedron.h"

#include "gameSystem.h"

#include <time.h>
#include <math.h>

#ifndef PI
#define PI 3.1415926535897932384626433832795f
#endif
#define PHIA 0.91843825753316954191056421394367f
#define PHIB 0.18871054084413369665112800937666f
#define DEG72 1.2566370614359172953850573533118f
#define DEG36 0.6283185307179586476925286766559f

#define RADIUS 1.0f

#define RGBA(r,g,b,a) (((unsigned int)a) << 24) | (((unsigned int)b) << 16) | (((unsigned int)g) << 8) | ((unsigned int)r)

littleParticleSystem::littleParticleSystem()
{
    time_t t;
    time(&t);
    srand((unsigned int)t);
    
    for(int i = 0; i < SCDH_NUM_PARTICLES; i++)
    {
        x[i][0] = (rand() % 201) * 0.01f - 1.0f;
        x[i][1] = (rand() % 201) * 0.01f - 1.0f;
        x[i][2] = (rand() % 201) * 0.01f - 1.0f;
        float d = (rand() % 51) * 0.01f/sqrt(x[i]*x[i]);
        x[i] *= d;
    }
}

void littleParticleSystem::cycle(float dt)
{
    static float t = 0.0f;
    t+=dt;

    attractor[0] = 0.3f*sin(t); attractor[1] = 0.3f*cos(t); attractor[2] = 0.0f;
    emitter[0] = 0.3f*cos(t); emitter[1] = 0.0f; emitter[2] = 0.3f*sin(t);

    axis[0] = sin(1.5f*t); axis[1] = cos(0.7f*t); axis[2] = sin(t);
    float d = 1/sqrt(axis*axis);
    axis *= d;

    float k = 5.0f*dt;
    float et = pow(.89f, 2.0f*dt);
    float ft = pow(1.02f, 2.0f*dt);

    for(int i = 0; i < SCDH_NUM_PARTICLES; i++)
    {
        x[i] -= attractor;

        // rotate a little
        float ls = sqrt(x[i]*x[i]);
        float d = 1/ls;
        ls *= et;

        x[i] *= d;

        float nds = axis[0]*x[i][0] + axis[1]*x[i][1] + axis[2]*x[i][2];
        float kt = nds*k/(ls*ls+1);
        float ckt = cos(kt); float skt = sin(kt);
        mVector3d sxaxis;
        sxaxis[0] = x[i][1]*axis[2] - x[i][2]*axis[1];
        sxaxis[1] = x[i][2]*axis[0] - x[i][0]*axis[2];
        sxaxis[2] = x[i][0]*axis[1] - x[i][1]*axis[0];

        x[i] = (x[i]*ckt + axis*nds*(1 - ckt) - sxaxis*skt)*ls;

        x[i] += attractor;

        x[i] -= emitter;

        // rotate a little
        ls = sqrt(x[i]*x[i]);
        d = 1/ls;
        ls *= ft;

        x[i][0] *= d;
        x[i][1] *= d;
        x[i][2] *= d;

        nds = axis*x[i];
        kt = nds*k/(ls*ls+1);
        ckt = cos(kt); skt = sin(kt);
        sxaxis[0] = x[i][1]*axis[2] - x[i][2]*axis[1];
        sxaxis[1] = x[i][2]*axis[0] - x[i][0]*axis[2];
        sxaxis[2] = x[i][0]*axis[1] - x[i][1]*axis[0];

        x[i] = (x[i]*ckt + axis*nds*(1 - ckt) - sxaxis*skt)*ls;

        x[i] += emitter;
    }
}

void littleParticleSystem::render(unsigned int ptex, float blendFactor, float t)
{
    gl->Enable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, ptex);
    gl->Color(0.8f, 0.8f, 0.8f, blendFactor);

    for(int i = 0; i < SCDH_NUM_PARTICLES; i++)
    {
        glPushMatrix();

        /*
        float modelview[16];
        glGetFloatv(GL_MODELVIEW_MATRIX , modelview);

        for(int j = 0; j<3; j+=2) 
        for(int k = 0; k<3; k++)
        {
	        if(j == k) modelview[j*4+k] = 1.0f;
	        else modelview[j*4+k] = 0.0f;
        }

        // set the modelview with no rotations and scaling
        glLoadMatrixf(modelview);
        */

        gl->Translate(x[i][0], x[i][1], x[i][2]);

        gl->Begin(GL_QUADS);
            gl->Vertex(.02f,.02f,0.0f);
            gl->TexCoord(1,0);
            gl->Vertex(.02f,-.02f,0.0f);
            gl->TexCoord(1,1);
            gl->Vertex(-.02f,-.02f,0.0f);
            gl->TexCoord(0,1);
            gl->Vertex(-.02f,.02f,0.0f);
            gl->TexCoord(0,0);
        gl->End();

        gl->PopMatrix();
    }

    gl->Disable(GL_TEXTURE_2D);
}

sceneDodecahedron::sceneDodecahedron()
{
    ps = littleParticleSystem();
    name = new wchar_t[wcslen(L"dodec") + 1];
    name = wcscpy(name, L"dodec");

    // initialise data
    float a = 0.0f;
    for(int i = 0; i < 5; i++)
    {
        vertices[i][0] = RADIUS*cos(a)*cos(PHIA);
        vertices[i][1] = RADIUS*sin(a)*cos(PHIA);
        vertices[i][2] = RADIUS*sin(PHIA);
        a = a + DEG72;
    }
    a = 0.0f;
    for(int i = 5; i < 10; i++)
    {
        vertices[i][0] = RADIUS*cos(a)*cos(PHIB);
        vertices[i][1] = RADIUS*sin(a)*cos(PHIB);
        vertices[i][2] = RADIUS*sin(PHIB);
        a = a + DEG72;
    }
    a = DEG36;
    for(int i = 10; i < 15; i++)
    {
        vertices[i][0] = RADIUS*cos(a)*cos(-PHIB);
        vertices[i][1] = RADIUS*sin(a)*cos(-PHIB);
        vertices[i][2] = RADIUS*sin(-PHIB);
        a = a + DEG72;
    }
    a = DEG36;
    for(int i = 15; i < 20; i++)
    {
        vertices[i][0] = RADIUS*cos(a)*cos(-PHIA);
        vertices[i][1] = RADIUS*sin(a)*cos(-PHIA);
        vertices[i][2] = RADIUS*sin(-PHIA);
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
}

void sceneDodecahedron::onInit()
{
    ps.gl = gl;
    unsigned int data[64];
    for(int i = 0; i < 64; i++)
    {
        float x = (float)((i / 8) - 4) * 0.5f + 0.25f;
        float y = (float)((i % 8) - 4) * 0.5f + 0.25f;
        float d = min(1.0f,max(1.4f - (x*x + y*y), 0.0f));
        data[i] = RGBA(192,224,255,(unsigned int)(255*d));
    }

    gl->Enable(GL_TEXTURE_2D);

    glGenTextures(1, &ptex);
    glBindTexture(GL_TEXTURE_2D, ptex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 8, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    gl->Disable(GL_TEXTURE_2D);
}

void sceneDodecahedron::onShutdown()
{
    glDeleteTextures(1, &ptex);
}

void sceneDodecahedron::onRender()
{
    // clear screen that works well with blending
    gl->Begin(GL_QUADS);
        gl->Color(0.3f, 0.5f, 0.7f, blendFactor);
        gl->Vertex(1,1);
        gl->Color(0.1f, 0.2f, 0.4f, blendFactor);
        gl->Vertex(1,-1);
        gl->Color(0.0f, 0.1f, 0.2f, blendFactor);
        gl->Vertex(-1,-1);
        gl->Color(0.1f, 0.2f, 0.4f, blendFactor);
        gl->Vertex(-1,1);
    gl->End();

    gl->MatrixMode(GL_PROJECTION);
    gl->LoadIdentity();
    gl->Perspective(60.0f, gls->getAspectRatio());
    gl->MatrixMode(GL_MODELVIEW);
    gl->LoadIdentity();
    gl->Clear(GL_DEPTH_BUFFER_BIT);

    static float t = 0.0f;
    if((g->isBloomRender() && g->isBloomed()) || !(g->isBloomed()))
    {
        t += gls->getLastDT();
        ps.cycle(gls->getLastDT());
    }

    gl->Disable(GL_DEPTH_TEST);

    gl->Translate(0.8f,0.0f,-2.7f);

    // render particles
    ps.render(ptex, blendFactor, t);

    gl->Rotate(45.0f*t,1.0f,0.0f,0.0f);
    gl->Rotate(45.0f*t,0.0f,1.0f,1.0f);

    gl->Color(1.0f, 1.0f, 1.0f, blendFactor);
    /*
	// this is an ugly wireframe dodecahedron... not really needed
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
	*/

    gl->Enable(GL_DEPTH_TEST);
}