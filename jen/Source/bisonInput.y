%{
    #include "parse.hpp"
    #include "Scanner.h"
    
    #include "glTextureManager.h"
    
    #include "jEngine.h"
    #include "console.h"
    
    #include <math.h>
    #include <stdio.h>
    // #include <windows.h>
    
    int yyerror(char* s);
    
    #pragma warning(disable : 4065)
    
    double vars[16] = { 0 };
    bool parseError = false;
    
    static glTextureManager* tex;
%}

%union {
    double              d;
    int                 i;
    char*               s;
    double*             dp;
    void*               vp;
}

%start program

%token <i> CINT
%token <d> CFLOAT
%token <s> CSTRING
%token <s> IDENTIFIER
%token ADD
%token SUB
%token MUL
%token DIV
%token POW
%token ASSIGN
%token LB
%token RB
%token LS
%token RS
%token SEMICOLON
%token TEXTURE
%token BAKED
%token DIFFUSE
%token NORMAL
%token SPECULAR
%token OCCLUSION
%token MAP
%token TCMOD
%token CHECKERBOARD
%token PLAIN
%token PNG

%left ADD
%left SUB
%left DIV
%left MUL
%left ASSIGN
%right POW

%type <d> constant

%error-verbose

%%

program:          /* nothing */
                | textures
;

textures:         texture
                | textures texture
;

texture:          TEXTURE IDENTIFIER LS expressions RS
;

expressions:      expression
                | expressions expression

expression:       BAKED LS expressions RS
                | MAP PLAIN CINT CINT constant constant constant constant
                | MAP CHECKERBOARD CINT CINT constant constant constant constant constant constant constant constant
                | MAP PNG CSTRING
;

constant:         CINT                                                                                                              { $$ = (int)$1; }
                | CFLOAT                                                                                                            { $$ = (double)$1; }
;

%%

int parse(char* code)
{
    tex = glTextureManager::getPointer(jEngine::getPointer());
    parseError = false;
    yy_scan_string(code);
    int ret = yyparse();
    yy_free_ptr();
    return ret;
}

int yyerror(char* s)
{
    // MessageBoxA(0,s,"Error!",MB_OK);
    jEngine::getPointer()->con->writeLine(s);
    parseError = true;
    return 0;
}

#pragma warning(default : 4065)