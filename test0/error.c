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
    fprintf(stderr, "    *** message: %s\n", s);
    fprintf(stderr, "    *** location: syntax error before %s at line %i\n", yytext, yylineno);
    fprintf(stderr, "*** compilation terminated\n");
    exit(1);
}

/*
 * for type errors
 */
void reportTypeError(char *s, int lineno) {
    printf("TYPE ERROR:\n");
    printf("    *** message: %s\n", s);
    printf("    *** location: line %i\n", lineno);
    printf("*** compilation terminated\n");
    errors++;
    // terminate if any type errors are found as one type error could propagate,
    // causing us to end up will null types and segmentation faults
    exit(1);
}

/*
 * For symbol errors
 */
void reportIdentifierError(char *s, char *name, int lineno) {
    printf("IDENTIFIER ERROR:\n");
    printf("    *** message: %s\n", s);
    printf("    *** identifier: %s\n", name);
    printf("    *** location: at line %i\n", lineno);
    errors++;
}

void terminateIfErrors() {
    if (errors != 0) {
        fprintf(stderr, "*** compilation terminated due to %i errors\n", errors);
        exit(1);
    }
}
