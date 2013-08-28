/* cVar class by Semi Essessi
 *
 * Console variable
 *
 */

#ifndef __CVAR_H
#define __CVAR_H

class jEngine;

class cVar
{
private:
	// pointer to top-level class
	jEngine* e;

	wchar_t*		name;
	wchar_t*		helpString;
	void*			variable;
	void*			defaultValue;
	char			type;
	void			(*setter)(void**, const wchar_t*);
	void			(*cycleUp)(void**);
	void			(*cycleDown)(void**);
	wchar_t*		(*getStr)(void*);

public:
	cVar(jEngine* j, const wchar_t* variableName, const wchar_t* description, char t, void (*up)(void**), void (*down)(void**), void (*set)(void**, const wchar_t*)=0);
	cVar(jEngine* j, const wchar_t* variableName, const wchar_t* description, char t, void (*cycler)(void**), void (*set)(void**, const wchar_t*)=0);
	cVar(jEngine* j, const wchar_t* variableName, const wchar_t* description, char t, void (*set)(void**, const wchar_t* )=0);
	cVar(jEngine* j, const wchar_t* variableName, char t, void (*set)(void**, const wchar_t*)=0);
	~cVar();

	void* getPointer() { return variable; }
	void** getDoublePointer() { return &variable; }
	wchar_t* getValueString() { return getStr(variable); }
	wchar_t* getDefaultString() { return getStr(defaultValue); }
	wchar_t* getName() { return name; }
	wchar_t* getHelp() { return helpString; }
	char getType() { return type; }

	void setValue(const wchar_t* str);
	void setDefaultValue(const wchar_t* str);
	void setToDefaultValue();
	void increment();
	void decrement();

	bool isVariable(const wchar_t* strName);

	void overrideGet(wchar_t* (*func)(void*));

	// setters
	static void setNop(void** p, const wchar_t* str);
	static void setBool(void** p, const wchar_t* str);
	static void setFloat(void** p, const wchar_t* str);
	static void setInt(void** p, const wchar_t* str);
	static void setString(void** p, const wchar_t* str);

	// getters
	static wchar_t* getNop(void* p);
	static wchar_t* getBool(void* p);
	static wchar_t* getFloat(void* p);
	static wchar_t* getInt(void* p);
	static wchar_t* getString(void* p);

	// increment/decrement functions
	static void swNop(void** p);
	static void swBool(void** p);
	static void swFloatInc(void** p);
	static void swFloatDec(void** p);
	static void swIntInc(void** p);
	static void swIntDec(void** p);
};

#endif