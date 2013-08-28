/* glTextureManager class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#ifndef __GLTEXTUREMANAGER_H
#define __GLTEXTUREMANAGER_H

class jEngine;
class glSystem;
class glTexture;

class textureFileLink
{
public:
	wchar_t*			textureName;
	wchar_t*			fileName;
	glTexture*			ptr;

	textureFileLink(const wchar_t* name, const wchar_t* path);
	~textureFileLink();
};

class glTextureManager
{
private:
	// pointer to top-level class
	jEngine*	e;
	glSystem*	gl;

	// pointer to single instance
	static glTextureManager* p;

    glTexture**             textures;
    int                     numTextures;

    textureFileLink**	    textureFileLinks;
	unsigned int		    numTFL;

	// make constructors and assignment private to enforce single instance
	glTextureManager(jEngine* j);
	glTextureManager(const glTextureManager& a);
	glTextureManager& operator=(const glTextureManager& a);
public:
	~glTextureManager();

	// return pointer to our single instance, or create it
	static glTextureManager* getPointer(jEngine* j);

    bool isLoaded();

    void buildTextureList();

    glTexture* addTexture(const wchar_t* name);
};

#endif