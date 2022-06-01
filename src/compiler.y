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
%type <addr> Var

%right tEQ
%left tADD tMINUS
%left tMUL tDIV

%start Main

%%

Body            : tAO { incr_depth(); } Instructions tAF { decr_depth(); } ;
BodyInt         : tAO { incr_depth(); } Instructions tRETURN tNB tPV tAF { decr_depth(); } ;

Instructions    : If 
                  | AffectationEdit tPV  { }
                  | While
                  | Declaration tPV
                  | Printf tPV
                  | Instructions Instructions
                  ;
                  
Main            : tINT tMAIN tPO tPF BodyInt { return 0; }
                  ;

Expr            : tPO Expr tPF
                  | Expr tADD Expr { addition(); }
                  | Expr tMINUS Expr { substraction(); }
                  | Expr tDIV Expr { divide(); }
                  | Expr tMUL Expr { multiply(); }
                  | Expr tAND Expr { andOp(); }
                  | Expr tOR Expr { orOp(); }
                  | Int
                  | tVAR { pushCOP(getAddress($1)); }
                  ;

While           : tWHILE tPO Expr tPF Body
                  ;

AffectationOpt  : tVAR { yyerror("c'est une declaration sans affectation!"); $<nb>$ = createVar($1); }
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

Affectation     : tVAR { $<nb>$ = createVar($1); } tEQ Expr { $<nb>$ = editVar(getAddress($1)); }
                  ;

AffectationEdit : Var tEQ Expr { $<nb>$ = editVar($1); }
                  ;

If              : IfCond tELSE { yyerror("c'est un IFJump!"); ifJumpElse(); } Body { yyerror("c'est un IFJumpElse!"); ifJump(); }
                  | IfCond { yyerror("c'est un IFJump!"); ifJump(); }

IfCond          : tIF tPO Expr tPF { yyerror("c'est un IF!"); yyerror("c'est un IFCond!"); ifCond(); } Body
                  ;

Printf          : tPRINTF tPO Var tPF { printf("%p", $3); yyerror("text var"); }
                  ;

Var             : tVAR { $$ = getAddress($1); }

Int             : tNB { allocate($1); }
                  | tNBEXP { allocate($1); }
                  | tMINUS tNB { allocate(-$2); }
                  | tMINUS tNBEXP { allocate(-$2); }
                  ;

%%

void yyerror(char *s) { fprintf(stderr, "%s\n", s); }

int main(void) {
  //yydebug = 1;
  printf("Compiler\n");
  sp = 0;
  stack = malloc(sizeof(Stack));
  i_addr = 0;
  istack = malloc(sizeof(InstructionStack));
  initFile();
  yyparse();
  writeInstructions();
  closeFile();

  return 0;
}
