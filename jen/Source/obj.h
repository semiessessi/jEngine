// library for loading OBJ

#include <malloc.h>
#include <memory.h>
#include <stdio.h>
#include <string.h>

class objData
{
public:
	double* 		vData;		// vertices	
	double* 		tData;		// texcoords
	double* 		nData;		// normals
	
	unsigned int*	fData;		// faces		- each face is laid out as v1, v2, v3, n1, n2, n3, t1, t2, t3, mtl
	
	char** 			mData;		// material names
	
	unsigned int 	vCount;
	unsigned int 	tCount;
	unsigned int 	nCount;
	unsigned int 	fCount;
	unsigned int 	mCount;
	
public:
	objData()
	{
		vData = tData = nData = 0;
        mData = 0;
		vCount = tCount = nCount = mCount = 0;
	}
	~objData()
	{
		if(vData) free(vData);
		if(tData) free(tData);
		if(nData) free(nData);
        if(mData) free(mData);
	}
	
	class objTriangleReference
	{
	public:
		unsigned int 	face;
		unsigned int 	material;
		objData*		ptr;
	public:
		objTriangleReference(objData* p, unsigned int n) : ptr(p), face(n) {};
			
		class objVertices
		{
		private:
			objData*		ptr;
			unsigned int	face;
		public:
			objVertices(objTriangleReference* ref) : ptr(ref->ptr), face(ref->face) {}
			
			double* operator[](unsigned int idx)
			{
				if(idx < 3) return &(ptr->vData[ptr->fData[face*10 + idx]]);
				else return 0;
			}
		};
		
		class objNormals
		{
		private:
			objData*		ptr;
			unsigned int	face;
		public:
			objNormals(objTriangleReference* ref) : ptr(ref->ptr), face(ref->face) {}
			
			double* operator[](unsigned int idx)
			{
				if(idx < 3) return &(ptr->nData[ptr->fData[face*10 + idx + 3]]);
				else return 0;
			}
		};
		
		class objTexCoords
		{
		private:
			objData*		ptr;
			unsigned int	face;
		public:
			objTexCoords(objTriangleReference* ref) : ptr(ref->ptr), face(ref->face) {}
			
			double* operator[](unsigned int idx)
			{
				if(idx < 3) return &(ptr->tData[ptr->fData[face*10 + idx + 6]]);
				else return 0;
			}
			
		};
		
		objVertices Vertices()
		{
			return objVertices(this);
		}
		
		objNormals Normals()
		{
			return objNormals(this);
		}
		
		objTexCoords TexCoords()
		{
			return objTexCoords(this);
		}
	};
	
	objTriangleReference operator[](unsigned int idx)
	{
		return objTriangleReference(this, idx);
	}
	
	bool load(const char* filename)
	{
		FILE* f = fopen(filename, "rb");
		
		if(!f) return false;
		
		// read all of the data line by line
		
		// get size of data from file
		fseek(f, 0, SEEK_SET);
		fseek(f, 0, SEEK_END);
		unsigned int dataLength = ftell(f);
		fseek(f, 0, SEEK_SET);
		
		char* data = (char*)malloc(sizeof(char)*(dataLength + 1));
		memset(data, 0, sizeof(char)*(dataLength + 1));
		
		// read data and close file
		fread(data, sizeof(char), dataLength, f);	
		fclose(f);
		
		unsigned int marker = 0;
		unsigned int curMtl = 0;
		double ta, tb, tc, td, te, tf, tg, th, ti;
		char buf[256];
		// process data
		for(unsigned int i = 0; i < dataLength; ++i)
		{
			if(data[i] == '\n')
			{
				unsigned int len = i - marker;
				
				if(data[i-1] == '\r') --len;
				
				// decide what sort of data this is...
				
				// is it a vertex, normal or texcoord?
				if(data[marker] == 'v')
				{
					if(data[marker+1] == 't')
					{
						// texcoord
						scanf(&(data[marker]), "vt %f %f", &ta, &tb);
						
						++tCount;
						tData = (double*)realloc(tData, (sizeof(double) * tCount) << 1);
						tData[(tCount - 1) << 1] = tb;
						tData[((tCount - 1) << 1) - 1] = ta;
						
					}
					else if(data[marker+1] == 'n')
					{
						// normal
						scanf(&(data[marker]), "vn %f %f %f", &ta, &tb, &tc);
						
						++nCount;
						nData = (double*)realloc(tData, (sizeof(double) * nCount)*3);
						nData[(nCount - 1)*3] = tc;
						nData[((nCount - 1)*3) - 1] = tb;
						nData[((nCount - 1)*3) - 2] = ta;
					}
					else
					{
						// vertex
						scanf(&(data[marker]), "v %f %f %f", &ta, &tb, &tc);
						
						++vCount;
						vData = (double*)realloc(vData, (sizeof(double) * vCount)*3);
						vData[(vCount - 1)*3] = tc;
						vData[((vCount - 1)*3) - 1] = tb;
						vData[((vCount - 1)*3) - 2] = ta;
					}
				}
				
				// is it a face?
				if(data[marker] == 'f')
				{
					scanf(&(data[marker]), "f %d/%d/%d %d/%d/%d %d/%d/%d", &ta, &tb, &tc, &td, &te, &tf, &tg, &th, &ti);
					
					fData = (unsigned int*)realloc(fData, (sizeof(unsigned int)*(fCount + 1)) * 10);
					
					fData[fCount*10] = ta;
					fData[(fCount*10) + 1] = tb;
					fData[(fCount*10) + 2] = tc;
					fData[(fCount*10) + 3] = td;
					fData[(fCount*10) + 4] = te;
					fData[(fCount*10) + 5] = tf;
					fData[(fCount*10) + 6] = tg;
					fData[(fCount*10) + 7] = th;
					fData[(fCount*10) + 8] = ti;
					fData[(fCount*10) + 9] = curMtl;
					
					++fCount;
				}
				
				// is it a "use material" instruction?
				if(data[marker] == 'u')
				{
					if((data[marker+1] == 's') && (data[marker+1] == 'e') && (data[marker+1] == 'm') && (data[marker+1] == 't') && (data[marker+1] == 'l'))
					{
						memset(buf, 0, 256);
						scanf("usemtl %s", buf);
						
						// see if material already added
						bool exists = false;
						unsigned int idx = 0;
						for(unsigned int i = 0; i < mCount; ++i)
						{
							if(!stricmp(buf, mData[i]))
							{
								exists = true;
								idx = i;
								break;
							}
						}
						
						// add material if necessary
						if(!exists)
						{
							idx = mCount;
							++mCount;
							
							// allocate space for pointer and space for string
							mData = (char**)realloc(mData, sizeof(char*)*mCount);
							unsigned int l = strlen(buf) + 1;
							mData[idx] = (char*)malloc(l*sizeof(char));
							
							// copy string into space
							for(unsigned int i = 0; i < l; ++i) mData[idx][i] = buf[i];
						}
						
						// update current material
						curMtl = idx;
					}
				}
				
				marker = i + 1;
			}
		}
		
		free(data);		
	
		return true;
	}
};