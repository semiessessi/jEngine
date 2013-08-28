/* inKeyReciever class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#ifndef __INKEYRECIEVER_H
#define __INKEYRECIEVER_H

class inKeyBinding
{
private:
    wchar_t* name;
    void    (*call)();
public:
    inKeyBinding();
    ~inKeyBinding();
};

class inKeyReciever
{
protected:
    inKeyBinding*    keyBinds[256];
public:
	inKeyReciever();
	virtual ~inKeyReciever();

	virtual void KeyUp(int vKey) {}
	virtual void KeyDown(int vKey) {}
};

#endif