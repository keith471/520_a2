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

char* headerfilepath;

/*
 * concatenate two strings and return the result
 */
char* concat(const char *s1, const char *s2) {
    char *result = malloc(strlen(s1)+strlen(s2)+1); //+ 1 for the null-terminator
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
    int slashIndex = -1;
    int i;
    // get the index of the last '.' and the last '/' in the name of the program file
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

    strncpy(name, &programFilename[slashIndex + 1], (end - slashIndex - 1));
    name[(end - slashIndex - 1)] = '\0';

    return name;
}

void parsePROGRAM(char* programFilename) {
    // open a file handle to the file to scan
    FILE *infile = fopen(programFilename, "r");
    // make sure it's valid
    if (!infile) {
        printf("Error: can't open the file '%s'\n", programFilename);
        exit(1);
    }
    // set lex to read from the external file instead of defaulting to STDIN
    yyin = infile;

    // parse through the input until there is no more
    printf("parsing program...");
    do {
        yyparse();
    } while (!feof(yyin));
    printf("SUCCESS\n");
    fclose(infile);
}

int main(int argc, char* argv[]) {

    char* programFilename = "default.min";
    char* outputPath;
    char* pathToSrc;
    char* programName;
    char* prettyFilename;
    char* symbolTableFilename;
    char* cFilename;

    if (argc == 3) {
        programFilename = argv[1];
        pathToSrc = argv[2];
        parsePROGRAM(programFilename);
    } else if (argc == 2 || argc > 3) {
        // assume we are one directory up from the source code
        printf("USAGE: <path_to_executable> <path_to_min_file> <path_to_src_code>\n");
        exit(1);
    } else {
        printf("Type some minilang code folowed by one or two Ctrl-d's:\n");
        // parse the program and build an AST rooted at theprogram
        yyparse();
    }

    programName = getProgramName(programFilename);
    // if pathToSrc ends in a '/', remove it
    if (pathToSrc[strlen(pathToSrc) - 1] == '/') {
        pathToSrc[strlen(pathToSrc) - 1] = '\0';
    }
    outputPath = concat(pathToSrc, "/output/");
    headerfilepath = concat(pathToSrc, "/headercode.txt");

    // pretty print the program
    printf("pretty printing program...");
    prettyFilename = concat(outputPath, concat(programName, ".pretty.min"));
    prettyPROGRAM(theprogram, prettyFilename);
    printf("SUCCESS\n");
    printf(">>> pretty printed program to %s\n", prettyFilename);

    // make a symbol table for the program
    printf("making a symbol table...");
    symPROGRAM(theprogram);
    symbolTableFilename = concat(outputPath, concat(programName, ".symbol.txt"));
    printSymbolTableToFile(symbolTableFilename);
    printf("SUCCESS\n");
    printf(">>> wrote the symbol table to %s\n", symbolTableFilename);
    terminateIfErrors();

    // type check the program
    printf("type checking the program...");
    typePROGRAM(theprogram);
    printf("SUCCESS\n");
    terminateIfErrors();

    // generate c code!
    printf("generating c code...");
    cFilename = concat(outputPath, concat(programName, ".c"));
    genPROGRAM(theprogram, cFilename);
    printf("SUCCESS\n");
    printf(">>> successfully compiled the program as %s\n", cFilename);

    return(0);
}
