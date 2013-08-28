#ifndef SCANNER_H
#define SCANNER_H

int yylex(void);
int yy_scan_string(char*);
void yy_free_ptr();

int parse(char*);

extern double vars[16];
extern bool parseError;

#endif