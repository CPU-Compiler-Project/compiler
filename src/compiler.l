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
"printf" { return tPRINTF; }
"("      { return tPO; }
")"      { return tPF; }
"return" { return tRETURN; }
";"      { return tPV; }
"+"      { return tADD; }
"*"      { return tMUL; }
"-"      { return tMINUS; }
"//".*$  { }
"/"      { return tDIV; }
"else"   { return tELSE; }
"=="     { return tEQEQ; }
">="     { return tSUPEQ; }
"<="     { return tINFEQ; }
"="      { return tEQ; }
">"      { return tSUP; }
"<"      { return tINF; }
"||"     { return tOR; }
"&&"     { return tAND; }
","      { return tCOMMA; }
"int"    { return tINT; }
"const"  { return tCONST; }
"void"   { return tVOID; }
[0-9]+"e"[-+]?[0-9]+ { yylval.nb = (int)atof(yytext); return tNBEXP; }
[0-9]+ { yylval.nb = atoi(yytext); return tNB; }
[a-zA-Z][a-zA-Z0-9_]*  { return tVAR; }
[\n\t ]	 { }
.	     { return tERROR; }
