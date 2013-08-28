/* jStack class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#ifndef __JSTACK_H
#define __JSTACK_H

#define JSTACK_START_SIZE 32

#include <malloc.h>

template<class C>
class jStack
{
private:
	C** m;
	unsigned int size;
	unsigned int alloc;
public:
	
	jStack()
	{
		// constructor code for jStack
		size = 0;
		alloc = JSTACK_START_SIZE;
		m=(C**)malloc(sizeof(C*)*alloc);
	}
	
	jStack(unsigned int startSize)
	{
		// constructor code for jStack
		size = 0;
		alloc = startSize;
		m=(C**)malloc(sizeof(C*)*alloc);
	}
	
	~jStack()
	{
		// destructor code for jStack
		if(m) free(m);
	}
	
	bool forceAlloc(unsigned int num)
	{
		alloc = num;
		m=(C**)realloc(m,sizeof(C*)*alloc);
		if(m) return true;
		else return false;
	}

	bool forceAllocExtra(unsigned int num)
	{
		alloc += num;
		m=(C**)realloc(m,sizeof(C*)*alloc);
		if(m) return true;
		else return false;
	}

	void clear()
	{
		size = 0;
	}
	
	bool push(C* p)
	{
		if(p)
		{
			size++;
			if(size>alloc)
			{
				alloc=alloc<<1;
				m=(C**)realloc(m,sizeof(C*)*alloc);
				if(!m) return false;
			}
			m[size-1]=p;
			return true;
		}
		else return false;
	}
	
	void pushFast(C* p)
	{
		m[size]=p;
		size++;
	}
	
	C* pop()
	{
		if(size>0)
		{
			size--;
			return m[size];
		}
		else return 0;
	}

	C* popFast()
	{
		size--;
		return m[size];
	}
	
	C* peek()
	{
		if(size==0) return 0;
		return m[size-1];
	}
};

#endif