/* glTextureManager class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#include "glTextureManager.h"
#include "jEngine.h"

#include "glSystem.h"
#include "glTexture.h"

#include "console.h"
#include "jProfiler.h"
#include "Scanner.h"
#include "strSystem.h"

#include <io.h>
#include <stdio.h>
#include <wchar.h>

const wchar_t defaultStr[] = L"default\r\n{\r\n\tdiffuse\r\n\t{\r\n\t\tmap checkerboard 512 512 1 0 0 0 1 1 1 1 1\r\n\t\tborder 4 1 0 1 1\r\n\t}\r\n}";

glTextureManager* glTextureManager::p = 0;

glTextureManager::glTextureManager(jEngine* j)
{
	// constructor code for glTextureManager
	e=j;
	gl=e->gl;

    textures = 0;
    numTextures = 0;

    textureFileLinks = 0;
    numTFL = 0;
}

glTextureManager::glTextureManager(const glTextureManager& a)
{
	// copy constructor code for glTextureManager
}

glTextureManager& glTextureManager::operator=(const glTextureManager& a)
{
	// since we only want a single instance always return this 
	return (*this);
}

glTextureManager::~glTextureManager()
{
	// destructor code for glTextureManager
    for(int i = 0; i < numTextures; i++) delete textures[i];
    if(textures) free(textures);
}

glTextureManager* glTextureManager::getPointer(jEngine* j)
{
	// return our instance of glTextureManager if its created 
	if(!p) p = new glTextureManager(j);
	return p;
}

void glTextureManager::buildTextureList()
{
#ifdef _DEBUG
	jProfiler profiler = jProfiler(L"glTextureManager::buildTextureList");
#endif

    _wfinddata_t f;
	intptr_t hFile;

	int count = 0;

	//TODO::look in mod directory
	hFile = _wfindfirst(L"\\graphics\\textures\\*.texture", &f);

	while(hFile!=-1L)
	{
		count++;
        bool err = false;
		
		wchar_t* path = (wchar_t*)malloc(2048*sizeof(wchar_t));
		int fl = swprintf(path, L"textures\\%s", f.name) + 1;
		path = (wchar_t*)realloc(path, fl*sizeof(wchar_t));
		
		FILE* file = _wfopen(path,L"rb");

		// get length of file
		fseek(file, 0L, SEEK_END);
		int lenData = (ftell(file))/sizeof(wchar_t);

		// dont read bom
		fseek(file, 2L, SEEK_SET);

		wchar_t* data = (wchar_t*)malloc(sizeof(wchar_t)*(lenData));
		fread(data, sizeof(wchar_t), lenData, file);
		data[lenData-1] = 0;
		fclose(file);

        // do something with the data
        // do rough conversion to ascii from unicode
        char* script = new char[lenData + 1];
        for(int i = 0; i < lenData; i++)
        {
            script[i] = ((unsigned short) data[i] < 255) ? (char)data[i] : 255;
        }

        parse(script);

        delete[] script;
		if(path) free(path);
		if(data) free(data);

        int retval = _wfindnext(hFile, &f);
		if(retval == -1) break;
    }

    wchar_t* ret = (wchar_t*)malloc(16*sizeof(wchar_t));
	int len = swprintf(ret, L"%i", count) + 1;
	ret = (wchar_t*)realloc(ret, len*sizeof(wchar_t));

	wchar_t* tret = (wchar_t*)malloc(16*sizeof(wchar_t));
	len = swprintf(tret, L"%i", numTFL) + 1;
	tret = (wchar_t*)realloc(tret, len*sizeof(wchar_t));

    wchar_t* rstr = e->str->getString(L"%1 texture file(s) succesfully loaded, %2 texture(s) found", ret, tret);
	e->con->writeLine(rstr);

    free(rstr);
	free(ret);
    free(tret);

    _findclose(hFile);
}

textureFileLink::textureFileLink(const wchar_t* name, const wchar_t* path)
{
	textureName = new wchar_t[wcslen(name)+1];
	fileName = new wchar_t[wcslen(path)+1];

	textureName = wcscpy(textureName, name);
	fileName = wcscpy(fileName, path);

	ptr = 0;
}

textureFileLink::~textureFileLink()
{
	if(textureName) delete textureName;
	if(fileName) delete fileName;

	if(ptr) delete ptr;
}

bool glTextureManager::isLoaded()
{
    return (numTextures > 0);
}

glTexture* glTextureManager::addTexture(const wchar_t* name)
{
    if(name)
    {
        glTexture* newTexture = new glTexture(name);
        ++numTextures;
        textures = (glTexture**)realloc(textures, sizeof(glTexture*)*numTextures);
        textures[numTextures-1] = newTexture;
        return newTexture;
    }

    return 0;
}