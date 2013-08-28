/* strSystem class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#ifndef __STRSYSTEM_H
#define __STRSYSTEM_H

class jEngine;

#include <stdarg.h>

class strSystem
{
private:
	// pointer to top-level class
	jEngine* e;

	// pointer to single instance
	static strSystem* p;

	wchar_t* currentLanguage;

	wchar_t**		strName;
	wchar_t**		strContent;
	int				numStrings;

	// to optimise search remember positions of first strings beginning with a,A,b,B.. (sort on load)
	int				searchOpt[53];

	// make constructors and assignment private to enforce single instance
	strSystem(jEngine* j);
	strSystem(const strSystem& a);
	strSystem& operator=(const strSystem& a);

	// comparison for qsort
	static int compStr(const void* s1, const void* s2);

	void cleanupStringTable();

	wchar_t* strFillParams(wchar_t* ptr, wchar_t* str, va_list parg);
public:
	~strSystem();

	// return pointer to our single instance, or create it
	static strSystem* getPointer(jEngine* j);

	bool loadLanguage(const wchar_t* langaugeName);

	wchar_t* getString(wchar_t* stringName, ...);

	static void setUILanguage(void** ptr, const wchar_t* val);
};

#endif