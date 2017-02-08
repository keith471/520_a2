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
// TODO: Modify --> this should be the head of the AST
extern EXP* theexpression;

// called if there is a syntax or parsing error
void yyerror(const char *s);
%}

%union {
	int ival;
	float fval;
	char *sval;
    struct EXP* exp;
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

%start prog

%left tMINUS tPLUS
%left tTIMES tDIVIDE
%left UMINUS

%%
// this is the actual grammar (productions) that bison will parse
// the first rule defined is the highest-level rule
// (note: $1 is the first thing matched, $2 is the second, etc.)

prog:
    dcls stmts
        {
            #ifdef BISON_DEBUG
                printf("done with a MiniLang file!\n");
            #endif
        }
    ;

dcls:
    dcls dcl | /*epsilon*/
        {
            #ifdef BISON_DEBUG
                printf("found the declarations\n");
            #endif
        }
    ;

stmts:
    stmts stmt | /*epsilon*/
        {
            #ifdef BISON_DEBUG
                printf("found the statements\n");
            #endif
        }
    ;

dcl:
    tVAR tIDENTIFIER tCOLON tFLOAT tSEMICOLON
        {
            #ifdef BISON_DEBUG
                printf("found a float declaration: %s\n", $2);
            #endif
        }
    | tVAR tIDENTIFIER tCOLON tINT tSEMICOLON
        {
            #ifdef BISON_DEBUG
                printf("found an int declaration: %s\n", $2);
            #endif
        }
    | tVAR tIDENTIFIER tCOLON tSTRING tSEMICOLON
        {
            #ifdef BISON_DEBUG
                printf("found a string declaration: %s\n", $2);
            #endif
        }
    ;

stmt:
    assignment | funccall | whileloop | condblock
    ;

assignment:
    tIDENTIFIER tASSIGN exp tSEMICOLON
        {
            #ifdef BISON_DEBUG
                printf("found an assignment\n");
            #endif
        }
    ;

funccall:
    tREAD tIDENTIFIER tSEMICOLON
        {
            #ifdef BISON_DEBUG
                printf("found a call to read\n");
            #endif
        }
    | tPRINT exp tSEMICOLON
        {
            #ifdef BISON_DEBUG
                printf("found a call to print\n");
            #endif
        }
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

whileloop:
    tWHILE exp tDO stmts tDONE
    ;

condblock:
    ifblock elseblock tENDIF
    ;

ifblock:
    tIF exp tTHEN stmts
    ;

elseblock:
    tELSE stmts | /*epsilon*/
    ;

%%

int main(int argc, char* argv[]) {

#ifdef FLEX_DEBUG
    // parse through the input until there is no more:
    do {
        yylex();
    } while (!feof(yyin));
#endif

#ifndef FLEX_DEBUG
    // If file name passed, read from that file. Else, do nothing
    char* fname;
    if (argc == 2) {
        fname = argv[1];
    } else {
        fname = "./src/test.min";
    }

    // open a file handle to the file to scan
    FILE *myfile = fopen(fname, "r");
    // make sure it's valid:
    if (!myfile) {
        printf("Error: can't open the file '%s'\n", fname);
        return 1;
    }
    // set lex to read from it instead of defaulting to STDIN:
    yyin = myfile;

    // parse through the input until there is no more:
    do {
        yyparse();
    } while (!feof(yyin));

    printf("VALID\n");
#endif

}

void yyerror(const char *s) {
#ifdef BISON_DEBUG
	printf("YIKES, parse error on line %d, before %s. Message: %s\n", yylineno, yytext, s);
#endif
    printf("INVALID\n");
	// exit on parse error
	exit(1);
}
