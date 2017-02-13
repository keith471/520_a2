%{
#include <stdio.h> // for printf
#include "y.tab.h"  // to get the token types that we return to Bison
int yyerror(char *s);
%}
%option yylineno
%%

[ \t\n\r] { ; }
var {
    #ifdef FLEX_DEBUG
        printf("found a 'var'\n");
    #endif
        return tVAR; }
while {
    #ifdef FLEX_DEBUG
        printf("found a 'while'\n");
    #endif
        return tWHILE;  }
do {
    #ifdef FLEX_DEBUG
        printf("found a 'do'\n");
    #endif
        return tDO; }
done {
    #ifdef FLEX_DEBUG
        printf("found a 'done'\n");
    #endif
        return tDONE; }
if {
    #ifdef FLEX_DEBUG
        printf("found an 'if'\n");
    #endif
        return tIF; }
then {
    #ifdef FLEX_DEBUG
        printf("found a 'then'\n");
    #endif
        return tTHEN; }
else {
    #ifdef FLEX_DEBUG
        printf("found an 'else'\n");
    #endif
        return tELSE; }
endif {
    #ifdef FLEX_DEBUG
        printf("found an 'endif'\n");
    #endif
        return tENDIF; }
float {
    #ifdef FLEX_DEBUG
        printf("found a 'float'\n");
    #endif
        return tFLOAT; }
int {
    #ifdef FLEX_DEBUG
        printf("found an 'int'\n");
    #endif
        return tINT; }
string {
    #ifdef FLEX_DEBUG
        printf("found a 'string'\n");
    #endif
        return tSTRING; }
print {
    #ifdef FLEX_DEBUG
        printf("found a 'print'\n");
    #endif
        return tPRINT; }
read {
    #ifdef FLEX_DEBUG
        printf("found a 'read'\n");
    #endif
        return tREAD; }
";" {
    #ifdef FLEX_DEBUG
        printf("found a semicolon\n");
    #endif
        return tSEMICOLON; }
":" {
    #ifdef FLEX_DEBUG
        printf("found a colon\n");
    #endif
        return tCOLON; }
"*" {
    #ifdef FLEX_DEBUG
        printf("found a times\n");
    #endif
        return tTIMES; }
"/" {
    #ifdef FLEX_DEBUG
        printf("found a division\n");
    #endif
        return tDIVIDE; }
"+" {
    #ifdef FLEX_DEBUG
        printf("found a plus\n");
    #endif
        return tPLUS; }
"-" {
    #ifdef FLEX_DEBUG
        printf("found a minus\n");
    #endif
        return tMINUS; }
"(" {
    #ifdef FLEX_DEBUG
        printf("found a left parenthesis\n");
    #endif
        return tLPAREN; }
")" {
    #ifdef FLEX_DEBUG
        printf("found a right parenthesis\n");
    #endif
        return tRPAREN; }
"=" {
    #ifdef FLEX_DEBUG
        printf("found an assignment operator\n");
    #endif
        return tASSIGN; }
(0|([1-9][0-9]*))\.[0-9]+ {
    #ifdef FLEX_DEBUG
        printf("found a float: %s\n", yytext);
    #endif
        yylval.fval = atof(yytext);
        return tFLOATVAL; }
0|[1-9][0-9]* {
    #ifdef FLEX_DEBUG
        printf("found an int: %s\n", yytext);
    #endif
        yylval.ival = atoi(yytext);
        return tINTVAL; }
[a-zA-z_][a-zA-z_0-9]* {
    #ifdef FLEX_DEBUG
        printf("found an identifier: %s\n", yytext);
    #endif
        yylval.sval = strdup(yytext);
        return tIDENTIFIER; }
\/\/.* {
    #ifdef FLEX_DEBUG
        printf("found a single-line comment: %s\n", yytext);
    #endif
}
. {
    #ifdef FLEX_DEBUG
        printf("found an unrecognized symbol. Throwing an error!\n");
    #endif
        yyerror(yytext); }
\"(?:\\[abfnrtv"\\]|[a-zA-Z0-9 ~#$%^&*\-+\/`<>=_|';:{}\[\]()@!?.,])*\" {
    #ifdef FLEX_DEBUG
        printf("found a string: %s\n", yytext);
    #endif
        yylval.sval = strdup(yytext);
        return tSTRINGVAL; }
%%
