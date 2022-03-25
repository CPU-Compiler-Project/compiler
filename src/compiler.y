%{
#include <stdlib.h>
#include <stdio.h>

#include "functions.h"

void yyerror(char *s);
%}
%union { int nb; int *addr; char *var; }
%token tPRINTF tCOMMA tMAIN tIF tAO tAF tWHILE tFOR tPO tPF tRETURN tPV tADD tMUL tELSE tEQEQ tEQ tSUP tINF tMINUS tDIV tOR tAND tINT tCONST tVOID tFL tERROR tSUPEQ tINFEQ
%token <nb> tNB tNBEXP
%token <var> tVAR
%type <addr> Int Expr Var

%right tEQ
%left tADD tMINUS
%left tMUL tDIV

%start Main

%%

Body            : tAO Instructions tAF { yyerror("c'est une BODY!"); } ;
BodyInt         : tAO Instructions tRETURN tNB tPV tAF { yyerror("c'est une return!"); } ;

Instructions    : If 
                  | AffectationEdit tPV  { yyerror("c'est une affectation!"); }
                  | While
                  | Declaration tPV
                  | Printf tPV
                  | Instructions Instructions
                  ;
                  
Main            : tINT tMAIN tPO tPF BodyInt | Body // TEMP, Body will be deleted there
                  ;

Expr            : tPO Expr tPF { yyerror("c'est un (expr)!"); $$ = $2; }
                  | Expr tADD Expr { yyerror("c'est un +!"); $$ = addition($1,$3); }
                  | Expr tMINUS Expr { yyerror("c'est un -!"); $$ = substraction($1,$3); }
                  | Expr tDIV Expr { yyerror("c'est un /!"); $$ = divide($1,$3); }
                  | Expr tMUL Expr { yyerror("c'est un *!"); $$ = multiply($1,$3); }
                  | Expr tAND Expr { yyerror("c'est un&&-!"); $$ = andOp($1,$3); }
                  | Expr tOR Expr { yyerror("c'est un ||!"); $$ = orOp($1,$3); }
                  | Int
                  | Var
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

Affectation     : tVAR tEQ Expr { $<nb>$ = createVar($1); printf("%p", $1); yyerror("c'est u"); }
                  ;

AffectationEdit : Var tEQ Expr { $<nb>$ = editVar($1, $3); printf("%p", $1); yyerror("c'est u"); }
                  ;

If              : tIF tPO Expr tPF Body { yyerror("c'est un IF!"); } 
                  | tIF tPO Expr tPF Body tELSE If { yyerror("c'est un ELSEIF!"); }
                  | tIF tPO Expr tPF Body tELSE Body { yyerror("c'est un ELSE!"); }
                  ;

Printf          : tPRINTF tPO Var tPF { printf("%p", $3); yyerror("text var"); }
                  ;

Var             : tVAR { yyerror("var"); $$ = getAddress($1); }

Int             : tNB { yyerror("int"); $$ = allocate($1); }
                  | tNBEXP { yyerror("exp"); $$ = allocate($1); }
                  | tMINUS tNB { yyerror("nb_negatif"); $$ = allocate(-$2); }
                  | tMINUS tNBEXP { yyerror("nb_negatif"); $$ = allocate(-$2); }
                  ;

%%

void yyerror(char *s) { fprintf(stderr, "%s\n", s); }

int main(void) {
  //yydebug = 1;
  printf("Compiler\n");
  stack = malloc(sizeof(Stack));
  initFile();
  yyparse();
  closeFile();

  return 0;
}
