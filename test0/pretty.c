/*
 * Reconstructs the original program from the AST
 */

#include <stdio.h>
#include "pretty.h"

// the number of times to indent before printing
int numIndents = 0;

void prettyPROGRAM(PROGRAM *p) {
    prettyDECLARATION(p->declarations);
    newLine();
    prettySTATEMENT(p->statements, 0);
}

/*
 * pretty print a declaration
 */
void prettyDECLARATION(DECLARATION *d) {
    // if null, stop, else print and then recurse on d->next
    if (d == NULL) return;
    printf("var %s: ", d->id->name);
    switch (d->type->kind) {
        case intK:
            printf("int;");
            break;
        case floatK:
            printf("float;");
            break;
        case stringK:
            printf("string;");
            break;
    }
    newLine();
    prettyDECLARATION(d->next);
}

/*
 * Printing statements:
 * For statement printing, we always assume we are on a new line with no tabs yet printed.
 * After we finish printing a statement, we must print a new line
 */

void prettySTATEMENT(STATEMENT *s, int level) {
    // if null, stop, else print and recurse
    if (s == NULL) {
        return;
    }
    switch (s->kind) {
        case assignK:
            prettySTATEMENTassign(s, level);
            break;
        case fcallK:
            prettySTATEMENTfunccall(s, level);
            break;
        case ifK:
            prettySTATEMENTif(s, level);
            break;
        case ifelseK:
            prettySTATEMENTifelse(s, level);
            break;
        case whileK:
            prettySTATEMENTwhile(s, level);
            break;
    }
    // print the next statement
    prettySTATEMENT(s->next, level);
}

void prettySTATEMENTassign(STATEMENT* s, int level) {
    printTabs(level);
    printf("%s = ", s->val.assignS.id->name);
    prettyEXP(s->val.assignS.exp);
    printf(";");
    newLine();
}

void prettySTATEMENTfunccall(STATEMENT* s, int level) {
    FUNCTIONCALL* fc = s->val.fcallS;
    printTabs(level);
    switch (fc->kind) {
        case readK:
            printf("read %s", fc->val.readid->name);
            break;
        case printK:
            printf("print ");
            prettyEXP(fc->val.printexp);
            break;
    }
    printf(";");
    newLine();
}

void prettySTATEMENTif(STATEMENT* s, int level) {
    printTabs(level);
    printf("if ");
    prettyEXP(s->val.ifS.condition);
    printf(" then");
    newLine();
    prettySTATEMENT(s->val.ifS.body, level + 1);
    printTabs(level);
    printf("endif");
    newLine();
}

void prettySTATEMENTifelse(STATEMENT* s, int level) {
    printTabs(level);
    printf("if ");
    prettyEXP(s->val.ifelseS.condition);
    printf(" then");
    newLine();
    prettySTATEMENT(s->val.ifelseS.thenpart, level + 1);
    printTabs(level);
    printf("else");
    newLine();
    prettySTATEMENT(s->val.ifelseS.elsepart, level + 1);
    printTabs(level);
    printf("endif");
    newLine();
}

void prettySTATEMENTwhile(STATEMENT* s, int level) {
    printTabs(level);
    printf("while ");
    prettyEXP(s->val.whileS.condition);
    printf(" do");
    newLine();
    prettySTATEMENT(s->val.whileS.body, level + 1);
    printTabs(level);
    printf("done");
    newLine();
}

/*
 * Printing expressions:
 * For expression printing, we print wherever the write pointer currently is.
 * We do not print a new line after printing an expression.
 */

void prettyEXP(EXP *e) {
    switch (e->kind) {
        case idK:
            printf("%s", e->val.idE->name);
            break;
        case intvalK:
            printf("%i", e->val.intvalE);
            break;
        case floatvalK:
            printf("%f", e->val.floatvalE);
            break;
        case stringvalK:
            printf("%s", e->val.stringvalE);
            break;
        case timesK:
            printf("(");
            // recursively print the left expression
            prettyEXP(e->val.timesE.left);
            // print the times
            printf("*");
            // recursively print the right expression
            prettyEXP(e->val.timesE.right);
            printf(")");
            break;
        case divK:
            printf("(");
            prettyEXP(e->val.divE.left);
            printf("/");
            prettyEXP(e->val.divE.right);
            printf(")");
            break;
        case plusK:
            printf("(");
            prettyEXP(e->val.plusE.left);
            printf("+");
            prettyEXP(e->val.plusE.right);
            printf(")");
            break;
        case minusK:
            printf("(");
            prettyEXP(e->val.minusE.left);
            printf("-");
            prettyEXP(e->val.minusE.right);
            printf(")");
            break;
        case uminusK:
            printf("(");
            printf("-");
            prettyEXP(e->val.uminusE);
            printf(")");
            break;
    }
}

// Helpers

void printTabs(int n) {
    int i;
    for (i = 0; i < n; i++) {
        printf("    ");
    }
}

void newLine() {
    printf("\n");
}
