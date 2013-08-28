/* glTextureStage class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#include "glTextureStage.h"
#include "jEngine.h"

#include "glSystem.h"
#include "glSubTexture.h"

glTextureStage::glTextureStage(jEngine* j)
{
	// constructor code for glTextureStage
	e=j;
	gl=e->gl;

	diffuse = 0;
	normal = 0;
	specular = 0;
	ambient = 0;

	bfSrc = GL_ONE;
	bfDst = GL_ZERO;
}

glTextureStage::~glTextureStage()
{
	// destructor code for glTextureStage
	if(diffuse) delete diffuse;
	if(normal) delete normal;
	if(specular) delete specular;
	if(ambient) delete ambient;
}

void glTextureStage::bindTextures()
{
	//TODO::write this, its damned important!
}

void glTextureStage::blendFunc()
{
	glBlendFunc(bfSrc, bfDst);
}