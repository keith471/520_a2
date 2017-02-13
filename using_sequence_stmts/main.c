#include "tree.h"
#include "pretty.h"
#include <stdio.h>

void yyparse();

PROGRAM *theprogram;

int main() {
    printf("Type some minilang code folowed by one or two Ctrl-d's:\n");
    yyparse();
    printf("\nPretty printed program:\n\n");
    prettyPROGRAM(theprogram);
    printf("\n");
    return(0);
}
