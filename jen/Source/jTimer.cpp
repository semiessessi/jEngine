/* jTimer class by Semi Essessi
 *
 * High performance timer
 *
 */

#include "jTimer.h"

jTimer::jTimer()
{
	QueryPerformanceFrequency(&tps);
	QueryPerformanceCounter(&start);
	buf1=start;
	buf2=start;

	lastDT=0;
}

jTimer::~jTimer()
{
}

float jTimer::getDT()
{
	LARGE_INTEGER			current_time;
	float					dt;

	QueryPerformanceCounter(&current_time);
	// work out change in time
	dt=(float)(current_time.QuadPart - buf1.QuadPart)/(float)tps.QuadPart;	
	buf1=current_time;

	lastDT=dt;

	return dt;
}

float jTimer::getFPS(unsigned long numframes)
{
	LARGE_INTEGER			current_time;
	float					fps;

	QueryPerformanceCounter(&current_time);
	// work out the frame rate from the number of frames given and the change in time
	fps=(float)numframes*(float)tps.QuadPart/((float)current_time.QuadPart - (float)buf2.QuadPart);
	buf2=current_time;
	
	return fps;
}