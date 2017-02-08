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

typedef struct EXP {
  int lineno;
  // the kind of this expression
  enum { idK, intvalK, floatvalK, stringvalK, timesK, divK, plusK, minusK, uminusK } kind;
  // this expression is one of the following types
  // to access any member of a union, we use a period: val.timesE
  union {
    char *idE;  // identifier
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

EXP *makeEXPid(char *id);

EXP *makeEXPintval(int intval);

EXP *makeEXPfloatval(float floatval);

EXP *makeEXPstringval(char* stringval);

EXP *makeEXPtimes(EXP *left, EXP *right);

EXP *makeEXPdiv(EXP *left, EXP *right);

EXP *makeEXPplus(EXP *left, EXP *right);

EXP *makeEXPminus(EXP *left, EXP *right);

EXP *makeEXPunaryminus(EXP *exp);

#endif /* !TREE_H */
