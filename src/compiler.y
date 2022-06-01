%{
#include <stdlib.h>
#include <stdio.h>

#include "functions.h"

void yyerror(char *s);
%}
%union { int nb; Stack *addr; char *var; }
%token tPRINTF tCOMMA tMAIN tIF tAO tAF tWHILE tFOR tPO tPF tRETURN tPV tADD tMUL tELSE tEQEQ tEQ tSUP tINF tMINUS tDIV tOR tANDAND tAND tINT tCONST tVOID tFL tERROR tSUPEQ tINFEQ
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
                  | Expr tANDAND Expr { andOp(); }
                  | Expr tOR Expr { orOp(); }
                  | Int
                  | tVAR { pushCOP(getAddress($1)); }
                  | tMUL tVAR { pushLOAD(getAddress($2)); }
                  | tAND tVAR { push(getAddress($2)->value.addr); }
                  ;

While           : tWHILE tPO { whileCond(); } Expr tPF { ifCond(); } Body { whileJump(); ifJump(0); }
                  ;

AffectationOpt  : tVAR { $<nb>$ = createVar($1); } // déclaration sans affectation
                  | Affectation
                  ;

MultVar         : AffectationOpt
                  | AffectationOpt tCOMMA MultVar
                  ;

ConstOpt        : tCONST
                  |
                  ;

PtrOpt          : tMUL
                  |
                  ;

Declaration     : ConstOpt tINT PtrOpt MultVar
                  ;

Affectation     : tVAR { $<nb>$ = createVar($1); } tEQ Expr { $<nb>$ = editVar(getAddress($1)); }
                  ;

AffectationEdit : Var tEQ Expr { $<nb>$ = editVar($1); }
                  |tMUL Var tEQ Expr { $<nb>$ = editPtr($2); }
                  ;

If              : IfCond tELSE { ifJump(1); } Body { ifJump(0); }
                  | IfCond { ifJump(0); }

IfCond          : tIF tPO Expr tPF { ifCond(); } Body
                  ;

Printf          : tPRINTF tPO Var tPF { printf("%p", $3)}
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
  while_stack = malloc(sizeof(WhileStack));
  initFile();
  yyparse();
  writeInstructions();
  closeFile();

  return 0;
}
