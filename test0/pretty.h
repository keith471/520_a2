#ifndef PRETTY_H
#define PRETTY_H

#include "tree.h"

void prettyPROGRAM(PROGRAM *p);
void prettyDECLARATION(DECLARATION *d);
void prettySTATEMENT(STATEMENT *s, int level);
void prettySTATEMENTsequence(STATEMENT* s, int level);
void prettySTATEMENTassign(STATEMENT* s, int level);
void prettySTATEMENTfunccall(STATEMENT* s, int level);
void prettySTATEMENTif(STATEMENT* s, int level);
void prettySTATEMENTifelse(STATEMENT* s, int level);
void prettySTATEMENTwhile(STATEMENT* s, int level);
void prettyEXP(EXP *e);

// helpers
void printTabs(int n);
void newLine();

#endif /* !PRETTY_H */
