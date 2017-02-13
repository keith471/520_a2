/*
 * Reconstructs the original program from the AST
 */

#include <stdio.h>
#include "pretty.h"
#include "outputhelpers.h"

// the number of times to indent before printing
int numIndents = 0;

void prettyPROGRAM(PROGRAM *p, char* fname) {
    FILE* emitFILE = fopen(fname, "w");
    prettyDECLARATION(p->declarations, emitFILE);
    newLineInFile(emitFILE);
    prettySTATEMENT(p->statements, 0, emitFILE);
    fclose(emitFILE);
}

/*
 * pretty print a declaration
 */
void prettyDECLARATION(DECLARATION *d, FILE* emitFILE) {
    // if null, stop, else print and then recurse on d->next
    if (d == NULL) return;
    fprintf(emitFILE, "var %s: ", d->id->name);
    switch (d->type->kind) {
        case intK:
            fprintf(emitFILE, "int;");
            break;
        case floatK:
            fprintf(emitFILE, "float;");
            break;
        case stringK:
            fprintf(emitFILE, "string;");
            break;
    }
    newLineInFile(emitFILE);
    prettyDECLARATION(d->next, emitFILE);
}

/*
 * Printing statements:
 * For statement printing, we always assume we are on a new line with no tabs yet printed.
 * After we finish printing a statement, we must print a new line
 */

void prettySTATEMENT(STATEMENT *s, int level, FILE* emitFILE) {
    // if null, stop, else print and recurse
    if (s == NULL) {
        return;
    }
    switch (s->kind) {
        case assignK:
            prettySTATEMENTassign(s, level, emitFILE);
            break;
        case fcallK:
            prettySTATEMENTfunccall(s, level, emitFILE);
            break;
        case ifK:
            prettySTATEMENTif(s, level, emitFILE);
            break;
        case ifelseK:
            prettySTATEMENTifelse(s, level, emitFILE);
            break;
        case whileK:
            prettySTATEMENTwhile(s, level, emitFILE);
            break;
    }
    // print the next statement
    prettySTATEMENT(s->next, level, emitFILE);
}

void prettySTATEMENTassign(STATEMENT* s, int level, FILE* emitFILE) {
    printTabsToFile(level, emitFILE);
    fprintf(emitFILE, "%s = ", s->val.assignS.id->name);
    prettyEXP(s->val.assignS.exp, emitFILE);
    fprintf(emitFILE, ";");
    newLineInFile(emitFILE);
}

void prettySTATEMENTfunccall(STATEMENT* s, int level, FILE* emitFILE) {
    FUNCTIONCALL* fc = s->val.fcallS;
    printTabsToFile(level, emitFILE);
    switch (fc->kind) {
        case readK:
            fprintf(emitFILE, "read %s", fc->val.readid->name);
            break;
        case printK:
            fprintf(emitFILE, "print ");
            prettyEXP(fc->val.printexp, emitFILE);
            break;
    }
    fprintf(emitFILE, ";");
    newLineInFile(emitFILE);
}

void prettySTATEMENTif(STATEMENT* s, int level, FILE* emitFILE) {
    printTabsToFile(level, emitFILE);
    fprintf(emitFILE, "if ");
    prettyEXP(s->val.ifS.condition, emitFILE);
    fprintf(emitFILE, " then");
    newLineInFile(emitFILE);
    prettySTATEMENT(s->val.ifS.body, level + 1, emitFILE);
    printTabsToFile(level, emitFILE);
    fprintf(emitFILE, "endif");
    newLineInFile(emitFILE);
}

void prettySTATEMENTifelse(STATEMENT* s, int level, FILE* emitFILE) {
    printTabsToFile(level, emitFILE);
    fprintf(emitFILE, "if ");
    prettyEXP(s->val.ifelseS.condition, emitFILE);
    fprintf(emitFILE, " then");
    newLineInFile(emitFILE);
    prettySTATEMENT(s->val.ifelseS.thenpart, level + 1, emitFILE);
    printTabsToFile(level, emitFILE);
    fprintf(emitFILE, "else");
    newLineInFile(emitFILE);
    prettySTATEMENT(s->val.ifelseS.elsepart, level + 1, emitFILE);
    printTabsToFile(level, emitFILE);
    fprintf(emitFILE, "endif");
    newLineInFile(emitFILE);
}

void prettySTATEMENTwhile(STATEMENT* s, int level, FILE* emitFILE) {
    printTabsToFile(level, emitFILE);
    fprintf(emitFILE, "while ");
    prettyEXP(s->val.whileS.condition, emitFILE);
    fprintf(emitFILE, " do");
    newLineInFile(emitFILE);
    prettySTATEMENT(s->val.whileS.body, level + 1, emitFILE);
    printTabsToFile(level, emitFILE);
    fprintf(emitFILE, "done");
    newLineInFile(emitFILE);
}

/*
 * Printing expressions:
 * For expression printing, we print wherever the write pointer currently is.
 * We do not print a new line after printing an expression.
 */

void prettyEXP(EXP *e, FILE* emitFILE) {
    switch (e->kind) {
        case idK:
            fprintf(emitFILE, "%s", e->val.idE->name);
            break;
        case intvalK:
            fprintf(emitFILE, "%i", e->val.intvalE);
            break;
        case floatvalK:
            fprintf(emitFILE, "%f", e->val.floatvalE);
            break;
        case stringvalK:
            fprintf(emitFILE, "%s", e->val.stringvalE);
            break;
        case timesK:
            fprintf(emitFILE, "(");
            // recursively print the left expression
            prettyEXP(e->val.timesE.left, emitFILE);
            // print the times
            fprintf(emitFILE, "*");
            // recursively print the right expression
            prettyEXP(e->val.timesE.right, emitFILE);
            fprintf(emitFILE, ")");
            break;
        case divK:
            fprintf(emitFILE, "(");
            prettyEXP(e->val.divE.left, emitFILE);
            fprintf(emitFILE, "/");
            prettyEXP(e->val.divE.right, emitFILE);
            fprintf(emitFILE, ")");
            break;
        case plusK:
            fprintf(emitFILE, "(");
            prettyEXP(e->val.plusE.left, emitFILE);
            fprintf(emitFILE, "+");
            prettyEXP(e->val.plusE.right, emitFILE);
            fprintf(emitFILE, ")");
            break;
        case minusK:
            fprintf(emitFILE, "(");
            prettyEXP(e->val.minusE.left, emitFILE);
            fprintf(emitFILE, "-");
            prettyEXP(e->val.minusE.right, emitFILE);
            fprintf(emitFILE, ")");
            break;
        case uminusK:
            fprintf(emitFILE, "(");
            fprintf(emitFILE, "-");
            prettyEXP(e->val.uminusE, emitFILE);
            fprintf(emitFILE, ")");
            break;
    }
}
