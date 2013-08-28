/* glShader class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#ifndef __GLSHADER_H
#define __GLSHADER_H

#include "glSystem.h"

class glShader
{
private:
	// pointer to top-level class
	jEngine*			e;
	glSystem*			gls;
    glSystem::glFuncs*  gl;

    unsigned int        prog;
    unsigned int        frag;
    unsigned int        vert;

public:
	glShader(const wchar_t* fsFile, const wchar_t* vsFile);
	~glShader();

    void Use();
};

#endif