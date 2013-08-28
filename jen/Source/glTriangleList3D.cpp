/* glTriangleList3D class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#include "glTriangleList3D.h"
#include "jEngine.h"

#include "glSystem.h"
#include "glTexture.h"
#include "glTextureManager.h"

#include "jMath.h"
#include "log.h"

glTriangleList3D::glTriangleList3D(bool normals, bool texcoords, bool exnormals) : glRenderable()
{
	// constructor code for glTriangleList3D
	e = jEngine::getPointer();

	useNormals = normals;
	useNormalsEx = exnormals;
	useTexCoords = texcoords;

	col[0] = 1.0f;
	col[1] = 1.0f;
	col[2] = 1.0f;
	col[3] = 1.0f;

	numTris = 0;

	vData = 0;
	nData = 0;
	nbData = 0;
	ntData = 0;
	tData = 0;

	tex = 0;

	vi = 0;
	ni = 0;
	ti = 0;

	incomplete = false;

	setUseNormals(useNormals);
	setUseTexCoords(useTexCoords);
	setUseNormalsEx(useNormalsEx);
}

glTriangleList3D::~glTriangleList3D()
{
	// destructor code for glTriangleList3D
	if(vData) free(vData);
	if(nData) free(nData);
	if(nbData) free(nbData);
	if(ntData) free(ntData);
	if(tData) free(tData);
}

void glTriangleList3D::initGraphics()
{
	glRenderable::initGraphics();
}

void glTriangleList3D::shutdownGraphics()
{
	glRenderable::shutdownGraphics();
}

void glTriangleList3D::setUseNormals(bool b)
{
	if(b)
	{
		useNormals = true;
		if(useTexCoords)
		{
			_r = useNormalsEx ? __rvntx : __rvnt;
		}
		else
		{
			_r = __rvn;
		}
	}
	else
	{
		useNormals = false;

		if(useTexCoords)
		{
			_r = __rvt;
		}
		else
		{
			_r = __rv;
		}
	}
}

void glTriangleList3D::setUseTexCoords(bool b)
{
	if(b)
	{
		useTexCoords = true;
		if(useNormals)
		{
			_r = useNormalsEx ? __rvntx : __rvnt;
		}
		else
		{
			_r = __rvt;
		}
	}
	else
	{
		useTexCoords = false;

		if(useNormals)
		{
			_r = __rvn;
		}
		else
		{
			_r = __rv;
		}
	}
}

void glTriangleList3D::setUseNormalsEx(bool b)
{
	if(b && useNormals && useTexCoords)
	{
		useNormalsEx = true;
		_r = __rvntx;
	}
	else
	{
		useNormalsEx = false;

		if(useNormals)
		{
			_r = useTexCoords ? __rvnt : __rvn;
		}
		else
		{
			_r = useTexCoords ? __rvt : __rv;
		}
	}
}

void glTriangleList3D::render()
{
	_r(this);
}

void glTriangleList3D::___rv()
{
	int	vp[4] = {0,1,2,0};
	int	vinc[4] = {3,3,3,0};

	glBegin(GL_TRIANGLES);
		for(int i=0;i<numTris;i++)
		{
			glVertex3f(vData[vp[0]], vData[vp[1]], vData[vp[2]]);
			Add4i(vp,vinc,vp);
		}
	glEnd();
}

void glTriangleList3D::___rvn()
{
	int	vp[4] = {0,1,2,0};
	int	vinc[4] = {3,3,3,0};

	glBegin(GL_TRIANGLES);
		for(int i=0;i<numTris;i++)
		{
			glVertex3f(vData[vp[0]], vData[vp[1]], vData[vp[2]]);
			Add4i(vp,vinc,vp);
		}
	glEnd();
}

void glTriangleList3D::___rvt()
{
	int	vp[4] = {0,1,2,0};
	int	vinc[4] = {3,3,3,0};

	glBegin(GL_TRIANGLES);
		for(int i=0;i<numTris;i++)
		{
			glVertex3f(vData[vp[0]], vData[vp[1]], vData[vp[2]]);
			Add4i(vp,vinc,vp);
		}
	glEnd();
}

void glTriangleList3D::___rvnt()
{
	int	vp[4] = {0,1,2,0};
	int	vinc[4] = {3,3,3,0};

	glBegin(GL_TRIANGLES);
		for(int i=0;i<numTris;i++)
		{
			glVertex3f(vData[vp[0]], vData[vp[1]], vData[vp[2]]);
			Add4i(vp,vinc,vp);
		}
	glEnd();
}

void glTriangleList3D::___rvntx()
{
	int	vp[4] = {0,1,2,0};
	int	vinc[4] = {3,3,3,0};

	glBegin(GL_TRIANGLES);
		for(int i=0;i<numTris;i++)
		{
			glVertex3f(vData[vp[0]], vData[vp[1]], vData[vp[2]]);
			Add4i(vp,vinc,vp);
		}
	glEnd();
}

void glTriangleList3D::__rv(glTriangleList3D* p)
{
	p->___rv();
}

void glTriangleList3D::__rvn(glTriangleList3D* p)
{
	p->___rvn();
}

void glTriangleList3D::__rvt(glTriangleList3D* p)
{
	p->___rvt();
}

void glTriangleList3D::__rvnt(glTriangleList3D* p)
{
	p->___rvnt();
}

void glTriangleList3D::__rvntx(glTriangleList3D* p)
{
	p->___rvntx();
}


void glTriangleList3D::addVertex(float x, float y, float z)
{
	int n = vi%3;
	if(!n)
	{
		numTris++;
		incomplete = true;
	} else if(n==2)
	{
		incomplete = false;
	}

	vi++;
	vData = (float*)realloc(vData,sizeof(float)*3*vi);
	
	vi = 3*(vi-1);
	vData[vi] = x;
	vData[vi+1] = y;
	vData[vi+2] = z;
	vi = (vi/3)+1;
}

void glTriangleList3D::addNormal(float x, float y, float z)
{
	int n = ni%3;
	if(!n)
	{
		numTris++;
		incomplete = true;
	} else if(n==2)
	{
		incomplete = false;
	}

	ni++;
	nData = (float*)realloc(nData,sizeof(float)*3*ni);
	
	ni = 3*(ni-1);
	nData[ni] = x;
	nData[ni+1] = y;
	nData[ni+2] = z;
	ni = (ni/3)+1;
}

void glTriangleList3D::addTexCoord(float x, float y)
{
}

void glTriangleList3D::calcNormalsEx()
{
}