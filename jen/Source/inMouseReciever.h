/* inMouseReciever class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#ifndef __INMOUSERECIEVER_H
#define __INMOUSERECIEVER_H

#include "inSystem.h"

class inMouseReciever
{
protected:
    inSystem*   in;
public:
	inMouseReciever();
	virtual ~inMouseReciever();

	virtual bool onMouseMovement(float x, float y, float dx, float dy);	
	virtual bool onMouseButtonDown(int button, float x, float y);
	virtual bool onMouseButtonUp(int button, float x, float y);
    virtual bool onMouseWheel(int amount);
};

#endif