#include "tree.h"

void initTypes();
void checkAssignment(TYPE *left, TYPE *right, int lineno);
void checkCondition(TYPE *t, int lineno);

void typePROGRAM(PROGRAM *p);
void typeSTATEMENT(STATEMENT* statement);
void typeFUNCTIONCALL(FUNCTIONCALL* fc);
void typeEXP(EXP* exp);

TYPE* getExpType(TYPE* left, TYPE* right);
TYPE* getTimesExpType(TYPE *left, TYPE* right, int lineno);
TYPE* getDivExpType(TYPE *left, TYPE* right, int lineno);
TYPE* getPlusExpType(TYPE *left, TYPE* right, int lineno);
TYPE* getMinusExpType(TYPE *left, TYPE* right, int lineno);
TYPE* getUnaryMinusExpType(TYPE* exp, int lineno);
