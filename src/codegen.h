

void addHeaderCode(FILE* emitFILE);

void genPROGRAM(PROGRAM* program, char* fname);
void genDECLARATION(DECLARATION* declaration, int level, FILE* emitFILE);
void genSTATEMENT(STATEMENT* statement, int level, FILE* emitFILE);
void genSTATEMENTfunccall(STATEMENT* s, int level, FILE* emitFILE);
void genREAD(ID* id, FILE* emitFILE);
void genPRINT(EXP* exp, FILE* emitFILE);
void genSTATEMENTassign(STATEMENT* s, int level, FILE* emitFILE);
void genSTATEMENTif(STATEMENT* s, int level, FILE* emitFILE);
void genSTATEMENTifelse(STATEMENT* s, int level, FILE* emitFILE);
void genSTATEMENTwhile(STATEMENT* s, int level, FILE* emitFILE);
void genEXP(EXP* exp, FILE* emitFILE);
