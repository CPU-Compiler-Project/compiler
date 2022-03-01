%{
#include <stdlib.h>
#include <stdio.h>

void yyerror(char *s);
%}

%token tIF tAO tAF tWHILE tFOR tPO tPF tRETURN tPV tADD tMUL tELSE tEQEQ tEQ tSUP tINF tMINUS tOR tAND tINT tCONST tVOID tVAR tFL tERROR
%token <nb> tNB
%start Compiler
%%
Compiler : Condition Compiler | Condition;
Condition : tIF tPO BOOL tPF EXPR  { yyerror("c'est un IF!"); } 
          | tIF tPO BOOL tPF EXPR tELSE EXPR { yyerror("c'est un ELSE!"); }
%%

void yyerror(char *s) { fprintf(stderr, "%s\n", s); }

int main(void) {
  printf("Compiler\n"); // yydebug=1;
  yyparse();
  return 0;
}
