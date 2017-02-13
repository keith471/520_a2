#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "codegen.h"
#include "outputhelpers.h"

void addHeaderCode(FILE* emitFILE) {
   FILE* headerFILE;
   char c;

   headerFILE = fopen("headercode.txt", "r");

   while ((c = fgetc(headerFILE)) != EOF) {
       fputc(c, emitFILE);
   }

   fclose(headerFILE);
}

void genPROGRAM(PROGRAM* p, char* fname) {
    FILE* emitFILE = fopen(fname, "w");
    addHeaderCode(emitFILE);
    newLineInFile(emitFILE);
    // now, all we have to do is construct the main function
    // our language doesn't support command line arguments, so main need not take any parameters
    fprintf(emitFILE, "int main() {");
    newLineInFile(emitFILE);
    // traverse the AST, writing each line's equivalent C code to emitFILE
    genDECLARATION(p->declarations, 1, emitFILE);
    newLineInFile(emitFILE);
    genSTATEMENT(p->statements, 1, emitFILE);
    newLineInFile(emitFILE);
    // exit with status code 0 and close off main
    fprintf(emitFILE, "    exit(0);");
    newLineInFile(emitFILE);
    fprintf(emitFILE, "}");
    newLineInFile(emitFILE);
    // close emitFILE
    fclose(emitFILE);
}

void genDECLARATION(DECLARATION* declaration, int level, FILE* emitFILE) {
    if (declaration == NULL) return;
    printTabsToFile(level, emitFILE);
    switch (declaration->type->kind) {
        case intK:
            fprintf(emitFILE, "int %s = 0;", declaration->id->name);
            break;
        case floatK:
            fprintf(emitFILE, "float %s = 0.0;", declaration->id->name);
            break;
        case stringK:
            fprintf(emitFILE, "char* %s = \"\";", declaration->id->name);
            break;
    }
    newLineInFile(emitFILE);
    genDECLARATION(declaration->next, level, emitFILE);
}

void genSTATEMENT(STATEMENT* statement, int level, FILE* emitFILE) {
    if (statement == NULL) return;
    switch (statement->kind) {
        case assignK:
            genSTATEMENTassign(statement, level, emitFILE);
            break;
        case fcallK:
            genSTATEMENTfunccall(statement, level, emitFILE);
            break;
        case ifK:
            genSTATEMENTif(statement, level, emitFILE);
            break;
        case ifelseK:
            genSTATEMENTifelse(statement, level, emitFILE);
            break;
        case whileK:
            genSTATEMENTwhile(statement, level, emitFILE);
            break;
    }
    genSTATEMENT(statement->next, level, emitFILE);
}

void genSTATEMENTif(STATEMENT* s, int level, FILE* emitFILE) {
    printTabsToFile(level, emitFILE);
    fprintf(emitFILE, "if (");
    genEXP(s->val.ifS.condition, emitFILE);
    fprintf(emitFILE, ") {");
    newLineInFile(emitFILE);
    genSTATEMENT(s->val.ifS.body, level + 1, emitFILE);
    printTabsToFile(level, emitFILE);
    fprintf(emitFILE, "}");
    newLineInFile(emitFILE);
}

void genSTATEMENTifelse(STATEMENT* s, int level, FILE* emitFILE) {
    printTabsToFile(level, emitFILE);
    fprintf(emitFILE, "if (");
    genEXP(s->val.ifelseS.condition, emitFILE);
    fprintf(emitFILE, ") {");
    newLineInFile(emitFILE);
    genSTATEMENT(s->val.ifelseS.thenpart, level + 1, emitFILE);
    printTabsToFile(level, emitFILE);
    fprintf(emitFILE, "} else {");
    newLineInFile(emitFILE);
    genSTATEMENT(s->val.ifelseS.elsepart, level + 1, emitFILE);
    printTabsToFile(level, emitFILE);
    fprintf(emitFILE, "}");
    newLineInFile(emitFILE);
}

void genSTATEMENTwhile(STATEMENT* s, int level, FILE* emitFILE) {
    printTabsToFile(level, emitFILE);
    fprintf(emitFILE, "while (");
    genEXP(s->val.whileS.condition, emitFILE);
    fprintf(emitFILE, ") {");
    newLineInFile(emitFILE);
    genSTATEMENT(s->val.whileS.body, level + 1, emitFILE);
    printTabsToFile(level, emitFILE);
    fprintf(emitFILE, "}");
    newLineInFile(emitFILE);
}

void genSTATEMENTassign(STATEMENT* s, int level, FILE* emitFILE) {
    printTabsToFile(level, emitFILE);
    fprintf(emitFILE, "%s = ", s->val.assignS.id->name);
    genEXP(s->val.assignS.exp, emitFILE);
    fprintf(emitFILE, ";");
    newLineInFile(emitFILE);
}

void genSTATEMENTfunccall(STATEMENT* s, int level, FILE* emitFILE) {
    printTabsToFile(level, emitFILE);
    FUNCTIONCALL* fc = s->val.fcallS;
    switch (fc->kind) {
        case readK:
            genREAD(fc->val.readid, emitFILE);
            break;
        case printK:
            genPRINT(fc->val.printexp, emitFILE);
            break;
    }
    fprintf(emitFILE, ";");
    newLineInFile(emitFILE);
}

void genREAD(ID* id, FILE* emitFILE) {
    switch (id->symbol->type->kind) {
        case intK:
            fprintf(emitFILE, "scanf(\"%%d\", %s)", id->name);
            break;
        case floatK:
            fprintf(emitFILE, "scanf(\"%%f\", %s)", id->name);
            break;
        case stringK:
            fprintf(emitFILE, "scanf(\"%%s\", %s)", id->name);
            break;
    }
}

void genPRINT(EXP* exp, FILE* emitFILE) {
    fprintf(emitFILE, "printf(");
    switch (exp->type->kind) {
        case intK:
            fprintf(emitFILE, "\"%%d\", ");
            genEXP(exp, emitFILE);
            break;
        case floatK:
            fprintf(emitFILE, "\"%%f\", ");
            genEXP(exp, emitFILE);
            break;
        case stringK:
            fprintf(emitFILE, "\"%%s\", ");
            genEXP(exp, emitFILE);
            break;
    }
    fprintf(emitFILE, ")");
}

void genEXP(EXP* e, FILE* emitFILE) {
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
            // check if this is string multiplication
            if (e->val.timesE.stringMultForward) {
                fprintf(emitFILE, "mulstr1(");
                genEXP(e->val.timesE.left, emitFILE);
                fprintf(emitFILE, ", ");
                genEXP(e->val.timesE.right, emitFILE);
                fprintf(emitFILE, ")");
            } else if (e->val.timesE.stringMultReverse) {
                fprintf(emitFILE, "mulstr2(");
                genEXP(e->val.timesE.left, emitFILE);
                fprintf(emitFILE, ", ");
                genEXP(e->val.timesE.right, emitFILE);
                fprintf(emitFILE, ")");
            } else {
                fprintf(emitFILE, "(");
                // recursively gen the left expression
                genEXP(e->val.timesE.left, emitFILE);
                // print the times
                fprintf(emitFILE, "*");
                // recursively gen the right expression
                genEXP(e->val.timesE.right, emitFILE);
                fprintf(emitFILE, ")");
            }
            break;
        case divK:
            fprintf(emitFILE, "(");
            genEXP(e->val.divE.left, emitFILE);
            fprintf(emitFILE, "/");
            genEXP(e->val.divE.right, emitFILE);
            fprintf(emitFILE, ")");
            break;
        case plusK:
            // check if this is string addition (concatenation)
            if (e->val.plusE.stringAddition) {
                fprintf(emitFILE, "concat(");
                genEXP(e->val.plusE.left, emitFILE);
                fprintf(emitFILE, ", ");
                genEXP(e->val.plusE.right, emitFILE);
                fprintf(emitFILE, ")");
            } else {
                fprintf(emitFILE, "(");
                genEXP(e->val.plusE.left, emitFILE);
                fprintf(emitFILE, "+");
                genEXP(e->val.plusE.right, emitFILE);
                fprintf(emitFILE, ")");
            }
            break;
        case minusK:
            fprintf(emitFILE, "(");
            genEXP(e->val.minusE.left, emitFILE);
            fprintf(emitFILE, "-");
            genEXP(e->val.minusE.right, emitFILE);
            fprintf(emitFILE, ")");
            break;
        case uminusK:
            fprintf(emitFILE, "(");
            fprintf(emitFILE, "-");
            genEXP(e->val.uminusE, emitFILE);
            fprintf(emitFILE, ")");
            break;
    }
}
