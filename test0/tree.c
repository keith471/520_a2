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

STATEMENT *appendSTATEMENT(STATEMENT* prevs, STATEMENT *curr) {
    STATEMENT* t;
    if (prevs == NULL) return curr;
    t = prevs;
    while (t->next != NULL) t = t->next;
    t->next = curr;
    return prevs;
}

/*
 * Sequence of statements ---???? QUESTION: What is a sequence statement? Why do we need first and second?
 */
/*
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
*/

STATEMENT *makeSTATEMENTassign(ID *id, EXP *exp) {
    STATEMENT *s;
    s = NEW(STATEMENT);
    s->lineno = yylineno;
    s->kind = assignK;
    s->next = NULL;
    s->val.assignS.id = id;
    s->val.assignS.exp = exp;
    return s;
}

STATEMENT *makeSTATEMENTfunccall(FUNCTIONCALL* fc) {
    STATEMENT *s;
    s = NEW(STATEMENT);
    s->lineno = yylineno;
    s->kind = fcallK;
    s->val.fcallS = fc;
    return s;
}

FUNCTIONCALL *makeFUNCTIONCALLread(ID* id) {
    FUNCTIONCALL *f;
    f = NEW(FUNCTIONCALL);
    f->lineno = yylineno;
    f->kind = readK;
    f->val.readid = id;
    return f;
}

FUNCTIONCALL *makeFUNCTIONCALLprint(EXP* exp) {
    FUNCTIONCALL *f;
    f = NEW(FUNCTIONCALL);
    f->lineno = yylineno;
    f->kind = printK;
    f->val.printexp = exp;
    return f;
}

STATEMENT *makeSTATEMENTwhile(EXP* condition, STATEMENT* body) {
    STATEMENT *s;
    s = NEW(STATEMENT);
    s->lineno = yylineno;
    s->kind = whileK;
    s->next = NULL;
    s->val.whileS.condition = condition;
    s->val.whileS.body = body;
    return s;
}

STATEMENT *makeSTATEMENTif(EXP *condition, STATEMENT *body) {
    STATEMENT *s;
    s = NEW(STATEMENT);
    s->lineno = yylineno;
    s->kind = ifK;
    s->next = NULL;
    s->val.ifS.condition = condition;
    s->val.ifS.body = body;
    return s;
}

STATEMENT *makeSTATEMENTifelse(EXP *condition, STATEMENT *thenpart, STATEMENT *elsepart) {
    STATEMENT *s;
    s = NEW(STATEMENT);
    s->lineno = yylineno;
    s->kind = ifelseK;
    s->next = NULL;
    s->val.ifelseS.condition = condition;
    s->val.ifelseS.thenpart = thenpart;
    s->val.ifelseS.elsepart = elsepart;
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
EXP* makeEXPid(ID *id) {
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
    e->val.timesE.stringMultForward = 0;   /* set to false for now; if true, will be set in type checking stage */
    e->val.timesE.stringMultReverse = 0;   /* set to false for now; if true, will be set in type checking stage */
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
    e->val.plusE.stringAddition = 0; /* set to false for now; if true, will be set in type checking stage */
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
