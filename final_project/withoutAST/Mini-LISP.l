%{
    #include "Mini-LISP.tab.h"
%}
%option noyywrap
%%
\n {}
[ \t]+ {}
"(" {return LBRC;}
")" {return RBRC;}
"+" {return TOKEN_PLUS; }
"-" {return TOKEN_MINUS; }
"*" {return TOKEN_MUL; }
"/" {return TOKEN_DIV; }
"mod" {return TOKEN_MOD; }
">" {return TOKEN_GT;}
"<" {return TOKEN_LT;}
"=" {return TOKEN_EQ;}
"and" {return TOKEN_AND;}
"or" {return TOKEN_OR;}
"not" {return TOKEN_NOT;}

"print-num" {return PRINT_NUM;}
"print-bool" {return PRINT_BOOL;}

"define" {return DEFINE;}
"fun" {return FUN;}
"if" {return IF;}

"#t" {
    yylval.var.type = T_BOOL, yylval.var.bval = 1;
    return BOOL;
}

"#f" {
    yylval.var.type = T_BOOL, yylval.var.bval = 0;
    return BOOL;
}

(0|-?[1-9][0-9]*) {
    yylval.var.type = T_NUM, yylval.var.ival = atoi(yytext);
    return NUMBER;
}

[-|a-z|0-9]* {
    yylval.var.type = T_ID;
    strcpy(yylval.var.name, yytext);
    return ID;
}

. {}
%%