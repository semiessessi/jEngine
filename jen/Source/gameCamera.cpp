/* gameCamera class by Semi Essessi
 *
 * (class description goes here)
 *
 */
#include "gameCamera.h"

#include "glSystem.h"
#include "jEngine.h"
#include "jMath.h"


void gameCamera::glTransforms()
{
    static glSystem* gls = jEngine::getPointer()->gl;
    static glSystem::glFuncs* gl = gls->f;
    
    gl->MatrixMode(GL_PROJECTION);
    gl->LoadIdentity();
    gl->Perspective(60.0f, gls->getAspectRatio());
    gl->MatrixMode(GL_MODELVIEW);
    gl->LoadIdentity();
	
    gl->Translate(pos[0],0.0f,z);
    gl->Rotate(rx,1.0f,0.0f,0.0f);

    gl->Translate(0.0f,0.0f,pos[2]);
	gl->Rotate(ry,0.0f,1.0f,0.0f);

    gl->Translate(0.0f, -pos[1], 0.0f);
}

void gameCamera::glRotates()
{
    static glSystem* gls = jEngine::getPointer()->gl;
    static glSystem::glFuncs* gl = gls->f;
    
    gl->MatrixMode(GL_PROJECTION);
    gl->LoadIdentity();
    gl->Perspective(60.0f, gls->getAspectRatio());
    gl->MatrixMode(GL_MODELVIEW);
    gl->LoadIdentity();
	
    //gl->Translate(pos[0],0.0f,z);
    gl->Rotate(rx,1.0f,0.0f,0.0f);

    //gl->Translate(0.0f,0.0f,pos[2]);
	gl->Rotate(ry,0.0f,1.0f,0.0f);

    //gl->Translate(0.0f, -pos[1], 0.0f);
}

void gameCamera::incrementDistance(const float &speed)
{
    z -= speed;
}

void gameCamera::decrementDistance(const float &speed)
{
    z += speed;
}