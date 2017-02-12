
void yyerror(char *s);
void reportError(char *s, int lineno);
void reportIdentifierError(char *s, char *name, int lineno);
void terminateIfErrors();
