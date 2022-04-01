%{
#include <stdlib.h>
#include <stdio.h>

#include "functions.h"

void yyerror(char *s);
%}
%union { int nb; Stack *addr; char *var; }
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
                  
Main            : tINT tMAIN tPO tPF BodyInt { incr_depth(); }| Body // TEMP, Body will be deleted there
                  ;

Expr            : tPO Expr tPF { yyerror("c'est un (expr)!"); $$ = $2; }
                  | Expr tADD Expr { yyerror("c'est un +!"); addition(); }
                  | Expr tMINUS Expr { yyerror("c'est un -!"); substraction(); }
                  | Expr tDIV Expr { yyerror("c'est un /!"); divide(); }
                  | Expr tMUL Expr { yyerror("c'est un *!"); multiply(); }
                  | Expr tAND Expr { yyerror("c'est un&&-!"); andOp(); }
                  | Expr tOR Expr { yyerror("c'est un ||!"); orOp(); }
                  | Int
                  | Var
                  ;

While           : tWHILE tPO Expr tPF Body
                  ;

AffectationOpt  : tVAR { $<nb>$ = createVar($1); yyerror("c'est CREER"); }
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

Affectation     : tVAR { $<nb>$ = createVar($1); yyerror("c'est CREER"); } tEQ Expr { $<nb>$ = editVar(getAddress($1)); yyerror("c'est INITIER"); }
                  ;

AffectationEdit : Var tEQ Expr { $<nb>$ = editVar($1); yyerror("c'est EDITER"); }
                  ;

If              : tIF tPO Expr tPF Body { yyerror("c'est un IF!"); } 
                  | tIF tPO Expr tPF Body tELSE If { yyerror("c'est un ELSEIF!"); }
                  | tIF tPO Expr tPF Body tELSE Body { yyerror("c'est un ELSE!"); }
                  ;

Printf          : tPRINTF tPO Var tPF { printf("%p", $3); yyerror("text var"); }
                  ;

Var             : tVAR { yyerror("var"); $$ = getAddress($1); }

Int             : tNB { yyerror("int"); allocate($1); }
                  | tNBEXP { yyerror("exp"); allocate($1); }
                  | tMINUS tNB { yyerror("nb_negatif"); allocate(-$2); }
                  | tMINUS tNBEXP { yyerror("nb_negatif"); allocate(-$2); }
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
