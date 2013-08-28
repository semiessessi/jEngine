/* glShader class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#include <stdio.h>

#include "glShader.h"
#include "jEngine.h"

#include "log.h"

glShader::glShader(const wchar_t* fsFile, const wchar_t* vsFile)
{
	// constructor code for glShader
	e = jEngine::getPointer();
	gls = e->gl;
    gl = e->gl->f;

    prog = vert = frag = 0;

    FILE*   f = 0;
    int     size = 0;
    char*   vb = 0;
    char*   fb = 0;

    if(f = _wfopen(fsFile, L"rb"))
    {
        fseek(f,0,SEEK_END);
        size = ftell(f) + 1;
        fseek(f,0,SEEK_SET);

        fb = (char*)malloc(size*sizeof(char));
        fread(fb, sizeof(char), size, f);
        fb[size-1] = 0;
        fclose(f);
    }
    else
    {
#ifdef _DEBUG
        LogWriteLine(L"Failed to load file %s", fsFile);
#endif
        return;
    }

    if(f = _wfopen(vsFile, L"rb"))
    {
        fseek(f,0,SEEK_END);
        size = ftell(f) + 1;
        fseek(f,0,SEEK_SET);

        vb = (char*)malloc(size*sizeof(char));
        fread(vb, sizeof(char), size, f);
        vb[size-1] = 0;
        fclose(f);
    }
    else
    {
        free(fb);
#ifdef _DEBUG
        LogWriteLine(L"Failed to load file %s", vsFile);
#endif
        return;
    }

    prog = gl->CreateProgram();
    frag = gl->CreateShader(GL_FRAGMENT_SHADER);
    vert = gl->CreateShader(GL_VERTEX_SHADER);

    gl->ShaderSource(frag, 1, &fb, 0);
    gl->ShaderSource(vert, 1, &fb, 0);
    
    char log[1024]; int len = 0;
    
    gl->CompileShader(frag);
    gl->CompileShader(vert);

    wchar_t* filename = 0;

    gl->GetShaderInfoLog(frag, 1023, &len, log);
    //if(len)
    //{
        // dump to file
        filename = (wchar_t*)malloc((wcslen(fsFile)+5)*sizeof(wchar_t));
        wcscpy(filename, fsFile);
        wcscat(filename, L".log");

        log[len] = 0;
        
        if(f = _wfopen(filename, L"wb"))
        {
            fwrite(log, sizeof(char), len, f);
            fclose(f);
        }
        else
        {
#ifdef _DEBUG
            LogWriteLine(L"Failed to load file %s", fsFile);
#endif
        }
        
        free(filename);
        len = 0;
    //}

    gl->GetShaderInfoLog(vert, 1023, &len, log);
    //if(len)
    //{
        // dump to file
        filename = (wchar_t*)malloc((wcslen(vsFile)+5)*sizeof(wchar_t));
        wcscpy(filename, vsFile);
        wcscat(filename, L".log");

        log[len] = 0;
        
        if(f = _wfopen(filename, L"wb"))
        {
            fwrite(log, sizeof(char), len, f);
            fclose(f);
        }
        else
        {
#ifdef _DEBUG
            LogWriteLine(L"Failed to load file %s", fsFile);
#endif
        }
        
        free(filename);
    //}

    gl->AttachShader(prog, frag);
    gl->AttachShader(prog, vert);
    gl->LinkProgram(prog);
    gl->UseProgram(0);

    if(fb) free(fb);
    if(vb) free(vb);
}

glShader::~glShader()
{
	// destructor code for glShader
    if(frag) gl->DeleteShader(frag);
    if(vert) gl->DeleteShader(vert);
    if(prog) gl->DeleteProgram(prog);
    gl->UseProgram(0);
}

void glShader::Use()
{
    gl->UseProgram(prog);
}