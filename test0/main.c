#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "pretty.h"
#include "symbol.h"
#include "error.h"
#include "type.h"
#include "codegen.h"

void yyparse();

PROGRAM *theprogram;

extern FILE* yyin;

/*
 * concatenate two strings and return the result
 */
char* concat(const char *s1, const char *s2) {
    char *result = malloc(strlen(s1)+strlen(s2)+1);//+1 for the null-terminator
    // should check for malloc errors here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

/*
 * parse and return the program name (without the .min extension)
 * splits the string at the first occurrence of '.'
 */
char* getProgramName(char* programFilename) {
    int dotIndex = -1;
    int i;
    // get the index of the '.' in the name of the program file
    for (i = 0; i < strlen(programFilename); i++) {
        if (programFilename[i] == '.') {
            dotIndex = i;
            break;
        }
    }

    char* name = malloc(strlen(programFilename)+1);
    if (dotIndex != -1) {
        strncpy(name, programFilename, i);
        name[i] = '\0';
    } else {
        // file name does not contain a '.'
        // we proceed anyway
        strncpy(name, programFilename, strlen(programFilename));
    }
    return name;
}

int main(int argc, char* argv[]) {

    char* programFilename = "default.min";
    char* programName;
    char* symbolTableFilename;
    char* cFilename;

    if (argc == 2) {
        programFilename = argv[1];
        // open a file handle to the file to scan
        FILE *infile = fopen(programFilename, "r");
        // make sure it's valid
        if (!infile) {
            printf("Error: can't open the file '%s'\n", programFilename);
            return 1;
        }
        // set lex to read from the external file instead of defaulting to STDIN
        yyin = infile;

        // parse through the input until there is no more
        do {
            yyparse();
        } while (!feof(yyin));
    } else {
        printf("Type some minilang code folowed by one or two Ctrl-d's:\n");
        // parse the program and build an AST rooted at theprogram
        yyparse();
    }

    programName = getProgramName(programFilename);

    printf("\n---------------------------------\n");
    printf("Pretty printed program:");
    printf("\n_________________________________\n");
    // pretty print the program
    prettyPROGRAM(theprogram);
    printf("\n==================================================================\n");
    // make a symbol table for the program
    printf("making a symbol table...\n");
    symPROGRAM(theprogram);
    symbolTableFilename = concat(programName, ".symbol.txt");
    printSymbolTableToFile(symbolTableFilename);
    printf("    wrote the symbol table to %s\n", symbolTableFilename);
    terminateIfErrors();
    // type check the program
    printf("type checking the program...\n");
    typePROGRAM(theprogram);
    terminateIfErrors();
    printf("generating c code...\n");
    cFilename = concat(programName, ".c");
    genPROGRAM(theprogram, cFilename);
    printf("\nSuccessfully compiled the program as %s\n", cFilename);
    return(0);
}
