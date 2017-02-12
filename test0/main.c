#include <stdio.h>
#include "tree.h"
#include "pretty.h"
#include "symbol.h"

void yyparse();

PROGRAM *theprogram;

int main() {
    printf("Type some minilang code folowed by one or two Ctrl-d's:\n");
    // parse the program and build an AST rooted at theprogram
    yyparse();
    printf("\nPretty printed program:\n\n");
    // pretty print the program
    prettyPROGRAM(theprogram);
    printf("\n");
    // make a symbol table for the program
    symPROGRAM(theprogram);
    return(0);
}
