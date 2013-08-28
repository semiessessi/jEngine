/* glTexture class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#include "glTexture.h"
#include "jEngine.h"

#include "glSystem.h"
#include "glTextureStage.h"

glTexture::glTexture(const wchar_t* str)
{
	// constructor code for glTexture
	e = jEngine::getPointer();
	gl = e->gl;

	stages = 0;
	numStages = 0;

    int l = (int)wcslen(str) + 1;
    name = new wchar_t[l];
    name = wcscpy(name, str);
}

glTexture::~glTexture()
{
	// destructor code for glTexture
	if(stages) free(stages);
    delete[] name;
}

void glTexture::addStage(glTextureStage* s)
{
	numStages++;
	stages = (glTextureStage**)realloc(stages,numStages*sizeof(glTextureStage*));
}

void glTexture::bindStage(int stage)
{
	stages[stage]->bindTextures();
}

void glTexture::blendFunc(int stage)
{
	stages[stage]->blendFunc();
}