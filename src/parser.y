%{
#include <stdio.h>
#include <stdlib.h>

// stuff from flex that bison needs to know about
extern int yylex();
extern int yyparse();
extern FILE *yyin;
extern int yylineno;
extern char* yytext;

void yyerror(const char *s);
%}

%union {
	int ival;
	float fval;
	char *sval;
}

// define the constant-string tokens
%token tVAR tCOLON
%token tFLOAT_TYPE tINT_TYPE tSTRING_TYPE
%token tWHILE tDO tDONE
%token tIF tTHEN tELSE tENDIF
%token tSEMICOLON
%token tPRINT
%token tREAD
%token tTIMES tDIVIDE tPLUS tMINUS
%token tASSIGN
%token tLPAREN tRPAREN
%token <sval> tIDENTIFIER
%token tENDL

// define the "terminal symbols" and associate each with a field of the union:
%token <ival> tINT
%token <fval> tFLOAT
%token <sval> tSTRING

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
    tVAR tIDENTIFIER tCOLON tFLOAT_TYPE tSEMICOLON
        {
            #ifdef BISON_DEBUG
                printf("found a float declaration: %s\n", $2);
            #endif
        }
    | tVAR tIDENTIFIER tCOLON tINT_TYPE tSEMICOLON
        {
            #ifdef BISON_DEBUG
                printf("found an int declaration: %s\n", $2);
            #endif
        }
    | tVAR tIDENTIFIER tCOLON tSTRING_TYPE tSEMICOLON
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
    tIDENTIFIER tASSIGN expr tSEMICOLON
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
    | tPRINT expr tSEMICOLON
        {
            #ifdef BISON_DEBUG
                printf("found a call to print\n");
            #endif
        }
    ;

expr:
      tIDENTIFIER
    | tFLOAT
    | tINT
    | tSTRING
    | tLPAREN expr tRPAREN
    | expr tPLUS expr
        {
            #ifdef BISON_DEBUG
                printf("found an addition\n");
            #endif
        }
    | expr tMINUS expr
        {
            #ifdef BISON_DEBUG
                printf("found a subtraction\n");
            #endif
        }
    | expr tTIMES expr
        {
            #ifdef BISON_DEBUG
                printf("found a multiplication\n");
            #endif
        }
    | expr tDIVIDE expr
        {
            #ifdef BISON_DEBUG
                printf("found a division\n");
            #endif
        }
    | tMINUS expr %prec UMINUS
        {
            #ifdef BISON_DEBUG
                printf("found a unary minus\n");
            #endif
        }
    ;

whileloop:
    tWHILE expr tDO stmts tDONE
    ;

condblock:
    ifblock elseblock tENDIF
    ;

ifblock:
    tIF expr tTHEN stmts
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
