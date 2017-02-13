#include "tree.h"

#define HashSize 317

typedef struct SymbolTable {
    SYMBOL *table[HashSize];
    // a pointer to the next symbol table (for nested scopes)
    struct SymbolTable *next;
} SymbolTable;

SymbolTable *symbolTable;

SymbolTable *initSymbolTable();

SYMBOL *putSymbol(char *name, TYPE* type, SymbolTable *t, int lineno);

SYMBOL *getSymbol(char *name, SymbolTable *t, int lineno);

void symPROGRAM(PROGRAM *p);
void symDECLARATIONS(DECLARATION* declarations, SymbolTable* symbolTable);
void symSTATEMENTS(STATEMENT* statements, SymbolTable* symbolTable);
void symEXP(EXP* exp, SymbolTable* symbolTable);
void symFUNCTIONCALL(FUNCTIONCALL* fc, SymbolTable* symbolTable);

// for printing symbol table to file
void printSymbolTableToFile(char* fname);
char* getTypeAsString(TYPE* type);
