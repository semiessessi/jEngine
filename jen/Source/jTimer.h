/* jTimer class by Semi Essessi
 *
 * High performance timer
 *
 */

#ifndef __JTIMER_H
#define __JTIMER_H

#include <windows.h>

class jTimer
{
private:
	LARGE_INTEGER	start;
	LARGE_INTEGER	tps;
	LARGE_INTEGER	buf1;
	LARGE_INTEGER	buf2;

	float			lastDT;
public:
	jTimer();
	~jTimer();
	
	float getDT();
	float getLastDT(){ return lastDT; }
  
	float getFPS(unsigned long numframes = 1);	// (numframes = frames since last query)										   
};

#endif