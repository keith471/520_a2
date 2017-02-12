#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "error.h"

// stuff from flex
extern char *yytext;
extern int yylineno;

int errors = 0;

void yyerror(char *s) {
    fprintf(stderr, "PARSER ERROR:\n");
    fprintf(stderr, "\t*** message: %s\n", s);
    fprintf(stderr, "\t*** location: syntax error before %s at line %i\n", yytext, yylineno);
    fprintf(stderr,"*** compilation terminated\n");
    exit(1);
}

void reportError(char *s, int lineno) {
    printf("*** %s at line %i\n", s, lineno);
    errors++;
}

void reportIdentifierError(char *s, char *name, int lineno) {
    printf("IDENTIFIER ERROR:\n");
    printf("\t*** message: %s\n", s);
    printf("\t*** identifier: %s\n", name);
    printf("\t*** location: at line %i\n", lineno);
    errors++;
}

void terminateIfErrors() {
    if (errors != 0) {
        fprintf(stderr,"*** compilation terminated due to errors\n");
        exit(1);
    }
}
