%{
    #include "Mini-LISP.tab.h"
%}

%%
[ \t\n\r]+ {}
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
    yylval.ival = 1;
    return BOOL;
}

"#f" {
    yylval.ival = 0;
    return BOOL;
}

(0|-?[1-9][0-9]*) {
    yylval.ival = atoi(yytext);
    return NUMBER;
}

[-|a-z|0-9]* {
    yylval.id = strdup(yytext);
    return ID;
}

. {}
%%