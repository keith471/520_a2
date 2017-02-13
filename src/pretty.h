#ifndef PRETTY_H
#define PRETTY_H

#include "tree.h"

void prettyPROGRAM(PROGRAM *p, char* fname);
void prettyDECLARATION(DECLARATION *d, FILE* emitFILE);
void prettySTATEMENT(STATEMENT *s, int level, FILE* emitFILE);
void prettySTATEMENTsequence(STATEMENT* s, int level, FILE* emitFILE);
void prettySTATEMENTassign(STATEMENT* s, int level, FILE* emitFILE);
void prettySTATEMENTfunccall(STATEMENT* s, int level, FILE* emitFILE);
void prettySTATEMENTif(STATEMENT* s, int level, FILE* emitFILE);
void prettySTATEMENTifelse(STATEMENT* s, int level, FILE* emitFILE);
void prettySTATEMENTwhile(STATEMENT* s, int level, FILE* emitFILE);
void prettyEXP(EXP *e, FILE* emitFILE);

#endif /* !PRETTY_H */
