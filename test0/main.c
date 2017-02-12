#include <stdio.h>
#include "tree.h"
#include "pretty.h"
#include "symbol.h"
#include "error.h"
#include "type.h"

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
    terminateIfErrors();
    // type check the program
    typePROGRAM(theprogram);
    terminateIfErrors();
    return(0);
}
