/* A Bison parser, made by GNU Bison 3.0.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    tVAR = 258,
    tCOLON = 259,
    tFLOAT_TYPE = 260,
    tINT_TYPE = 261,
    tSTRING_TYPE = 262,
    tWHILE = 263,
    tDO = 264,
    tDONE = 265,
    tIF = 266,
    tTHEN = 267,
    tELSE = 268,
    tENDIF = 269,
    tSEMICOLON = 270,
    tPRINT = 271,
    tREAD = 272,
    tTIMES = 273,
    tDIVIDE = 274,
    tPLUS = 275,
    tMINUS = 276,
    tASSIGN = 277,
    tLPAREN = 278,
    tRPAREN = 279,
    tIDENTIFIER = 280,
    tENDL = 281,
    tINT = 282,
    tFLOAT = 283,
    tSTRING = 284,
    UMINUS = 285
  };
#endif
/* Tokens.  */
#define tVAR 258
#define tCOLON 259
#define tFLOAT_TYPE 260
#define tINT_TYPE 261
#define tSTRING_TYPE 262
#define tWHILE 263
#define tDO 264
#define tDONE 265
#define tIF 266
#define tTHEN 267
#define tELSE 268
#define tENDIF 269
#define tSEMICOLON 270
#define tPRINT 271
#define tREAD 272
#define tTIMES 273
#define tDIVIDE 274
#define tPLUS 275
#define tMINUS 276
#define tASSIGN 277
#define tLPAREN 278
#define tRPAREN 279
#define tIDENTIFIER 280
#define tENDL 281
#define tINT 282
#define tFLOAT 283
#define tSTRING 284
#define UMINUS 285

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 15 "parser.y" /* yacc.c:1915  */

	int ival;
	float fval;
	char *sval;

#line 120 "y.tab.h" /* yacc.c:1915  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
