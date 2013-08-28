// 3d model file format helper class by Semi Essessi

#ifndef J_MODEL_H
#define J_MODEL_H

#define JMODEL_DEFAULT			(unsigned int)0
#define JMODEL_EDGEOPT			(unsigned int)1
#define JMODEL_FACENORMALS		(unsigned int)2
#define JMODEL_MATERIALS		(unsigned int)4
#define JMODEL_TEXCOORDS		(unsigned int)8
#define JMODEL_NBASIS			(unsigned int)16
#define JMODEL_EXTFLAGS			(unsigned int)(1 << 32)

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#pragma comment(lib, "opengl32.lib")

#include <windows.h>
#include <gl\gl.h>

struct jm_vertex
{
	float x;
	float y;
	float z;
};

struct jm_normal
{
	float x;
	float y;
	float z;
};

struct jm_basis
{
	jm_normal tan;
	jm_normal bin;
};

struct jm_texcoord
{
	float u;
	float v;
};

struct jm_material
{
	float dr;
	float dg;
	float db;

	float sr;
	float sg;
	float sb;

	float sn;
};

class jmodel
{
private:
	bool			loaded;

	jm_normal*			nList;
	jm_texcoord*		tList;
	jm_vertex*			vList;

	// num normals, texcoords, vertices
	int				numN;
	int				numT;
	int				numV;
	// num faces
	int				numF;
	// num indices per face
	int				numI;

	// function pointers for SPEED!!!
	int (*nindex)(int nFace, int nNormal);
	int (*tindex)(int nFace, int nTexCoord);
	int (*vindex)(int nFace, int nVertex);

	int*	iList;

	// optimisers for haxpilating... :)
	
	// do nothing (hopefully never gets called ever :/)
	static int nopI(int a, int b) { return 0; }

	// default
	static int nindex0(int a, int b) { return 6*a+b; }
	static int tindex0(int a, int b) { return nopI(a,b); }
	static int vindex0(int a, int b) { return 6*a+b+3; }

	// with texture coords only
	static int nindext(int a, int b) { return 9*a+b; }
	static int tindext(int a, int b) { return 9*a+b+3; }
	static int vindext(int a, int b) { return 9*a+b+6; }

	// with texture coords and face normals
	static int nindextf(int a, int b) { return 7*a; }
	static int tindextf(int a, int b) { return 7*a+b+1; }
	static int vindextf(int a, int b) { return 7*a+b+4; }

	// with face normals only
	static int nindexf(int a, int b) { return 4*a; }
	static int tindexf(int a, int b) { return nopI(a,b); }
	static int vindexf(int a, int b) { return 4*a+b+1; }

public:
	jmodel(char* filename);
	~jmodel();

	bool IsLoaded() { return loaded; }

	int GetNumTris() { return numF; }

	int GetNormalIndex(int nFace, int nNormal) { return iList[nindex(nFace, nNormal)]; }
	int GetTexCoordIndex(int nFace, int nTexCoord){ return iList[tindex(nFace, nTexCoord)]; }
	int GetVertexIndex(int nFace, int nVertex){ return iList[vindex(nFace, nVertex)]; }

	jm_normal* GetNormal(int nFace, int nNormal) { return &(nList[GetNormalIndex(nFace, nNormal)]); }
	jm_texcoord* GetTexCoord(int nFace, int nTexCoord) { return &(tList[GetTexCoordIndex(nFace, nTexCoord)]); }
	jm_vertex* GetVertex(int nFace, int nVertex) { return &(vList[GetVertexIndex(nFace, nVertex)]); }

	void glNormal(int nFace, int nNormal) { glNormal3f(GetNormal(nFace, nNormal)->x,GetNormal(nFace, nNormal)->y,GetNormal(nFace, nNormal)->z); }
	void glTexCoord(int nFace, int nTexCoord) { glTexCoord2f(GetTexCoord(nFace, nTexCoord)->u,GetTexCoord(nFace, nTexCoord)->v); }
	void glVertex(int nFace, int nVertex) { glVertex3f(GetVertex(nFace, nVertex)->x,GetVertex(nFace, nVertex)->y,GetVertex(nFace, nVertex)->z); }

	void glRender();
};

#endif