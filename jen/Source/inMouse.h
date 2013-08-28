/* inMouse class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#ifndef __INMOUSE_H
#define __INMOUSE_H

class inMouseReciever;

class inMouse
{
private:
	// pointer to single instance
	static inMouse* p;

	float xPos;
	float yPos;
	float zPos;

	bool b1;
	bool b2;
	bool b3;
	bool b4;
	bool b5;

	inMouseReciever* mouseReciever;

	// make constructors and assignment private to enforce single instance
	inMouse();
	inMouse(const inMouse& a);
	inMouse& operator=(const inMouse& a);
public:
	~inMouse();

	// return pointer to our single instance, or create it
	static inMouse* getPointer();

	void inputCycle();
	float getXPos() { return xPos; }
	float getYPos() { return yPos; }
	float getZPos() { return zPos; }

	bool getButton1() { return b1; }
	bool getButton2() { return b2; }
	bool getButton3() { return b3; }
	bool getButton4() { return b4; }
	bool getButton5() { return b5; }

	inMouseReciever* getMouseReciever();
	void setMouseReciever(inMouseReciever* mr);
};

#endif