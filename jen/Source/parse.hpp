/* A Bison parser, made by GNU Bison 2.1.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     CINT = 258,
     CFLOAT = 259,
     CSTRING = 260,
     IDENTIFIER = 261,
     ADD = 262,
     SUB = 263,
     MUL = 264,
     DIV = 265,
     POW = 266,
     ASSIGN = 267,
     LB = 268,
     RB = 269,
     LS = 270,
     RS = 271,
     SEMICOLON = 272,
     TEXTURE = 273,
     BAKED = 274,
     DIFFUSE = 275,
     NORMAL = 276,
     SPECULAR = 277,
     OCCLUSION = 278,
     MAP = 279,
     TCMOD = 280,
     CHECKERBOARD = 281,
     PLAIN = 282,
     PNG = 283
   };
#endif
/* Tokens.  */
#define CINT 258
#define CFLOAT 259
#define CSTRING 260
#define IDENTIFIER 261
#define ADD 262
#define SUB 263
#define MUL 264
#define DIV 265
#define POW 266
#define ASSIGN 267
#define LB 268
#define RB 269
#define LS 270
#define RS 271
#define SEMICOLON 272
#define TEXTURE 273
#define BAKED 274
#define DIFFUSE 275
#define NORMAL 276
#define SPECULAR 277
#define OCCLUSION 278
#define MAP 279
#define TCMOD 280
#define CHECKERBOARD 281
#define PLAIN 282
#define PNG 283




#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 24 "bisonInput.y"
typedef union YYSTYPE {
    double              d;
    int                 i;
    char*               s;
    double*             dp;
    void*               vp;
} YYSTYPE;
/* Line 1447 of yacc.c.  */
#line 102 "parse.hpp"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;



