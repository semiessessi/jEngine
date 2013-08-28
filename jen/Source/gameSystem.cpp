/* gameSystem class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#include "jEngine.h"

#include "gameSystem.h"
#include "glSystem.h"

#include "gameScene.h"
#include "gameMission.h"

#include "console.h"
#include "cVar.h"
#include "jTimer.h"

#include <io.h>
#include <stdio.h>

#define BLOOM_SIZE  512
#define GAUSS_MUL   1.1f

gameSystem* gameSystem::p = 0;
float gameSystem::fac = 0.0f;

float gameSystem::gaussianCoefficients[] = { //GAUSS_MUL*0.008933913f,
                                             GAUSS_MUL*0.039061797f,
                                             GAUSS_MUL*0.111907377f,
                                             GAUSS_MUL*0.21032738f,
                                             GAUSS_MUL*0.259539066f,
                                             GAUSS_MUL*0.21032738f,
                                             GAUSS_MUL*0.111907377f,
                                             GAUSS_MUL*0.039061797f
                                             //GAUSS_MUL*0.008933913f
                                            };


gameSystem::gameSystem(jEngine* j)
{
	// constructor code for gameSystem
	e=j;

    scenes = 0;
    numScenes = 0;

    currentScene = 0;
    previousScene = 0;

    bloom = 0;
    bTex = 0;
    bRen = false;

    time = 0.0f;

    missions = 0;
    numMissions = 0;
}

gameSystem::gameSystem(const gameSystem& a)
{
	// copy constructor code for gameSystem
}

gameSystem& gameSystem::operator=(const gameSystem& a)
{
	// since we only want a single instance always return this 
	return (*this);
}

gameSystem::~gameSystem()
{
	// destructor code for gameSystem
    for(int i = 0; i < numScenes; i++) delete scenes[i];
    if(scenes) free(scenes);
}

gameSystem* gameSystem::getPointer(jEngine* j)
{
	// return our instance of gameSystem if its created 
	if(!p) p = new gameSystem(j);
	return p;
}

void gameSystem::addScene(gameScene* s)
{
    if(s)
    {
        numScenes++;
        scenes = (gameScene**)realloc(scenes, sizeof(gameScene*)*numScenes);
        scenes[numScenes-1] = s;
    }
}

void gameSystem::init()
{
    _wfinddata_t f;
    intptr_t hFile = _wfindfirst(L"missions\\*.mission", &f);
    wchar_t* path = (wchar_t*)malloc(2048*sizeof(wchar_t));
	int fl = 0;

    while(true)
	{
        fl = swprintf(path, L"missions\\%s", f.name) + 1;
		path = (wchar_t*)realloc(path, fl*sizeof(wchar_t));

		++numMissions;
        missions[numMissions - 1] = new gameMission(this);
        missions[numMissions - 1]->loadFromFile(path);

        if(_wfindnext(hFile, &f) == -1) break;
    }

    free(path);
}

void gameSystem::initGraphics()
{
    glRenderable::initGraphics();

    // create texture for bloom effect
    unsigned int* data = new unsigned int[BLOOM_SIZE*BLOOM_SIZE];

    memset(data, 0, BLOOM_SIZE*BLOOM_SIZE);

    glEnable(GL_TEXTURE_2D);

    glGenTextures(1, &bTex);
    glBindTexture(GL_TEXTURE_2D, bTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, BLOOM_SIZE, BLOOM_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    delete[] data;

    glDisable(GL_TEXTURE_2D);

    // set up scenes
    for(int i = 0; i < numScenes; i++) scenes[i]->initGraphics();
    fac = time = 0.0f;

    // get bloom variable
    bloom = (bool*)e->con->getVariable(L"rBloom")->getPointer();
}

void gameSystem::shutdownGraphics()
{
    for(int i = 0; i < numScenes; i++) scenes[i]->shutdownGraphics();

    // clean up bloom texture
    glDeleteTextures(1, &bTex);
    glRenderable::shutdownGraphics();
}

void gameSystem::render()
{
    time += e->timer->getLastDT();
    // TODO::need to make this use controlable timings, use menu etc..
    if(time < 2.0f) fac = 0.5f*time;
    else fac = 1.01f;

    gl->ClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    gl->ClearDepth(1500.0f);

    if(*bloom) renBloom();
    else renPlain();

    glBindTexture(GL_TEXTURE_2D, 0);
    gl->Disable(GL_TEXTURE_2D);
}

void gameSystem::renBloom()
{
    // clear screen and render to 512x512 first
    int foo[4] = { 0,0,0,0 };
    glGetIntegerv(GL_VIEWPORT, foo);
    glViewport(0, 0, BLOOM_SIZE, BLOOM_SIZE);
    gl->Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    gl->Enable(GL_DEPTH_TEST);

    gl->Enable(GL_BLEND);
    gl->BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindTexture(GL_TEXTURE_2D, 0);
    gl->Disable(GL_TEXTURE_2D);

    gl->MatrixMode(GL_PROJECTION);
    gl->LoadIdentity();
    gl->Ortho(-1,1,-1,1,-1,1);
    gl->MatrixMode(GL_MODELVIEW);
    gl->LoadIdentity();

    bRen = true;
    if(scenes)
    {
        if(fac < 1.0f)
        {
            scenes[previousScene]->render();
            gl->Clear(GL_DEPTH_BUFFER_BIT);

            gl->Enable(GL_BLEND);
            gl->BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glBindTexture(GL_TEXTURE_2D, 0);
            gl->Disable(GL_TEXTURE_2D);

            gl->MatrixMode(GL_PROJECTION);
            gl->LoadIdentity();
            gl->Ortho(-1,1,-1,1,-1,1);
            gl->MatrixMode(GL_MODELVIEW);
            gl->LoadIdentity();
        }
        scenes[currentScene]->render();
    }

    // copy texture
    gl->Enable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, bTex);
    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, BLOOM_SIZE, BLOOM_SIZE, 0);

    gl->Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    gl->MatrixMode(GL_PROJECTION);
    gl->LoadIdentity();
    gl->Ortho(-1,1,-1,1,-1,1);
    gl->MatrixMode(GL_MODELVIEW);
    gl->LoadIdentity();
    
    gl->Enable(GL_BLEND);
    gl->BlendFunc(GL_SRC_ALPHA, GL_ONE);
    // horizontal gaussian
    gl->Disable(GL_DEPTH_TEST);
    for(int i = 0; i < 7; i++)
    {
        gl->Color(1.0f, 1.0f, 1.0f, gaussianCoefficients[i]);

        float xoff = (float)(2*i - 6) / (float)BLOOM_SIZE;

        gl->Begin(GL_QUADS);
            gl->Vertex(-1.0f + xoff,-1.0f,0.0f);
            gl->TexCoord(1,0);
            gl->Vertex(1.0f + xoff,-1.0f,0.0f);
            gl->TexCoord(1,1);
            gl->Vertex(1.0f + xoff,1.0f,0.0f);
            gl->TexCoord(0,1);
            gl->Vertex(-1.0f + xoff,1.0f,0.0f);
            gl->TexCoord(0,0);
        gl->End();
    }

    // copy texture
    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, BLOOM_SIZE, BLOOM_SIZE, 0);

    gl->Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(int i = 0; i < 7; i++)
    {
        gl->Color(1.0f, 1.0f, 1.0f, gaussianCoefficients[i]);

        float yoff = (float)(2*i - 6) / (float)BLOOM_SIZE;

        gl->Begin(GL_QUADS);
            gl->Vertex(-1.0f,-1.0f + yoff,0.0f);
            gl->TexCoord(1,0);
            gl->Vertex(1.0f,-1.0f + yoff,0.0f);
            gl->TexCoord(1,1);
            gl->Vertex(1.0f,1.0f + yoff,0.0f);
            gl->TexCoord(0,1);
            gl->Vertex(-1.0f,1.0f + yoff,0.0f);
            gl->TexCoord(0,0);
        gl->End();
    }

    // copy texture
    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, BLOOM_SIZE, BLOOM_SIZE, 0);

    glViewport(foo[0], foo[1], foo[2], foo[3]);

    gl->Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    gl->Enable(GL_DEPTH_TEST);

    gl->Enable(GL_BLEND);
    gl->BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindTexture(GL_TEXTURE_2D, 0);
    gl->Disable(GL_TEXTURE_2D);

    gl->MatrixMode(GL_PROJECTION);
    gl->LoadIdentity();
    gl->Ortho(-1,1,-1,1,-1,1);
    gl->MatrixMode(GL_MODELVIEW);
    gl->LoadIdentity();

    bRen = false;
    // actual render
    if(scenes)
    {
        if(fac < 1.0f)
        {
            scenes[previousScene]->render();
            gl->Clear(GL_DEPTH_BUFFER_BIT);

            gl->Enable(GL_BLEND);
            gl->BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glBindTexture(GL_TEXTURE_2D, 0);
            gl->Disable(GL_TEXTURE_2D);

            gl->MatrixMode(GL_PROJECTION);
            gl->LoadIdentity();
            gl->Ortho(-1,1,-1,1,-1,1);
            gl->MatrixMode(GL_MODELVIEW);
            gl->LoadIdentity();
        }
        scenes[currentScene]->render();
    }

    gl->Enable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, bTex);

    gl->Enable(GL_BLEND);
    gl->BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    gl->MatrixMode(GL_PROJECTION);
    gl->LoadIdentity();
    gl->Ortho(-1,1,-1,1,-1,1);
    gl->MatrixMode(GL_MODELVIEW);
    gl->LoadIdentity();

    // render bloom
    glClear(GL_DEPTH_BUFFER_BIT);
    gl->Color(1.0f, 1.0f, 1.0f, 0.5f);

    gl->Begin(GL_QUADS);
        gl->Vertex(-1,-1,0);
        gl->TexCoord(1,0);
        gl->Vertex(1,-1,0);
        gl->TexCoord(1,1);
        gl->Vertex(1,1,0);
        gl->TexCoord(0,1);
        gl->Vertex(-1,1,0);
        gl->TexCoord(0,0);
    gl->End();
}

void gameSystem::renPlain()
{
    gl->Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    gl->Enable(GL_DEPTH_TEST);

    gl->Enable(GL_BLEND);
    gl->BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindTexture(GL_TEXTURE_2D, 0);
    gl->Disable(GL_TEXTURE_2D);

    gl->MatrixMode(GL_PROJECTION);
    gl->LoadIdentity();
    gl->Ortho(-1,1,-1,1,-1,1);
    gl->MatrixMode(GL_MODELVIEW);
    gl->LoadIdentity();

    bRen = false;
    // actual render
    if(scenes)
    {
        if(fac < 1.0f)
        {
            scenes[previousScene]->render();
            gl->Clear(GL_DEPTH_BUFFER_BIT);

            gl->Enable(GL_BLEND);
            gl->BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glBindTexture(GL_TEXTURE_2D, 0);
            gl->Disable(GL_TEXTURE_2D);

            gl->MatrixMode(GL_PROJECTION);
            gl->LoadIdentity();
            gl->Ortho(-1,1,-1,1,-1,1);
            gl->MatrixMode(GL_MODELVIEW);
            gl->LoadIdentity();
        }
        scenes[currentScene]->render();
    }
}

int gameSystem::getSceneFromName(wchar_t* n)
{
    for(int i = 0; i < numScenes; i++)
    {
        if(scenes[i]->isNamed(n)) return i;
    }
    return 0;
}

void gameSystem::ccomSwitchScene(const wchar_t* cl)
{
    wchar_t* str = 0;
	str = console::strGetParam(str, cl, 1);
	if(!str) console::ccomHelp(L"help switchscene");
	else
    {
        p->previousScene = p->currentScene;
        p->currentScene = p->getSceneFromName(str);

        p->scenes[p->previousScene]->blendedIn = true;
        p->scenes[p->currentScene]->blendedIn = false;

        p->time = 0;

        p->scenes[p->currentScene]->onSwitch();
    }
    free(str);
}