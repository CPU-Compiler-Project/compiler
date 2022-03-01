%{
#include <stdlib.h>
#include <stdio.h>

void yyerror(char *s);
%}

%token tIF tAO tAF tWHILE tFOR tPO tPF tRETURN tPV tADD tMUL tELSE tEQEQ tEQ tSUP tINF tMINUS tOR tAND tINT tCONST tVOID tNB tVAR tFL tERROR

%start Compiler
%%
Compiler : Condition Compiler | Condition;
Condition : tIF tFL { yyerror("c'est un IF!"); } ;
%%

void yyerror(char *s) { fprintf(stderr, "%s\n", s); }

int main(void) {
  printf("Compiler\n"); // yydebug=1;
  yyparse();
  return 0;
}
