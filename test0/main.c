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

char* outputPath = "./output/";

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
 */
char* getProgramName(char* programFilename) {
    int dotIndex = -1;
    int slashIndex = 0;
    int i;
    // get the index of the '.' and first '/' in the name of the program file
    for (i = strlen(programFilename) - 1; i >= 0; i--) {
        if (programFilename[i] == '.' && dotIndex == -1) {
            dotIndex = i;
        }
        if (programFilename[i] == '/') {
            slashIndex = i;
            break;
        }
    }

    char* name = malloc(strlen(programFilename)+1);

    int end;
    if (dotIndex == -1) {
        end = strlen(programFilename);
    } else {
        end = dotIndex;
    }

    for (i = slashIndex; i < end; i++) {
        name[i] = programFilename[i];
    }
    name[i] = '\0';

    printf("%s\n", name);
    exit(1);


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
    char* prettyFilename;
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

    // pretty print the program
    printf("pretty printing program...\n");
    prettyFilename = concat(outputPath, concat(programName, ".pretty.min"));
    prettyPROGRAM(theprogram, prettyFilename);
    printf(">>> pretty printed program to %s\n", prettyFilename);

    // make a symbol table for the program
    printf("making a symbol table...\n");
    symPROGRAM(theprogram);
    symbolTableFilename = concat(outputPath, concat(programName, ".symbol.txt"));
    printSymbolTableToFile(symbolTableFilename);
    printf(">>> wrote the symbol table to %s\n", symbolTableFilename);
    terminateIfErrors();

    // type check the program
    printf("type checking the program...\n");
    typePROGRAM(theprogram);
    terminateIfErrors();

    // generate c code!
    printf("generating c code...\n");
    cFilename = concat(outputPath, concat(programName, ".c"));
    genPROGRAM(theprogram, cFilename);
    printf(">>> successfully compiled the program as %s\n", cFilename);

    return(0);
}
