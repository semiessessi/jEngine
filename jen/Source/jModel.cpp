// 3d model file format helper class by Semi Essessi

#include <stdlib.h>
#include <stdio.h>
//#include <malloc.h>
//#include <memory.h>

#include "jmodel.h"

jmodel::jmodel(char* filename)
{
	loaded = false;

	nindex = nopI;
	tindex = nopI;
	vindex = nopI;

	FILE* f = fopen(filename, "rb");
	if(!f) return;
	
	char* data = NULL;

	fseek(f,0,SEEK_END);
	int len = ftell(f);
	fseek(f,0,SEEK_SET);

	data = (char*)malloc(len);
	if(!data) return;

	fread(data,sizeof(char),len,f);
	fclose(f);

	if((data[0]!='J')||(data[1]!='M'))
	{
		free(data);
		return;
	}

	int pos = 2;
	unsigned int flags = 0;
	memcpy(&flags,data+pos,sizeof(unsigned int));
	pos+=sizeof(unsigned int);

	bool param_e = (flags & JMODEL_EDGEOPT)!=0;
	bool param_f = (flags & JMODEL_FACENORMALS)!=0;
	bool param_m = (flags & JMODEL_MATERIALS)!=0;
	bool param_t = (flags & JMODEL_TEXCOORDS)!=0;

	// fp setup to avoid a ton of ifs and logic
	if(flags == JMODEL_DEFAULT)
	{
		nindex = nindex0;
		tindex = nopI;
		vindex = vindex0;
	}
	else if(param_f && !param_t)
	{
		nindex = nindexf;
		tindex = nopI;
		vindex = vindexf;
	}
	else if(param_t && !param_f)
	{
		nindex = nindext;
		tindex = tindext;
		vindex = vindext;
	}
	else if(param_t && param_f)
	{
		nindex = nindextf;
		tindex = tindextf;
		vindex = vindextf;
	}

	memcpy(&numF,data+pos,sizeof(int));
	pos+=sizeof(int);

	memcpy(&numN,data+pos,sizeof(int));
	pos+=sizeof(int);

	if(param_t)
	{
		memcpy(&numT,data+pos,sizeof(int));
		pos+=sizeof(int);
	} else numT = 0;

	memcpy(&numV,data+pos,sizeof(int));
	pos+=sizeof(int);

	nList = (jm_normal*)malloc(sizeof(jm_normal)*numN);
	if(numT) tList = (jm_texcoord*)malloc(sizeof(jm_texcoord)*numT);
	else 
	{
		tList = (jm_texcoord*)malloc(sizeof(jm_texcoord));
		tList[0].u = 0;
		tList[0].v = 0;
	}
	vList = (jm_vertex*)malloc(sizeof(jm_vertex)*numN);

	numI = 6;
	if(param_f) numI-=2;
	if(param_t) numI+=3;
	if(param_m) numI++;

	iList = (int*)malloc(sizeof(int)*numF*numI);
	
	if(!(nList && tList && vList)) 
	{
		if(nList) free(nList);
		if(tList) free(tList);
		if(vList) free(vList);
		free(data);
		return;
	}

	// load normals
	memcpy(nList, data+pos, sizeof(jm_normal)*numN);
	pos+=sizeof(jm_normal)*numN;
	
	// load texture coords
	if(numT)
	{
		memcpy(tList, data+pos, sizeof(jm_texcoord)*numT);
		pos+=sizeof(jm_texcoord)*numT;
	}
	
	// load vertices
	memcpy(vList, data+pos, sizeof(jm_vertex)*numV);
	pos+=sizeof(jm_vertex)*numV;

	// load per face indices
	memcpy(iList, data+pos, sizeof(int)*numF*numI);
	pos+=sizeof(int)*numF*numI;

	// done :)
	free(data);
	loaded = true;
}

jmodel::~jmodel()
{
	if(loaded)
	{
		free(nList);
		if(numT) free(tList);
		free(vList);
		free(iList);
	}
}

void jmodel::glRender()
{
	glBegin(GL_TRIANGLES);
		for(int i=0;i<numF;i++)
		{
			glNormal(i,0);
			glTexCoord(i,0);
			glVertex(i,0);

			glNormal(i,1);
			glTexCoord(i,1);
			glVertex(i,1);

			glNormal(i,2);
			glTexCoord(i,2);
			glVertex(i,2);
		}
	glEnd();
}