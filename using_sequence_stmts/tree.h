/*
 * This file is for all the AST node definitions
 */

#ifndef TREE_H
#define TREE_H

/*
C structs
- A datatype that allows us to combine data items of different kinds
- defined as follows
    struct [structure tag] {
        member definition;
        ...
        member definition;
    } [one or more structure variables];
- the structure tag is optional
- accessing structure members
    - use the member access operator ('.'), e.g.
    struct Books {
       char  title[50];
       char  author[50];
       char  subject[100];
       int   book_id;
    };
    // then later...
    struct Books Book1;
    strcpy( Book1.title, "C Programming");
    strcpy( Book1.author, "Nuha Ali");
    strcpy( Book1.subject, "C Programming Tutorial");
    Book1.book_id = 6495407;
- but if you have a pointer to a struct, then you access the members with ->, e.g.
    struct Books *struct_pointer;
    struct_pointer = &Book1;
    struct_pointer->title;
*/

/*
 * a program consists of declarations and statements
 */
typedef struct PROGRAM {
    struct DECLARATION* declarations;
    struct STATEMENT* statements;
} PROGRAM;

/*
 * Type
 */
typedef struct TYPE {
  int lineno;
  enum {intK, floatK, stringK} kind;
} TYPE;

/*
 * a declaration
 */
typedef struct DECLARATION {
  int lineno;
  struct ID* id;
  struct TYPE *type;
  struct DECLARATION *next;
} DECLARATION;

/*
 * a statement
 */
typedef struct STATEMENT {
    int lineno;
    enum {sequenceK, assignK, funccallK, ifK, ifelseK, whileK} kind;
    union{
        struct FUNCTIONCALL *fcall;
        struct {struct ID *id;
                //SYMBOL *leftsym; /* symbol */
                struct EXP *exp;} assignS;
        struct {struct STATEMENT *first;
                struct STATEMENT *second;} sequenceS;
        struct {struct EXP *condition;
                struct STATEMENT *body;
                int stoplabel; /* resource */} ifS;
        struct {struct EXP *condition;
                struct STATEMENT *thenpart;
                struct STATEMENT *elsepart;
                int elselabel,stoplabel; /* resource */} ifelseS;
        struct {struct EXP *condition;
                struct STATEMENT *body;
                /*int startlabel, stoplabel;*/ /* resource */} whileS;
    } val;
} STATEMENT;

/*
 * Function call (read or print)
 * We encapsulate this as such since we only permit two function calls, so it need not be generic
 */
typedef struct FUNCTIONCALL {
    int lineno;
    enum {readK, printK} kind;
    union {
        struct ID* readid;
        struct EXP* printexp;
    } val;
} FUNCTIONCALL;

/*
 * identifier
 */
typedef struct ID {
    char *name;
} ID;

/*
 * An expression
 */
typedef struct EXP {
    int lineno;
    // the kind of this expression
    enum { idK, intvalK, floatvalK, stringvalK, timesK, divK, plusK, minusK, uminusK } kind;
    // this expression is one of the following types
    // to access any member of a union, we use a period: val.timesE
    union {
        struct ID *idE;  // identifier
        int intvalE;  // value of integer
        float floatvalE;    // value of float
        char *stringvalE;   // value of string
        struct {struct EXP *left; struct EXP *right;} timesE;
        struct {struct EXP *left; struct EXP *right;} divE;
        struct {struct EXP *left; struct EXP *right;} plusE;
        struct {struct EXP *left; struct EXP *right;} minusE;
        struct EXP *uminusE;    // the expression following the unary minus
    } val;
} EXP;

/*
PROGRAM* makePROGRAM(EXP* exp);
*/
PROGRAM* makePROGRAM(DECLARATION* declarations, STATEMENT* statements);

DECLARATION *appendDECLARATION(DECLARATION *f, DECLARATION *g);
DECLARATION *makeDECLARATION(ID *id, TYPE *type);

STATEMENT *makeSTATEMENTsequence(STATEMENT *first, STATEMENT *second);
STATEMENT *makeSTATEMENTassign(ID *id, EXP *exp);
STATEMENT *makeSTATEMENTfunccall(FUNCTIONCALL* fc);
STATEMENT *makeSTATEMENTif(EXP *condition, STATEMENT *body);
STATEMENT *makeSTATEMENTifelse(EXP *condition, STATEMENT *thenpart, STATEMENT *elsepart);
STATEMENT *makeSTATEMENTwhile(EXP* condition, STATEMENT* body);

FUNCTIONCALL *makeFUNCTIONCALLread(ID* id);
FUNCTIONCALL *makeFUNCTIONCALLprint(EXP* exp);

ID *makeID(char *name);

TYPE *makeTYPEint();
TYPE *makeTYPEfloat();
TYPE *makeTYPEstring();

EXP *makeEXPid(ID *id);
EXP *makeEXPintval(int intval);
EXP *makeEXPfloatval(float floatval);
EXP *makeEXPstringval(char* stringval);
EXP *makeEXPtimes(EXP *left, EXP *right);
EXP *makeEXPdiv(EXP *left, EXP *right);
EXP *makeEXPplus(EXP *left, EXP *right);
EXP *makeEXPminus(EXP *left, EXP *right);
EXP *makeEXPunaryminus(EXP *exp);

#endif /* !TREE_H */
