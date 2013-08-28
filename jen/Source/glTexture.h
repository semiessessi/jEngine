/* glTexture class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#ifndef __GLTEXTURE_H
#define __GLTEXTURE_H

class jEngine;
class glSystem;

class glTextureStage;

class glTexture
{
private:
	// pointer to top-level class
	jEngine*			e;
	glSystem*			gl;

	glTextureStage**	stages;
	int					numStages;

    wchar_t*            name;
    unsigned int        id;

	void addStage(glTextureStage* s);
public:
	glTexture(const wchar_t* str);
	~glTexture();

	int getNumStages() { return numStages; }

	void bindStage(int stage);
	void blendFunc(int stage);
};

#endif