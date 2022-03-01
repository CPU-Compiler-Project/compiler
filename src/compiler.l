%{
#include <stdlib.h>
#include <stdio.h>
#include "y.tab.h"
%}

%option noyywrap

%%

"if" { fprintf(stderr, "tIF"); }
