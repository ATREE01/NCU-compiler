%{
#include <stdio.h>
#include <stdlib.h>
void yyerror (const char *message);
void error(int cntChar);

%}
%code requires
{
    struct Matrix{
        int r, c;
    };
}

%union{
	int ival;
    int oper;
    struct Matrix mat;
}

%token <oper> ADDSUB MUL
%token COMMA LSBC RSBC LBC RBC TP
%token<ival> NUM

%type <mat> matrix

%left ADDSUB
%left MUL
%left TP

%%

program	:  matrix { 
    printf("Accepted\n");
    return 0;    
};

matrix: LSBC NUM COMMA NUM RSBC { $$.r = $2, $$.c = $4; };
    | LBC matrix RBC { $$ = $2; } 
    | matrix TP { $$.r = $1.c, $$.c = $1.r; }
    | matrix MUL matrix { 
        if($1.c != $3.r){
            error($2);    
        }
        $$.r = $1.r, $$.c = $3.c; 
    }
    | matrix ADDSUB matrix { 
        if($1.r != $3.r || $1.c != $3.c){
            error($2);    
        }
        $$.r = $1.r, $$.c = $1.c; 
    };
    
%%

void error(int cntChar){
    printf("Semantic error on col %d", cntChar);
    exit(0);
}

void yyerror (const char *message){
    printf("%s\n", message);
}


int main()
{
	/** You can write any code in main function **/
	yyparse();
	return 0;
}
