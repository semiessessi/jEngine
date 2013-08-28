/* alMusic class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#ifndef __ALMUSIC_H
#define __ALMUSIC_H

class alMusic
{
private:
	// pointer to single instance
	static alMusic* p;

	// make constructors and assignment private to enforce single instance
	alMusic();
	alMusic(const alMusic& a);
	alMusic& operator=(const alMusic& a);
public:
	~alMusic();

	// return pointer to our single instance, or create it
	static alMusic* getPointer();
};

#endif