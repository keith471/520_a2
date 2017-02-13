
void yyerror(char *s);
void reportTypeError(char *s, int lineno);
void reportIdentifierError(char *s, char *name, int lineno);
void terminateIfErrors();
