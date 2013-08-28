/* glTriangleList2D class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#include "glTriangleList2D.h"
#include "jEngine.h"

#include "glSystem.h"
#include "glTexture.h"
#include "glTextureManager.h"

#include "jMath.h"
#include "log.h"

glTriangleList2D::glTriangleList2D(bool texcoords) : glRenderable()
{
	// constructor code for glTriangleList2D
	e = jEngine::getPointer();

	useTexCoords = texcoords;

	col[0] = 1.0f;
	col[1] = 1.0f;
	col[2] = 1.0f;
	col[3] = 1.0f;

	numTris = 0;

	vData = 0;
	tData = 0;

	tex = 0;

	vi = 0;
	ti = 0;

	incomplete = false;

	setUseTexCoords(useTexCoords);
}

glTriangleList2D::~glTriangleList2D()
{
	// destructor code for glTriangleList2D
	if(vData) free(vData);
	if(tData) free(tData);
}

void glTriangleList2D::initGraphics()
{
	glRenderable::initGraphics();
}

void glTriangleList2D::shutdownGraphics()
{
	glRenderable::shutdownGraphics();
}

void glTriangleList2D::setUseTexCoords(bool b)
{
	useTexCoords = b;
	_r = b ? __rvt : __rv;
}

void glTriangleList2D::render()
{
	_r(this);
}

void glTriangleList2D::___rv()
{
    glSystem::glFuncs* gl = e->gl->f;

	int	vp[2] = {0,1};
	int	vinc[2] = {2,2};

	gl->PushAttrib(GL_ENABLE_BIT);

	gl->Disable(GL_TEXTURE_2D);
	gl->Enable(GL_BLEND);
	gl->BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	gl->Color(col[0],col[1],col[2],col[3]);

	gl->Begin(GL_TRIANGLES);
		for(int i=0;i<numTris*3;i++)
		{
			gl->Vertex(vData[vp[0]], vData[vp[1]]);
			Add2i(vp,vinc,vp);
		}
	gl->End();

	gl->PopAttrib();

}

void glTriangleList2D::___rvt()
{
    glSystem::glFuncs* gl = e->gl->f;

	int	vp[2] = {0,1};
	int	vinc[2] = {2,2};

	gl->PushAttrib(GL_ENABLE_BIT);

	gl->Enable(GL_TEXTURE_2D);
	gl->Enable(GL_BLEND);
	gl->BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	gl->Color(col[0],col[1],col[2],col[3]);

	gl->Begin(GL_TRIANGLES);
		for(int i=0;i<numTris;i++)
		{
			//TODO::add texturing
			gl->Vertex(vData[vp[0]], vData[vp[1]]);
            gl->TexCoord(tData[vp[0]], tData[vp[1]]);
			Add2i(vp,vinc,vp);
		}
	gl->End();

	gl->PopAttrib();
}

void glTriangleList2D::__rv(glTriangleList2D* p)
{
	p->___rv();
}

void glTriangleList2D::__rvt(glTriangleList2D* p)
{
	p->___rvt();
}


void glTriangleList2D::addVertex(float x, float y)
{
	int n = vi%3;
	if(!n)
	{
		if(vi+1>ti) numTris++;
		incomplete = !(vi==ti);
	} else if(n==2)
	{
		incomplete = false;
	}

	vi++;
	vData = (float*)realloc(vData,sizeof(float)*2*vi);
	
	vi = 2*(vi-1);
	vData[vi] = x;
	vData[vi+1] = -y;
	vi = (vi>>1)+1;
}

void glTriangleList2D::addTexCoord(float x, float y)
{
	int n = ti%3;
	if(!n)
	{
		if(ti+1>vi) numTris++;
		incomplete = !(vi==ti);
	} else if(n==2)
	{
		incomplete = false;
	}

	ti++;
	tData = (float*)realloc(tData,sizeof(float)*2*ti);
	
	ti = 2*(ti-1);
	tData[ti] = x;
	tData[ti+1] = y;
	ti = (ti>>1)+1;
}
