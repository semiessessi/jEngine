/* glTriangleList2D class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#ifndef __GLTRIANGLELIST2D_H
#define __GLTRIANGLELIST2D_H

#include "glRenderable.h"

class jEngine;

class glSystem;
class glTexture;

class glTriangleList2D : public glRenderable
{
private:
	// pointer to top-level class
	jEngine*	e;

	// vertex parameter data
	float*		vData;			// vertices
	float*		tData;			// texcoords
	int			numTris;

	int			vi;
	int			ti;

	float		col[4];

	glTexture*	tex;

	bool		useTexCoords;

	// for warning of not enough vertices/normals/texcoords
	bool		incomplete;

	// optimisation junk
	void		(*_r)(glTriangleList2D*);

	static void __rv(glTriangleList2D* p);
	static void __rvt(glTriangleList2D* p);
public:
	glTriangleList2D(bool texcoords);
	~glTriangleList2D();

	void setUseTexCoords(bool b);

	// this is probably not the best way to manage this data
	// but code readability ++
	void addVertex(float x, float y);
	void addTexCoord(float x, float y);

	float* getColor() { return col; }
	void setColor(float r, float g, float b, float a) { col[0]=r; col[1]=g; col[2]=b; col[3]=a; }

	// glRenderable overrides
	void initGraphics();
	void shutdownGraphics();
	void render();

	void ___rv();
	void ___rvt();
};

#endif