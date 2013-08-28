/* menuScriptRoutine class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#include "menuScriptRoutine.h"

#include "menuCom.h"
#include "menuScriptLine.h"
#include "menuSystem.h"
#include "menuVar.h"

#include "jProfiler.h"
#include "jStack.h"
#include "log.h"

#include <malloc.h>
#include <memory.h>
#include <wchar.h>

#define isNum(a) ((a==L'0')||(a==L'1')||(a==L'2')||(a==L'3')||(a==L'4')||(a==L'5')||(a==L'6')||(a==L'7')||(a==L'8')||(a==L'9'))
#define getNum(a) (((int)a)-48)
#define isType(a) (!_wcsicmp(a,L"bool") || !_wcsicmp(a,L"float") || !_wcsicmp(a,L"int") || !_wcsicmp(a,L"string") || !_wcsicmp(a,L"variant"))

unsigned int menuScriptRoutine::codeLineNum = 0;
jStack<menuVar>* menuScriptRoutine::tStack = 0;
jStack<menuVar>* menuScriptRoutine::pStack = 0;

menuScriptRoutine::menuScriptRoutine(menuSystem* ms, menuItem* mi)
{
	m = ms;
	parentItem = mi;

	lines = 0;
	numLines = 0;

	locals = 0;
	numLocals = 0;

	statics = 0;
	numStatics = 0;

	if(!tStack) tStack = new jStack<menuVar>(256);
	if(!pStack) pStack = new jStack<menuVar>(256);

	addLine(L"nop",0,0);
}

menuScriptRoutine::menuScriptRoutine(menuSystem* ms, menuItem* mi,  const wchar_t* code, menuVar* dt)
{
#ifdef _DEBUG
	jProfiler profiler = jProfiler(L"menuScriptRoutine::menuScriptRoutine");
#endif
	// constructor code for menuScriptRoutine
	m = ms;
	parentItem = mi;

	lines = 0;
	numLines = 0;

	locals = 0;
	numLocals = 0;

	statics = 0;
	numStatics = 0;

	if(!tStack) tStack = new jStack<menuVar>(256);
	if(!pStack) pStack = new jStack<menuVar>(256);

	if(dt) addLocal(dt);

	wchar_t tType[64];
	wchar_t tName[256];

	memset(tType, 0, sizeof(wchar_t)*64);
	memset(tName, 0, sizeof(wchar_t)*256);
	swscanf(code, L"%s %s", tType, tName);

	int i = (int)wcslen(tName)+(int)wcslen(tType)+3;
	int lastPos = i-1;
	bool inQuotes = false;
	while(code[i]!=0)
	{
		// look for semi-colons outside of quotes
		if(code[i]==L'"') inQuotes = !inQuotes;
		if(code[i]==L';' && !inQuotes)
		{
			int len = i-lastPos;
			wchar_t* tmp = (wchar_t*)malloc(sizeof(wchar_t)*(len+1));
			int j = 0;
			while(j<len)
			{
				tmp[j] = code[lastPos+j+1];
				j++;
			}

			tmp[j]=0;

			wchar_t* exp = postfixFromInfix(tmp);
			
			addLinesFromPostfix(exp);

			free(tmp);
			lastPos = i;
		}
		i++;
	}
}

menuScriptRoutine::~menuScriptRoutine()
{
	// destructor code for menuScriptRoutine
	if(numLines)
	{
		for(int i=0;i<numLines;i++)
		{
			delete lines[i];
		}

		free(lines);
	}

	if(numLocals)
	{
		for(int i=0;i<numLocals;i++)
		{
			delete locals[i];
		}

		free(locals);
	}

	if(numStatics)
	{
		for(int i=0;i<numStatics;i++)
		{
			delete statics[i];
		}

		free(statics);
	}

	if(tStack) delete tStack;
	if(pStack) delete pStack;
	tStack = 0;
	pStack = 0;
}

void menuScriptRoutine::Call()
{
	if(numLines)
	{
		for(int i=0;i<numLines;i++)
		{
			lines[i]->Call();
		}
	}
}

void menuScriptRoutine::addLine(const wchar_t* funcName, menuCom* cParam, menuVar* vParam)
{
	menuCom* mC = m->getCommand(funcName);
	if(mC)
	{
		numLines++;
		lines = (menuScriptLine**)realloc(lines,sizeof(menuScriptLine*)*numLines);
		lines[numLines-1] = new menuScriptLine(mC, parentItem, tStack, pStack, cParam, vParam);
	}
}

void menuScriptRoutine::addLocal(menuVar* var)
{
	if(var)
	{
		numLocals++;
		locals = (menuVar**)realloc(locals,sizeof(menuVar*)*numLocals);
		locals[numLocals-1] = var;
	}
}

void menuScriptRoutine::addStatic(menuVar* var)
{
	if(var)
	{
		numStatics++;
		statics = (menuVar**)realloc(statics,sizeof(menuVar*)*numStatics);
		statics[numStatics-1] = var;
	}
}

wchar_t* menuScriptRoutine::postfixFromInfix(const wchar_t* exp)
{
	jStack<wchar_t> st;
	jStack<wchar_t> l;

	// push a marker onto the stack so we know when we reach the end...
	wchar_t end[2] = L";";
	st.pushFast(end);

	bool inQuotes = false;

	wchar_t* ret = 0;
	int rLen = 1;

	int i = 0;
	while(true)
	{
		if(exp[i]==L'"') inQuotes = !inQuotes; // ignore spaces inside of quotes
		else if((exp[i]==L' ' || exp[i]==0) && !inQuotes)
		{
			// run back to the next space then copy into buffer
			int j=i;
			while(!((exp[j-1]==L' ') && !inQuotes))
			{
				if(!j) break;
				j--;
				if(exp[j]==L'"') inQuotes = !inQuotes;
			}

			int size = i-j;
			wchar_t* buf = new wchar_t[size+1];
			l.push(buf);
			memcpy(buf,&(exp[j]),sizeof(wchar_t)*(size));
			buf[size] = 0;

			// TODO::if its an operator or function name lets stick it on the stack
			menuCom* mC = m->getCommand(buf);
			if(mC)
			{
				if(!(mC->isOperator()))
				{
					// if function then first push a bracket, push the function, then push a second bracket
					// TODO:: check next token is bracket and compile error
					wchar_t* temp = new wchar_t[2];
					swprintf(temp, L"(");
					l.push(temp);
					st.push(temp);

					while(exp[i]!=L'(') i++;
					i++;

					st.push(buf);
					st.push(temp);
				}				
				else if(st.peek())
				{
					// if semi-colon then push onto stack
					if(!_wcsicmp(st.peek(), L";"))
					{
						st.push(buf);
					}
					else if(!_wcsicmp(st.peek(), L"("))
					{
						// if bracket then push onto stack
						st.push(buf);
					}
					else if(mC->getPrecedence() > m->getCommand(st.peek())->getPrecedence())
					{
						// if has higher precedance push onto stack
						st.push(buf);
					}
					else
					{
						// otherwise pop until we reach a lower precedence function/operator then push onto stack
						while(true)
						{
							wchar_t* tmp = st.pop();
							int rem=rLen-1;
							rLen+=(int)wcslen(tmp)+1;
							ret = (wchar_t*)realloc(ret,sizeof(wchar_t)*(rLen));
							memcpy(&(ret[rem+1]),tmp,sizeof(wchar_t)*((int)wcslen(tmp)+1));
							ret[rem]=L' ';
							if(st.peek())
							{
								// if semi-colon then push onto stack
								if(!_wcsicmp(st.peek(), L";"))
								{
									st.push(buf);
									break;
								}
								else if(!_wcsicmp(st.peek(), L"("))
								{
									// if bracket then push onto stack
									st.push(buf);
									break;
								}
								else if(mC->getPrecedence() > m->getCommand(st.peek())->getPrecedence())
								{
									// if has higher precedance push onto stack
									st.push(buf);
									break;
								}
							}
							else
							{
								// push onto stack if stack is empty
								st.push(buf);
								break;
							}
						}
					}
				}
				else
				{
					// push onto stack if stack is empty
					st.push(buf);
				}
			}
			else if(!_wcsicmp(buf,L"("))
			{
				// else if we have a left bracket
				st.push(buf);
			}
			else if(!_wcsicmp(buf,L")"))
			{
				// else if we have a right bracket
				wchar_t* tmp = st.pop();
				// pop everything till we reach a left bracket
				while(_wcsicmp(tmp,L"("))
				{
					int rem=rLen-1;
					rLen+=(int)wcslen(tmp)+1;
					ret = (wchar_t*)realloc(ret,sizeof(wchar_t)*(rLen));
					memcpy(&(ret[rem+1]),tmp,sizeof(wchar_t)*((int)wcslen(tmp)+1));
					ret[rem]=L' ';
					tmp = st.pop();
				}

				// check if we are at the end of a function parameter list
				menuCom* mC = m->getCommand(st.peek());
				if(mC)
				{
					if(!(mC->isOperator()))
					{
						tmp = st.pop();
						// pop everything till we reach a left bracket
						while(_wcsicmp(tmp,L"("))
						{
							int rem=rLen-1;
							rLen+=(int)wcslen(tmp)+1;
							ret = (wchar_t*)realloc(ret,sizeof(wchar_t)*(rLen));
							memcpy(&(ret[rem+1]),tmp,sizeof(wchar_t)*((int)wcslen(tmp)+1));
							ret[rem]=L' ';
							tmp = st.pop();
						}
					}
				}
			}
			else if(!_wcsicmp(buf,L";"))
			{
				// else if we have a semicolon
				wchar_t* tmp = st.pop();
				// pop everything till we reach the end
				while(_wcsicmp(tmp,L";"))
				{
					int rem=rLen-1;
					rLen+=(int)wcslen(tmp)+1;
					ret = (wchar_t*)realloc(ret,sizeof(wchar_t)*(rLen));
					memcpy(&(ret[rem+1]),tmp,sizeof(wchar_t)*((int)wcslen(tmp)+1));
					ret[rem]=L' ';
					tmp = st.pop();
				}
				break;
			}
			else if(!_wcsicmp(buf,L","))
			{
				// we have a comma
				// treat like ) but push a ( on afterwards

				wchar_t* tmp = st.pop();
				// pop everything till we reach a left bracket
				while(_wcsicmp(tmp,L"("))
				{
					int rem=rLen-1;
					rLen+=(int)wcslen(tmp)+1;
					ret = (wchar_t*)realloc(ret,sizeof(wchar_t)*(rLen));
					memcpy(&(ret[rem+1]),tmp,sizeof(wchar_t)*((int)wcslen(tmp)+1));
					ret[rem]=L' ';
					tmp = st.pop();
				}

				// push a left bracket
				wchar_t* temp = new wchar_t[2];
				swprintf(temp, L"(");
				l.push(temp);
				st.push(temp);
			}
			else if(wcslen(buf)==0)
			{
				// do nothing with zero length identifiers
			}
			else
			{
				// else stick it in the output
				int rem=rLen-1;
				rLen+=size+1;
				ret = (wchar_t*)realloc(ret,sizeof(wchar_t)*(rLen));
				memcpy(&(ret[rem+1]),buf,sizeof(wchar_t)*(size+1));
				ret[rem]=L' ';
			}
		}
		if(exp[i]==0) break;
		i++;
	}

	while(wchar_t* b = l.pop())
	{
		if(b) delete[] b;
	}

	// trim starting space if its there...
	if(ret[0]==L' ')
	{
		i=0;
		while(ret[i]!=0)
		{
			ret[i]=ret[i+1];
			i++;
		}
	}

	ret = (wchar_t*)realloc(ret,sizeof(wchar_t)*(wcslen(ret)+1));

	return ret;
}

void menuScriptRoutine::addLinesFromPostfix(const wchar_t* exp)
{
	jStack<wchar_t> l;
	bool inQuotes = false;
	int i = 0;
	
	while(true)
	{
		if(exp[i]==L'"') inQuotes = !inQuotes; // ignore spaces inside of quotes
		else if((exp[i]==L' ' || exp[i]==0) && !inQuotes)
		{
			// run back to the next space then copy into buffer
			int j=i;
			while(!((exp[j-1]==L' ') && !inQuotes))
			{
				if(!j) break;
				j--;
				if(exp[j]==L'"') inQuotes = !inQuotes;
			}

			int size = i-j;
			wchar_t* buf = new wchar_t[size+1];
			l.push(buf);
			memcpy(buf,&(exp[j]),sizeof(wchar_t)*(size));
			buf[size] = 0;

			menuCom* mC = m->getCommand(buf);
			if(mC)
			{
				addLine(buf,0,0);
			}
			else if(!_wcsicmp(buf,L"static")) // static variable declaration
			{
				// get next two tokens
				i++;
				while(exp[i]!=L' ' && exp[i]!=0) i++;

				// TODO::make sure there are enough tokens and that they are not strings or numbers

				// run back to the next space then copy into buffer
				j=i;
				while(!((exp[j-1]==L' ') && !inQuotes))
				{
					if(!j) break;
					j--;
					if(exp[j]==L'"') inQuotes = !inQuotes;
				}

				// TODO::make sure type is valid
				int sType = i-j;
				wchar_t* bType = new wchar_t[sType+1];
				l.push(bType);
				memcpy(bType,&(exp[j]),sizeof(wchar_t)*(sType));
				bType[sType] = 0;

				i++;
				while(exp[i]!=L' ' && exp[i]!=0) i++;

				// run back to the next space then copy into buffer
				j=i;
				while(!((exp[j-1]==L' ') && !inQuotes))
				{
					if(!j) break;
					j--;
					if(exp[j]==L'"') inQuotes = !inQuotes;
				}

				// TODO::make sure name is valid
				int sName = i-j;
				wchar_t* bName = new wchar_t[sName+1];
				l.push(bName);
				memcpy(bName,&(exp[j]),sizeof(wchar_t)*(sName));
				bName[sName] = 0;

				// TODO:: build new routine out of this line somehow so we can initialise the varaible once only...

				menuVar* tmp = new menuVar(bName,(char)bType[0]);
				addStatic(tmp);
				addLine(L"@pushv",0,tmp);
			}
			else if(isType(buf)) // local variable declaration
			{
				// get next token
				// TODO::make sure there are enough tokens and that they are not strings or numbers

				wchar_t* bType = buf;

				i++;
				while(exp[i]!=L' ' && exp[i]!=0) i++;

				// run back to the next space then copy into buffer
				j=i;
				while(!((exp[j-1]==L' ') && !inQuotes))
				{
					if(!j) break;
					j--;
					if(exp[j]==L'"') inQuotes = !inQuotes;
				}

				// TODO::make sure name is valid
				int sName = i-j;
				wchar_t* bName = new wchar_t[sName+1];
				l.push(bName);
				memcpy(bName,&(exp[j]),sizeof(wchar_t)*(sName));
				bName[sName] = 0;

				menuVar* tmp = new menuVar(bName,(char)bType[0]);
				//addLine(L"@local"
				addLocal(tmp);
				addLine(L"@pushv",0,tmp);
			}
			else if(isLocal(buf)) // local variable usage
			{
				menuVar* tmp = getLocal(buf);
				addLine(L"@pushv",0,tmp);
			}
			else if(buf[0]==L'"') // string
			{
				menuVar* tmp = new menuVar(L"@",'s');
				buf=&(buf[1]);
				buf[size-1]=0;
				buf[size-2]=0;
				tmp->setString(buf);
				addStatic(tmp);
				addLine(L"@pushv",0,tmp);
			}
			else if(isNum(buf[0])) // number
			{
				j = 0;
				int pCount = 0;
				bool num = true;
				while(buf[j]!=0)
				{
					if(buf[j]==L'.') pCount++;
					else if(!isNum(buf[j])) num = false;
					j++;
				}
				if(pCount == 1) // float
				{
					menuVar* tmp = new menuVar(L"@",'f');
					tmp->setFloat((float)_wtof(buf));
					addStatic(tmp);
					addLine(L"@pushv",0,tmp);
				}
				else if(pCount == 0) // int
				{
					menuVar* tmp = new menuVar(L"@",'i');
					tmp->setInt((int)_wtoi(buf));
					addStatic(tmp);
					addLine(L"@pushv",0,tmp);
				}
			}
		}
		if(exp[i]==0)
		{
			// TODO::optimise this
			// clear stacks at end of each line
			addLine(L"@stclear",0,0);
			break;
		}
		i++;
	}

	while(wchar_t* b = l.pop())
	{
		if(b) delete[] b;
	}
}

bool menuScriptRoutine::isLocal(const wchar_t* name)
{
	for(int i=0;i<numLocals;i++)
	{
		if(locals[i]->isVariable(name)) return true;
	}
	return false;
}

menuVar* menuScriptRoutine::getLocal(const wchar_t* name)
{
	for(int i=0;i<numLocals;i++)
	{
		if(locals[i]->isVariable(name)) return locals[i];
	}
	return 0;
}