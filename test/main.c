#include "tree.h"
#include "pretty.h"
#include <stdio.h>

void yyparse();

PROGRAM *theprogram;

int main() {
    printf("Type in a tiny exp folowed by one or two Ctrl-d's:\n");
    yyparse();
    printf("\nThe expression is:\n");
    prettyPROGRAM(theprogram);
    printf("\n");
    return(1);
}
