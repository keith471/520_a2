#include "tree.h"

#define HashSize 317

typedef struct SymbolTable {
    SYMBOL *table[HashSize];
    struct SymbolTable *next;
} SymbolTable;

SymbolTable *initSymbolTable();
