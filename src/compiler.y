%{
#include <stdlib.h>
#include <stdio.h>

void yyerror(char *s);
%}
%union { int nb; }
%token tNBEXP tPRINTF tCOMMA tMAIN tIF tAO tAF tWHILE tFOR tPO tPF tRETURN tPV tADD tMUL tELSE tEQEQ tEQ tSUP tINF tMINUS tDIV tOR tAND tINT tCONST tVOID tVAR tFL tERROR tSUPEQ tINFEQ
%token <nb> tNB

%right tEQ
%left tADD tMINUS
%left tMUL tDIV

%start Main

%%

Body            : tAO Instructions tAF { yyerror("c'est une BODY!"); } ;
BodyInt         : tAO Instructions tRETURN tNB tPV tAF { yyerror("c'est une return!"); } ;

Instructions    : If 
                  | Affectation tPV  { yyerror("c'est une affectation!"); }
                  | While
                  | Declaration tPV
                  | Printf tPV
                  | Instructions Instructions
                  ;
                  
Main            : tINT tMAIN tPO tPF BodyInt | Body // TEMP, Body will be deleted there
                  ;

Expr            : tPO Expr tPF { yyerror("c'est un (expr)!"); }
                  | Expr tADD Expr { yyerror("c'est un +!"); }
                  | Expr tMINUS Expr { yyerror("c'est un -!"); }
                  | Expr tDIV Expr { yyerror("c'est un /!"); }
                  | Expr tMUL Expr { yyerror("c'est un *!"); }
                  | Expr tAND Expr { yyerror("c'est un&&-!"); }
                  | Expr tOR Expr { yyerror("c'est un ||!"); }
                  | Int
                  | tVAR
                  ;

While           : tWHILE tPO Expr tPF Body
                  ;

AffectationOpt  : tVAR
                  | Affectation
                  ;

MultVar         : AffectationOpt
                  | AffectationOpt tCOMMA MultVar
                  ;

ConstOpt        : tCONST
                  |
                  ;

Declaration     : ConstOpt tINT MultVar
                  ;

Affectation     : tVAR tEQ Expr { yyerror("c'est u"); }
                  ;

If              : tIF tPO Expr tPF Body { yyerror("c'est un IF!"); } 
                  | tIF tPO Expr tPF Body tELSE If { yyerror("c'est un ELSEIF!"); }
                  | tIF tPO Expr tPF Body tELSE Body { yyerror("c'est un ELSE!"); }
                  ;

Printf          : tPRINTF tPO tPF { yyerror("TODO"); } // TODO
                  ;

Int             : tNB { yyerror("int"); }
                  | tNBEXP { yyerror("exp"); }
                  | tMINUS Int { yyerror("nb_negatif"); }
                  ;

%%

void yyerror(char *s) { fprintf(stderr, "%s\n", s); }

int main(void) {
  // yydebug = 1;
  printf("Compiler\n");
  yyparse();
  return 0;
}
