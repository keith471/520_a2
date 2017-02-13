/*
 * This file is for all the AST node constructors
 */

#include "memory.h"
#include "tree.h"

extern int lineno;

/*
 * Make an expression for an identifier
 */
EXP* makeEXPid(char *id) {
    EXP *e;
    e = NEW(EXP);
    e->lineno = lineno;
    e->kind = idK;
    e->val.idE = id;
    return e;
}

/*
 * Make an expression for an integer value
 */
EXP* makeEXPintval(int intval) {
    EXP *e;
    e = NEW(EXP);
    e->lineno = lineno;
    e->kind = intvalK;
    e->val.intvalE = intval;
    return e;
}

/*
 * Make an expression for a float value
 */
EXP* makeEXPfloatval(float floatval) {
    EXP *e;
    e = NEW(EXP);
    e->lineno = lineno;
    e->kind = floatvalK;
    e->val.floatvalE = floatval;
    return e;
}

/*
 * Make an expression for a string value
 */
EXP* makeEXPstringval(char* stringval) {
    EXP *e;
    e = NEW(EXP);
    e->lineno = lineno;
    e->kind = stringvalK;
    e->val.stringvalE = stringval;
    return e;
}

/*
 * Make an expression for a multiplication statement from the left and right expressions of the statement
 */
EXP *makeEXPtimes(EXP *left, EXP *right) {
    EXP *e;
    e = NEW(EXP);
    e->lineno = lineno;
    e->kind = timesK;
    e->val.timesE.left = left;
    e->val.timesE.right = right;
    return e;
}

/*
 * Make an expression for a division statement from the left and right expressions of the statement
 */
EXP *makeEXPdiv(EXP *left, EXP *right) {
    EXP *e;
    e = NEW(EXP);
    e->lineno = lineno;
    e->kind = divK;
    e->val.divE.left = left;
    e->val.divE.right = right;
    return e;
}

/*
 * Make an expression for an addition statement from the left and right expressions of the statement
 */
EXP *makeEXPplus(EXP *left, EXP *right) {
    EXP *e;
    e = NEW(EXP);
    e->lineno = lineno;
    e->kind = plusK;
    e->val.plusE.left = left;
    e->val.plusE.right = right;
    return e;
}

/*
 * Make an expression for a subtraction statement from the left and right expressions of the statement
 */
EXP *makeEXPminus(EXP *left, EXP *right) {
    EXP *e;
    e = NEW(EXP);
    e->lineno = lineno;
    e->kind = minusK;
    e->val.minusE.left = left;
    e->val.minusE.right = right;
    return e;
}

/*
 * Make an expression for a unary minus statement from the expression following the minus
 */
EXP *makeEXPunaryminus(EXP *exp) {
    EXP *e;
    e = NEW(EXP);
    e->lineno = lineno;
    e->kind = uminusK;
    e->val.uminusE = exp;
    return e;
}
