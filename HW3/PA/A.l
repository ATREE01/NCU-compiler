%{
#include "A.tab.h"
%}
%option noyywrap

number [0-9]+

%%

"load" {return LOAD;}
"inc" {return INC;}
"dec" {return DEC;}
"add" {return ADD;}
"sub" {return SUB;}
"mul" {return MUL;}
"mod" {return MOD;}
{number} {yylval.ival = atoi(yytext); return NUMBER;}
[ \n] {}

%%