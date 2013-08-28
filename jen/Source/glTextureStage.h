/* glTextureStage class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#ifndef __GLTEXTURESTAGE_H
#define __GLTEXTURESTAGE_H

class jEngine;
class glSystem;

class glSubTexture;

class glTextureStage
{
private:
	// pointer to top-level class
	jEngine*		e;
	glSystem*		gl;
									// <rgb>+<alpha>
	glSubTexture*	diffuse;		// diffuse+transparency
	glSubTexture*	normal;			// normal+height
	glSubTexture*	specular;		// gloss+specular
	glSubTexture*	ambient;		// ambient+occlusion

	// blend func params
	unsigned int	bfSrc;
	unsigned int	bfDst;

public:
	glTextureStage(jEngine* j);
	~glTextureStage();

	void bindTextures();
	void blendFunc();
};

#endif