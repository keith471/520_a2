#include <stdio.h>
#include <string.h>
#include "memory.h"
#include "error.h"
#include "symbol.h"
#include "type.h"

TYPE *intTYPE, *floatTYPE, *stringTYPE;

void initTypes() {
    intTYPE = NEW(TYPE);
    intTYPE->kind = intK;
    floatTYPE = NEW(TYPE);
    floatTYPE->kind = floatK;
    stringTYPE = NEW(TYPE);
    stringTYPE->kind = stringK;
}

void checkAssignment(TYPE *left, TYPE *right, int lineno) {
    // we can assign int to int, float to float, string to string, or int to float
    if (left->kind == right->kind) return;
    if (left->kind == floatK && right->kind == intK) return;
    if (left->kind != right->kind) reportTypeError("invalid assignment", lineno);
}

void checkCondition(TYPE *t, int lineno) {
    if (t->kind != intK) {
        reportTypeError("int type expected for condition", lineno);
    }
}

void setStringMult(EXP* exp, TYPE *left, TYPE *right) {
    if (left->kind == stringK && right->kind == intK) {
        exp->val.timesE.stringMultForward = 1;
    } else if (left->kind == intK && right->kind == stringK) {
        exp->val.timesE.stringMultReverse = 1;
    }
}

void setStringAddition(EXP* exp, TYPE *left, TYPE *right) {
    if (left->kind == stringK && right->kind == stringK) {
        exp->val.plusE.stringAddition = 1;
    }
}

void typePROGRAM(PROGRAM *p) {
    initTypes();
    // we don't need to type check the declarations
    // if there were any invalid types, the scanner would have caught them right away!
    typeSTATEMENT(p->statements);
}

void typeSTATEMENT(STATEMENT* statement) {
    if (statement == NULL) return;
    switch (statement->kind) {
        case assignK:
            // set the type of the expression to the right
            typeEXP(statement->val.assignS.exp);
            // compare to the type of the identifier to the left
            checkAssignment(statement->val.assignS.id->symbol->type, statement->val.assignS.exp->type, statement->lineno);
            break;
        case fcallK:
            typeFUNCTIONCALL(statement->val.fcallS);
            break;
        case ifK:
            typeEXP(statement->val.ifS.condition);
            checkCondition(statement->val.ifS.condition->type, statement->lineno);
            typeSTATEMENT(statement->val.ifS.body);
            break;
        case ifelseK:
            typeEXP(statement->val.ifelseS.condition);
            checkCondition(statement->val.ifelseS.condition->type, statement->lineno);
            typeSTATEMENT(statement->val.ifelseS.thenpart);
            typeSTATEMENT(statement->val.ifelseS.elsepart);
            break;
        case whileK:
            typeEXP(statement->val.whileS.condition);
            checkCondition(statement->val.whileS.condition->type, statement->lineno);
            typeSTATEMENT(statement->val.whileS.body);
            break;
    }
    typeSTATEMENT(statement->next);
}

void typeFUNCTIONCALL(FUNCTIONCALL* fc) {
    switch (fc->kind) {
        case readK:
            // can read into a variable of any type
            break;
        case printK:
            typeEXP(fc->val.printexp);
            break;
    }
}

/*
 * sets the type of an expression
 */
void typeEXP(EXP* exp) {
    switch (exp->kind) {
        case idK:
            exp->type = exp->val.idE->symbol->type;
            break;
        case intvalK:
            exp->type = intTYPE;
            break;
        case floatvalK:
            exp->type = floatTYPE;
            break;
        case stringvalK:
            exp->type = stringTYPE;
            break;
        case timesK:
            typeEXP(exp->val.timesE.left);
            typeEXP(exp->val.timesE.right);
            setStringMult(exp, exp->val.timesE.left->type, exp->val.timesE.right->type);
            exp->type = getTimesExpType(exp->val.timesE.left->type, exp->val.timesE.right->type, exp->lineno);
            break;
        case divK:
            typeEXP(exp->val.divE.left);
            typeEXP(exp->val.divE.right);
            exp->type = getDivExpType(exp->val.divE.left->type, exp->val.divE.right->type, exp->lineno);
            break;
        case plusK:
            typeEXP(exp->val.plusE.left);
            typeEXP(exp->val.plusE.right);
            setStringAddition(exp, exp->val.plusE.left->type, exp->val.plusE.right->type);
            exp->type = getPlusExpType(exp->val.plusE.left->type, exp->val.plusE.right->type, exp->lineno);
            break;
        case minusK:
            typeEXP(exp->val.minusE.left);
            typeEXP(exp->val.minusE.right);
            exp->type = getMinusExpType(exp->val.minusE.left->type, exp->val.minusE.right->type, exp->lineno);
            break;
        case uminusK:
            typeEXP(exp->val.uminusE);
            exp->type = getUnaryMinusExpType(exp->val.uminusE->type, exp->lineno);
            break;
    }
}

/*
 * Valid binary expression types
 * valid for all operators
 *  int int -> int
 *  float float -> float
 *  int float -> float
 *  float int -> float
 *
 * valid for plus
 *  string + string -> string (string concatenation)
 *
 * valid for mult
 *  string * int -> string
 *  int * string -> string
 */

TYPE* getExpType(TYPE* left, TYPE* right) {
    if (left->kind == intK && right->kind == intK) return intTYPE;
    if (left->kind == floatK && right->kind == floatK) return floatTYPE;
    if (left->kind == intK && right->kind == floatK) return floatTYPE;
    if (left->kind == floatK && right->kind == intK) return floatTYPE;
    return NULL;
}

TYPE* getTimesExpType(TYPE *left, TYPE* right, int lineno) {
    TYPE* t;
    t = getExpType(left, right);
    if (t == NULL) {
        // the only other valid multiplications are int*string or string*int
        if (left->kind == stringK && right->kind == intK) return stringTYPE;
        if (left->kind == intK && right->kind == stringK) return stringTYPE;
        // error
        reportTypeError("invalid types for multiplication", lineno);
    }
    return t;
}

TYPE* getDivExpType(TYPE* left, TYPE* right, int lineno) {
    TYPE* t;
    t = getExpType(left, right);
    if (t == NULL) {
        reportTypeError("invalid types for division", lineno);
    }
    return t;
}

TYPE* getPlusExpType(TYPE* left, TYPE* right, int lineno) {
    TYPE* t;
    t = getExpType(left, right);
    if (t == NULL) {
        // only other valid addition is string concatentation
        if (left->kind == stringK && right->kind == stringK) return stringTYPE;
        // error
        reportTypeError("invalid types for addition", lineno);
    }
    return t;
}

TYPE* getMinusExpType(TYPE* left, TYPE* right, int lineno) {
    TYPE* t;
    t = getExpType(left, right);
    if (t == NULL) {
        reportTypeError("invalid types for subtraction", lineno);
    }
    return t;
}

/*
 * Valid unary minus expression types
 *  int -> int
 *  float -> float
 */

TYPE* getUnaryMinusExpType(TYPE* exp, int lineno) {
    if (exp->kind == intK) return intTYPE;
    if (exp->kind == floatK) return floatTYPE;
    // error
    reportTypeError("invalid type for unary minus", lineno);
    return NULL;
}
