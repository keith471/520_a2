#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory.h"
#include "symbol.h"
#include "error.h"

/*
 * Concatenate two strings and return the result
 */
char* concat(const char *s1, const char *s2) {
    char *result = malloc(strlen(s1)+strlen(s2)+1);//+1 for the null-terminator
    // should check for malloc errors here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

char* printSymbolTableToFile(char* programName) {
    int dotIndex = -1;
    int i;
    // get the index of the '.' in the name of the program file
    for (i = 0; i < strlen(programName); i++) {
        if (programName[i] == '.') {
            dotIndex = i;
            break;
        }
    }

    // create the name of the file where we will print the symbol table
    char partOne[100];
    char* fname;
    if (dotIndex != -1) {
        strncpy(partOne, programName, i);
        partOne[i] = '\0';
    } else {
        // file name does not contain a '.'
        // we proceed anyway
        strncpy(partOne, programName, strlen(programName));
    }
    fname = concat(partOne, ".symbol.txt");

    // print the symbol table to the file
    FILE *file = fopen(fname, "w");

    // print headers
    fprintf(file, "symbol\ttype\n");

    // print symbols and types
    SYMBOL* s;
    for (i = 0; i < HashSize; i++) {
        s = symbolTable->table[i];
        while (s != NULL) {
            fprintf(file, "%s\t%s\n", s->name, getTypeAsString(s->type));
            s = s->next;
        }
    }

    fclose(file);

    return fname;
}

char* getTypeAsString(TYPE* type) {
    switch (type->kind) {
        case intK:
            return "int";
        case floatK:
            return "float";
        case stringK:
            return "string";
    }
    return "";
}

/*
 * Hash an identifier into an index
 */
int Hash(char *str) {
    unsigned int hash = 0;
    while (*str) hash = (hash << 1) + *str++;
    return hash % HashSize;
}

SymbolTable *initSymbolTable() {
    SymbolTable *t;
    int i;
    t = NEW(SymbolTable);
    for (i=0; i < HashSize; i++) t->table[i] = NULL;
    t->next = NULL;
    return t;
}

SYMBOL *putSymbol(char *name, TYPE* type, SymbolTable *t, int lineno) {
    int i = Hash(name);
    SYMBOL *s;
    // check for an existing symbol with the same name
    for (s = t->table[i]; s; s = s->next) {
        // TODO probably should throw an error instead of returning it
        if (strcmp(s->name, name) == 0) reportIdentifierError("identifier already exists", name, lineno);
    }
    s = NEW(SYMBOL);
    s->name = name;
    s->type = type;
    s->next = t->table[i];
    t->table[i] = s;
    return s;
}

SYMBOL *getSymbol(char *name, SymbolTable *t, int lineno) {
    int i = Hash(name);
    SYMBOL *s;
    for (s = t->table[i]; s; s = s->next) {
        if (strcmp(s->name,name)==0) return s;
    }
    // couldn't find the symbol
    reportIdentifierError("identifier not declared", name, lineno);
    return NULL;
}

void symPROGRAM(PROGRAM *p) {
    symbolTable = initSymbolTable();
    symDECLARATIONS(p->declarations, symbolTable);
    symSTATEMENTS(p->statements, symbolTable);
}

void symDECLARATIONS(DECLARATION* declarations, SymbolTable* symbolTable) {
    if (declarations == NULL) return;
    SYMBOL* s;
    s = putSymbol(declarations->id->name, declarations->type, symbolTable, declarations->lineno);
    declarations->id->symbol = s;
    symDECLARATIONS(declarations->next, symbolTable);
}

void symSTATEMENTS(STATEMENT* statements, SymbolTable* symbolTable) {
    if (statements == NULL) return;
    SYMBOL* s;
    switch (statements->kind) {
        case assignK:
            s = getSymbol(statements->val.assignS.id->name, symbolTable, statements->lineno);
            statements->val.assignS.id->symbol = s;
            symEXP(statements->val.assignS.exp, symbolTable);
            break;
        case fcallK:
            symFUNCTIONCALL(statements->val.fcallS, symbolTable);
            break;
        case ifK:
            symEXP(statements->val.ifS.condition, symbolTable);
            symSTATEMENTS(statements->val.ifS.body, symbolTable);
            break;
        case ifelseK:
            symEXP(statements->val.ifelseS.condition, symbolTable);
            symSTATEMENTS(statements->val.ifelseS.thenpart, symbolTable);
            symSTATEMENTS(statements->val.ifelseS.elsepart, symbolTable);
            break;
        case whileK:
            symEXP(statements->val.whileS.condition, symbolTable);
            symSTATEMENTS(statements->val.whileS.body, symbolTable);
            break;
    }
    symSTATEMENTS(statements->next, symbolTable);
}

void symEXP(EXP* exp, SymbolTable* symbolTable) {
    SYMBOL* s;
    switch (exp->kind) {
        case idK:
            s = getSymbol(exp->val.idE->name, symbolTable, exp->lineno);
            exp->val.idE->symbol = s;
        case intvalK:
            break;
        case floatvalK:
            break;
        case stringvalK:
            break;
        case timesK:
            symEXP(exp->val.timesE.left, symbolTable);
            symEXP(exp->val.timesE.right, symbolTable);
        case divK:
            symEXP(exp->val.divE.left, symbolTable);
            symEXP(exp->val.divE.right, symbolTable);
        case plusK:
            symEXP(exp->val.plusE.left, symbolTable);
            symEXP(exp->val.plusE.right, symbolTable);
        case minusK:
            symEXP(exp->val.minusE.left, symbolTable);
            symEXP(exp->val.minusE.right, symbolTable);
        case uminusK:
            symEXP(exp->val.uminusE, symbolTable);
            break;
    }
}

void symFUNCTIONCALL(FUNCTIONCALL* fc, SymbolTable* symbolTable) {
    SYMBOL* s;
    switch (fc->kind) {
        case readK:
            s = getSymbol(fc->val.readid->name, symbolTable, fc->lineno);
            fc->val.readid->symbol = s;
            break;
        case printK:
            symEXP(fc->val.printexp, symbolTable);
            break;
    }
}
