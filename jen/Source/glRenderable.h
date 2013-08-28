/* glRenderable class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#ifndef __GLRENDERABLE_H
#define __GLRENDERABLE_H

#include "glSystem.h"

class glRenderable
{
protected:
	glSystem*           gls;
    glSystem::glFuncs*  gl;
public:
	glRenderable();
	virtual ~glRenderable();

	virtual void initGraphics();
	virtual void shutdownGraphics();

	virtual void render();
};

#endif