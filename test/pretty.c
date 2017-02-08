/*
 * Reconstructs the original program from the AST
 */

#include <stdio.h>
#include "pretty.h"

void prettyPROGRAM(PROGRAM *p) {
    prettyDECLARATION(p->declarations);

    // TODO print the statements
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
            printf("int;\n");
            break;
        case floatK:
            printf("float;\n");
            break;
        case stringK:
            printf("string;\n");
            break;
    }
    prettyDECLARATION(d->next);
}

void prettyEXP(EXP *e) {
    switch (e->kind) {
    case idK:
        printf("%s",e->val.idE);
        break;
    case intvalK:
        printf("%i",e->val.intvalE);
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
