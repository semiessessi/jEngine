/* log wrapper by Semi Essessi
 *
 * Debug error logging
 *
 */

#ifdef _DEBUG

#include "log.h"

FILE* log___f;

void LogInit()
{
	log___f = _wfopen(L"log.txt", L"wb");
	unsigned short a = 0xFEFF;
	fwrite(&a, sizeof(unsigned short), 1, log___f);
}

void LogShutdown()
{
	fclose(log___f);
}

void LogWriteLine(const wchar_t* instr, ...)
{
	if(!instr) return;

	wchar_t*	buf = 0;
	va_list	 parg;
	va_start(parg, instr);
	
	// allocate buffer
	int len = (_vscwprintf(instr, parg)+1);
	buf = (wchar_t*)malloc(len*sizeof(wchar_t));
	if(!buf) return;
	vswprintf(buf, instr, parg);
	va_end(parg);

	fwprintf(log___f, buf);
	fwprintf(log___f, L"\r\n");

	free(buf);
}

#endif