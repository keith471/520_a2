%{
// General
#include <stdio.h>
#include <stdlib.h>

// AST stuff
#include "tree.h"

// Stuff from flex that bison needs to know about
extern int yylex();
extern int yyparse();
extern FILE* yyin;
extern int yylineno;
extern char* yytext;

// AST stuff
//extern EXP* theexpression;

// this is the root of the AST!
// defined in main, but created here
extern PROGRAM* theprogram;
/*
Can probs do something like this:
extern PROGRAM* theprogram;
Paired with the following below:
prog: exp { theprogram = $$; }
*/

// called if there is a syntax or parsing error
void yyerror(const char *s);
%}

%union {
	int ival;
	float fval;
	char *sval;
    struct EXP* exp;
    struct DECLARATION* declaration;
    struct STATEMENT* statement;
    struct TYPE* type;
    struct ID* id;
}

// define the constant-string tokens
%token tVAR tCOLON
%token tFLOAT tINT tSTRING
%token tWHILE tDO tDONE
%token tIF tTHEN tELSE tENDIF
%token tSEMICOLON
%token tPRINT
%token tREAD
%token tTIMES tDIVIDE tPLUS tMINUS
%token tASSIGN
%token tLPAREN tRPAREN
%token <sval> tIDENTIFIER

// define the "terminal symbols" and associate each with a field of the union:
%token <ival> tINTVAL
%token <fval> tFLOATVAL
%token <sval> tSTRINGVAL

%type <exp> exp
%type <declaration> declarations declaration
%type <statement> statements statement
%type <type> type
%type <id> identifier


%start program

%left tMINUS tPLUS
%left tTIMES tDIVIDE
%left UMINUS

%%
// this is the actual grammar (productions) that bison will parse
// the first rule defined is the highest-level rule
// (note: $1 is the first thing matched, $2 is the second, etc.)

// this is how we create the root node of the AST :) We build up all the entire tree in this fashion!
// i.e. here we build a PROGRAM from an EXP, and below we build the EXP :)
/*
prog: exp { theprogram = makePROGRAM($1); }
    ;
*/

program:
    declarations statements
        {
            #ifdef BISON_DEBUG
                printf("done with a MiniLang file!\n");
            #endif
            theprogram = makePROGRAM($1, $2);
        }
    ;

declarations:
      /*epsilon*/
        { $$ = NULL; }
    | declarations declaration
        {
            #ifdef BISON_DEBUG
                printf("found the declarations\n");
            #endif
            //$$ = appendDECLARATION($2, $1);
            $$ = appendDECLARATION($1, $2);
        }
    ;

statements:
      /*epsilon*/
        { $$ = NULL; }
    | statements statement
        {
            #ifdef BISON_DEBUG
                printf("found the statements\n");
            #endif
            $$ = makeSTATEMENTsequence($1, $2);
        }
    ;

declaration:
    tVAR identifier tCOLON type tSEMICOLON
        {
            #ifdef BISON_DEBUG
                printf("found a declaration: %s\n", $2);
            #endif
            $$ = makeDECLARATION($2, $4);
        }
    ;

identifier:
    tIDENTIFIER
        { $$ = makeID($1); }
    ;

type:
    tINT
        {
            #ifdef BISON_DEBUG
                printf("int declaration\n");
            #endif
            $$ = makeTYPEint();
        }
    | tFLOAT
        {
            #ifdef BISON_DEBUG
                printf("float declaration\n");
            #endif
            $$ = makeTYPEfloat();
        }
    | tSTRING
        {
            #ifdef BISON_DEBUG
                printf("string declaration\n");
            #endif
            $$ = makeTYPEstring();
        }
    ;

statement:
    exp
        { $$ = $1; }
    ;

exp:
      tIDENTIFIER
        {
            $$ = makeEXPid($1);
        }
    | tFLOATVAL
        {
            $$ = makeEXPfloatval($1);
        }
    | tINTVAL
        {
            $$ = makeEXPintval($1);
        }
    | tSTRINGVAL
        {
            $$ = makeEXPstringval($1);
        }
    | tLPAREN exp tRPAREN
        {
            $$ = $2;
        }
    | exp tPLUS exp
        {
            #ifdef BISON_DEBUG
                printf("found an addition\n");
            #endif
            $$ = makeEXPplus($1, $3);
        }
    | exp tMINUS exp
        {
            #ifdef BISON_DEBUG
                printf("found a subtraction\n");
            #endif
            $$ = makeEXPminus($1, $3);
        }
    | exp tTIMES exp
        {
            #ifdef BISON_DEBUG
                printf("found a multiplication\n");
            #endif
            $$ = makeEXPtimes($1, $3);
        }
    | exp tDIVIDE exp
        {
            #ifdef BISON_DEBUG
                printf("found a division\n");
            #endif
            $$ = makeEXPdiv($1, $3);
        }
    | tMINUS exp %prec UMINUS
        {
            #ifdef BISON_DEBUG
                printf("found a unary minus\n");
            #endif
            $$ = makeEXPunaryminus($2);
        }
    ;

%%

void yyerror(const char *s) {
    #ifdef BISON_DEBUG
    	printf("YIKES, parse error on line %d, before %s. Message: %s\n", yylineno, yytext, s);
    #endif
    printf("INVALID\n");
	// exit on parse error
	exit(1);
}
