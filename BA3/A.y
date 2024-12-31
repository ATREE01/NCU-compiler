%{
#include <stdio.h>
#include <stdlib.h>

void yyerror(const char *s);

%}

%union{
	double fval;
}

%token FFUNC GFUNC HFUNC LPAREN RPAREN COMMA INVALID
%token<fval> NUMBER
%type<fval> function expression

%%
startsymbol: function {printf("%.3f\n", $1);}

function: FFUNC LPAREN expression RPAREN { $$ = 4.0 * $3 - 1.0;}
    | GFUNC LPAREN expression COMMA expression RPAREN {$$ = 2.0 * $3 + $5 - 5.0;}
    | HFUNC LPAREN expression COMMA expression COMMA expression RPAREN {$$ = 3.0 * $3 - 5.0 * $5 + $7;}

expression: function { $$ = $1;}
    | NUMBER { $$ = $1;}

%%
void yyerror (const char *s){
    printf("Invalid\n");
	exit(0);
}


int main()
{
	/** You can write any code in main function **/
	yyparse();
	return 0;
}
