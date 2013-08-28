/* inTextReciever class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#include "inTextReciever.h"

#include <memory.h>
#include <windows.h>

inTextReciever::inTextReciever()
{
	// constructor code for inTextReciever
	memset(inLine,0,sizeof(wchar_t)*1024);
	caretPos = 0;
	insertMode = false;
}

inTextReciever::~inTextReciever()
{
	// destructor code for inTextReciever
}

void inTextReciever::wmCharHandler(unsigned int wParam, long lParam)
{
	defCharHandler(wParam, lParam);	
}

void inTextReciever::wmKeyHandler(unsigned int wParam, long lParam)
{
	defKeyHandler(wParam, lParam);	
}

void inTextReciever::defCharHandler(unsigned int wParam, long lParam)
{
	switch (wParam) 
	{ 
		case 0x08: 
			// Process a backspace.
			break;

		case 0x0A: 
			// Process a linefeed. 
			break; 

		case 0x1B: 
			// Process an escape. 
			break; 

		case 0x09: 
			// Process a tab. 
			break; 

		case 0x0D: 
			// Process a carriage return. 
			break;
		
		default: 
			// Process displayable characters.
			if(caretPos<1023)
			{
				if(insertMode)
				{
					inLine[caretPos] = (wchar_t)wParam;
					caretPos++;
				} else
				{
					for(int i=1023;i>caretPos;i--)
					{
						inLine[i]=inLine[i-1];
					}
					inLine[caretPos] = (wchar_t)wParam;
					caretPos++;
				}
			}
			break;
	}
}

void inTextReciever::defKeyHandler(unsigned int wParam, long lParam)
{
	switch(wParam) 
	{ 
		case VK_LEFT: 
			caretPos=max(caretPos-1,0);
			break; 

		case VK_RIGHT: 
			caretPos=min(caretPos+1,(int)wcslen(inLine));
			break; 

		case VK_UP: 
			// Process the UP ARROW key. 
			break; 

		case VK_DOWN: 
			// Process the DOWN ARROW key. 
			break; 

		case VK_HOME: 
			caretPos = 0; 
			break; 

		case VK_END: 
			caretPos = (int)wcslen(inLine);
			break; 

		case VK_INSERT: 
			insertMode = !insertMode; 
			break; 

		case VK_DELETE: 
			if(caretPos<((int)wcslen(inLine)))
			{
				for(unsigned int i=caretPos;i<wcslen(inLine);i++)
				{
					inLine[i]=inLine[i+1];
				}
			}
			break;

		case VK_BACK:
			if(caretPos>0)
			{
				for(unsigned int i=caretPos-1;i<wcslen(inLine);i++)
				{
					inLine[i]=inLine[i+1];
				}
				caretPos--;
			}			
			break;

		default: 
			break; 
	} 
}