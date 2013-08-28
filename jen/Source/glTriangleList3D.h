/* glTriangleList3D class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#ifndef __GLTRIANGLELIST3D_H
#define __GLTRIANGLELIST3D_H

#include "glRenderable.h"

class jEngine;

class glSystem;
class glTexture;

class glTriangleList3D : public glRenderable
{
private:
	// pointer to top-level class
	jEngine*	e;

	// vertex parameter data
	float*		vData;			// vertices
	float*		nData;			// normals
	float*		nbData;			// binormals
	float*		ntData;			// tangents
	float*		tData;			// texcoords
	int			numTris;

	int			vi;
	int			ni;
	int			ti;

	float		col[4];

	glTexture*	tex;

	bool		useNormals;
	bool		useNormalsEx;
	bool		useTexCoords;

	// for warning of not enough vertices/normals/texcoords
	bool		incomplete;

	// optimisation junk
	void		(*_r)(glTriangleList3D*);

	static void __rv(glTriangleList3D* p);
	static void __rvn(glTriangleList3D* p);
	static void __rvt(glTriangleList3D* p);
	static void __rvnt(glTriangleList3D* p);
	static void __rvntx(glTriangleList3D* p);
public:
	glTriangleList3D(bool normals, bool texcoords, bool exnormals);
	~glTriangleList3D();

	void setUseNormals(bool b);
	void setUseNormalsEx(bool b);
	void setUseTexCoords(bool b);

	// this is probably not the best way to manage this data
	// but code readability ++
	void addVertex(float x, float y, float z);
	void addNormal(float x, float y, float z);
	void addTexCoord(float x, float y);

	void calcNormalsEx();

	// glRenderable overrides
	void initGraphics();
	void shutdownGraphics();
	void render();

	void ___rv();
	void ___rvn();
	void ___rvt();
	void ___rvnt();
	void ___rvntx(); 
};

#endif