%{
#include <stdlib.h>
#include <stdio.h>

void yyerror(char *s);
%}
%union {int nb; char var[16]; }
%token tMAIN tIF tAO tAF tWHILE tFOR tPO tPF tRETURN tPV tADD tMUL tELSE tEQEQ tEQ tSUP tINF tMINUS tDIV tOR tAND tINT tCONST tVOID tVAR tFL tERROR tSUPEQ tINFEQ
%token <nb> tNB

%left tADD tMINUS
%left tMUL tDIV

%start Main

%%

Body            : tAO Instructions tAF { yyerror("c'est une BODY!"); } ;
BodyInt         : tAO Instructions tRETURN tNB tPV tAF { yyerror("c'est une return!"); } ;

Instructions    : If Instructions
                  | Affectation Instructions { yyerror("c'est une affectation!"); }
                  | 
                  ;
                  
Main            : tINT tMAIN tPO tPF BodyInt | Body // TEMP, Body will be deleted there
                  ;

Affectation     : tVAR tEQ Expr tPV { yyerror("c'est u");}
                  ;

If              : tIF tPO Expr tPF Body { yyerror("c'est un IF!"); } 
                  | tIF tPO Expr tPF Body tELSE Body { yyerror("c'est un ELSE!"); }
                  ;

Expr            : tPO Expr tPF { yyerror("c'est un (expr)!"); }
                  | Expr tADD Expr { yyerror("c'est un +!"); }
                  | Expr tDIV Expr { yyerror("c'est un /!"); }
                  | Expr tMUL Expr { yyerror("c'est un *!"); }
                  | Expr tMINUS Expr { yyerror("c'est un -!"); }
                  | Expr tAND Expr { yyerror("c'est un&&-!"); }
                  | Expr tOR Expr { yyerror("c'est un ||!"); }
                  | tNB
                  | tVAR
                  ;
%%

void yyerror(char *s) { fprintf(stderr, "%s\n", s); }

int main(void) {
  // yydebug = 1;
  printf("Compiler\n");
  yyparse();
  return 0;
}
