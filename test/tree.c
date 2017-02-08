/*
 * This file is for all the AST node constructors
 */

#include <stdio.h>
#include <string.h>
#include "memory.h"
#include "tree.h"

extern int yylineno;

/*
PROGRAM* makePROGRAM(EXP* exp) {
    PROGRAM* p;
    p = NEW(PROGRAM);
    p->exp = exp;
    return p;
}
*/

PROGRAM* makePROGRAM(DECLARATION* declarations, STATEMENT* statements) {
    PROGRAM* p;
    p = NEW(PROGRAM);
    p->declarations = declarations;
    p->statements = statements;
    return p;
}

/* BACKWARDS VERSION
 * Append a declaration node to the list of declarations
 * f is the new declaration
 * g is the list of all previous declarations
 */
/*
DECLARATION *appendDECLARATION(DECLARATION *f, DECLARATION *g) {
    DECLARATION *t;
    // if no more declarations, return g
    if (f == NULL) return g;
    // keep a reference to the new declaration, which will become the head of the list
    t = f;
    // move to the end of the new linkedlist of declarations given by f
    while (t->next != NULL) t = t->next;
    // set t to be the head of the list
    t->next = g;
    // return the head of the list
    return f;
}
*/


/*
 * Append a declaration node to the list of declarations
 * f is the list of previous declarations
 * g is the new declaration
 */
DECLARATION *appendDECLARATION(DECLARATION *f, DECLARATION *g) {
    DECLARATION *t;
    // f will be null for the first declaration that we parse
    if (f == NULL) return g;
    // get a second reference, t, to the head of the list
    t = f;
    // move to the end of the linkedlist of declarations given by f
    while (t->next != NULL) t = t->next;
    // append g to the end of the list
    t->next = g;
    // return the head of the list
    return f;
}

/*
 * Make a declaration
 */
DECLARATION *makeDECLARATION(ID *id, TYPE *type) {
    DECLARATION *d;
    d = NEW(DECLARATION);
    d->lineno = yylineno;
    d->id = id;
    d->type = type;
    d->next = NULL;
    return d;
}

/*
 * Sequence of statements ---???? QUESTION: What is a sequence statement? Why do we need first and second?
 */
STATEMENT *makeSTATEMENTsequence(STATEMENT *first, STATEMENT *second) {
    STATEMENT *s;
    if (first == NULL) return second;
    if (second == NULL) return first;
    s = NEW(STATEMENT);
    s->lineno = yylineno;
    s->kind = sequenceK;
    s->val.sequenceS.first = first;
    s->val.sequenceS.second = second;
    return s;
}

/*
 * Make in identifier node
 */
ID *makeID(char *name) {
    ID *i;
    i = NEW(ID);
    i->name = name;
    return i;
}

/*
 * Make a node for type int
 */
TYPE *makeTYPEint() {
    TYPE *t;
    t = NEW(TYPE);
    t->lineno = yylineno;
    t->kind = intK;
    return t;
}

/*
 * Make a node for type float
 */
TYPE *makeTYPEfloat() {
    TYPE *t;
    t = NEW(TYPE);
    t->lineno = yylineno;
    t->kind = floatK;
    return t;
}

/*
 * Make a node for type string
 */
TYPE *makeTYPEstring() {
    TYPE *t;
    t = NEW(TYPE);
    t->lineno = yylineno;
    t->kind = stringK;
    return t;
}

/*
 * Make an expression for an identifier
 */
EXP* makeEXPid(char *id) {
    EXP *e;
    e = NEW(EXP);
    e->lineno = yylineno;
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
    e->lineno = yylineno;
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
    e->lineno = yylineno;
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
    e->lineno = yylineno;
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
    e->lineno = yylineno;
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
    e->lineno = yylineno;
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
    e->lineno = yylineno;
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
    e->lineno = yylineno;
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
    e->lineno = yylineno;
    e->kind = uminusK;
    e->val.uminusE = exp;
    return e;
}
