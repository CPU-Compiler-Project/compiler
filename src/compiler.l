%{
#include <stdlib.h>
#include <stdio.h>
#include "y.tab.h"
%}

%option noyywrap

%%

"if"	 { return tIF; }
"{"      { return tAO; }
"}"      { return tAF; }
"while"  { return tWHILE; }
"main"   { return tMAIN; }
"for"    { return tFOR; }
"("      { return tPO; }
")"      { return tPF; }
"return" { return tRETURN; }
";"      { return tPV; }
"+"      { return tADD; }
"*"      { return tMUL; }
"//".*$  {}
"/"      { return tDIV; }
"else"   { return tELSE; }
"=="     { return tEQEQ; }
">="     { return tSUPEQ; }
"<="     { return tINFEQ; }
"="      { return tEQ; }
">"      { return tSUP; }
"<"      { return tINF; }
"-"      { return tMINUS; }
"||"     { return tOR; }
"&&"     { return tAND; }
"int"    { return tINT; }
"const"  { return tCONST; }
"void"   { return tVOID; }
-?[0-9]+ { return tNB; }
[a-z]+   { return tVAR; }
[\n\t ]	 { }
.	     { return tERROR; }
