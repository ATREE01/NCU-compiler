%{
#include "A.tab.h"
%}
%option noyywrap

number -?[0-9]+(.[0-9]+)?

%%

"f" { return FFUNC;}
"g" { return GFUNC;}
"h" { return HFUNC;}
"(" { return LPAREN;}
")" { return RPAREN;}
"," { return COMMA;}
{number} {yylval.ival = atof(yytext); return NUMBER;}
[ \t\n\r]  {}
. {}

%%