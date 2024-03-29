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
 * Symbol
 */
typedef struct SYMBOL {
    char *name;
    struct TYPE* type;
    // this is a linked list in the SymbolTable hashmap, so we have to have this next field.
    // it doesn't actually have anything to do with the 'current' symbol
    struct SYMBOL *next;
} SYMBOL;

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
     enum {assignK, fcallK, ifK, ifelseK, whileK} kind;
     // points to the next statement at the same level as this one
     // nested statements are pointed to by the appropriate statement structs in val
     struct STATEMENT* next;
     union{
         struct FUNCTIONCALL *fcallS;
         struct {struct ID *id;
                 struct EXP *exp;} assignS;
         struct {struct EXP *condition;
                 struct STATEMENT *body;} ifS;
         struct {struct EXP *condition;
                 struct STATEMENT *thenpart;
                 struct STATEMENT *elsepart;} ifelseS;
         struct {struct EXP *condition;
                 struct STATEMENT *body;} whileS;
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
 * every identifier is associated with a SYMBOL
 */
typedef struct ID {
    char *name;
    struct SYMBOL* symbol;
} ID;

/*
 * An expression
 */
typedef struct EXP {
    int lineno;
    // the type that this expression evaluates to
    struct TYPE* type;
    // the kind of this expression
    enum { idK, intvalK, floatvalK, stringvalK, timesK, divK, plusK, minusK, uminusK } kind;
    // this expression is one of the following types
    // to access any member of a union, we use a period: val.timesE
    union {
        struct ID *idE;  // identifier
        int intvalE;  // value of integer
        float floatvalE;    // value of float
        char *stringvalE;   // value of string
        struct {struct EXP *left;
                struct EXP *right;
                int stringMultForward; /* 1 if string * int */
                int stringMultReverse; /* 1 if int * string */} timesE;
        struct {struct EXP *left;
                struct EXP *right;
                int stringAddition; /* 1 if string addition */} plusE;
        struct {struct EXP *left; struct EXP *right;} divE;
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

STATEMENT *appendSTATEMENT(STATEMENT* prevs, STATEMENT *curr);
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
