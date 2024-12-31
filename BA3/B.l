%{
#include "B.tab.h"
%}
%option noyywrap

number [1-9][0-9]*

%%

{number} { yylval.ival = atoi(yytext); return NUMBER; }
"0" { return END;}
[ \t\n\r]        { /* ignore whitespace */ }
.              { /* ignore everything else */ }

%%