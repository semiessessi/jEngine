/* log wrapper by Semi Essessi
 *
 * Debug error logging
 *
 */

#ifndef __LOG_H
#define __LOG_H

#ifdef _DEBUG

#include <stdio.h>
#include <stdarg.h>
#include <malloc.h>

extern FILE*	log___f;

void LogInit();
void LogShutdown();
void LogWriteLine(const wchar_t* instr, ...);

#endif

#endif