/* cCom class by Semi Essessi
 *
 * Console command
 *
 */

#ifndef __CCOM_H
#define __CCOM_H

class jEngine;

class cCom
{
private:
	// pointer to top-level class
	jEngine* e;

	wchar_t*		command;
	wchar_t*		helpString;
	void			(*caller)(const wchar_t*);
public:
	cCom(jEngine* j, const wchar_t* name, const wchar_t* help, void(*func)(const wchar_t*));
	cCom(jEngine* j, const wchar_t* name, void(*func)(const wchar_t*));
	~cCom();

	bool isCommand(const wchar_t* strName);
	void Call(const wchar_t* paramString) { caller(paramString); }

	wchar_t* getName() { return command; }
	wchar_t* getHelp() { return helpString; }

	static void ccomNop(const wchar_t* p);
};

#endif