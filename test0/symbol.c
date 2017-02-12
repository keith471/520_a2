#include <stdio.h>
#include <string.h>
#include "memory.h"
#include "symbol.h"
#include "error.h"

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
