/* glRenderable class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#include "glRenderable.h"
#include "jEngine.h"

#include "glSystem.h"

glRenderable::glRenderable()
{
	// constructor code for glRenderable
}

glRenderable::~glRenderable()
{
	// destructor code for glRenderable
}

void glRenderable::initGraphics()
{
	gls = glSystem::getPointer(jEngine::getPointer());
    gl = gls->f;
}

void glRenderable::shutdownGraphics()
{

}

void glRenderable::render()
{
}