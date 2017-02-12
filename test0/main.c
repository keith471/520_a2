#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "pretty.h"
#include "symbol.h"
#include "error.h"
#include "type.h"

void yyparse();

PROGRAM *theprogram;

extern FILE* yyin;

int main(int argc, char* argv[]) {

    char* fname = "default.min";
    if (argc == 2) {
        fname = argv[1];
        // open a file handle to the file to scan
        FILE *myfile = fopen(fname, "r");
        // make sure it's valid:
        if (!myfile) {
            printf("Error: can't open the file '%s'\n", fname);
            return 1;
        }
        // set lex to read from the external file instead of defaulting to STDIN
        yyin = myfile;

        // parse through the input until there is no more
        do {
            yyparse();
        } while (!feof(yyin));
    } else {
        printf("Type some minilang code folowed by one or two Ctrl-d's:\n");
        // parse the program and build an AST rooted at theprogram
        yyparse();
    }

    printf("\n---------------------------------\n");
    printf("Pretty printed program:");
    printf("\n_________________________________\n");
    // pretty print the program
    prettyPROGRAM(theprogram);
    printf("\n==================================================================\n");
    // make a symbol table for the program
    printf("making a symbol table...\n");
    symPROGRAM(theprogram);
    fname = printSymbolTableToFile(fname);
    printf("    wrote the symbol table to %s\n", fname);
    terminateIfErrors();
    // type check the program
    printf("type checking the program...\n");
    typePROGRAM(theprogram);
    terminateIfErrors();
    printf("\nCompilation successful\n");
    return(0);
}
