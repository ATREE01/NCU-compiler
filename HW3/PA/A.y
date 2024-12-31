%{
#include <stdio.h>
#include <stdlib.h>
int stack[1000];
int sp = -1;

void yyerror();

void load(int n){
    stack[++sp] = n;
}

void inc(){
    if(sp < 0)
        yyerror();
    stack[sp]++;
}

void dec(){
    if(sp < 0)
        yyerror();
    stack[sp]--;
}

void add(){
    if(sp < 1)
        yyerror();
    int a = stack[sp], b = stack[sp - 1];
    stack[sp - 1] = a + b;
    sp--;
}

void sub(){
    if(sp < 1)
        yyerror();
    int a = stack[sp], b = stack[sp - 1];
    stack[sp - 1] = a - b;
    sp--;
}

void mul(){
    if(sp < 1)
        yyerror();
    int a = stack[sp], b = stack[sp - 1];
    stack[sp - 1] = a * b;
    sp--;
}

void mod(){
    if(sp < 1)
        yyerror();
    int a = stack[sp], b = stack[sp - 1];
    stack[sp - 1] = a % b;
    sp--;
}

%}

%union{
	int ival;
}

%token LOAD INC DEC ADD SUB MUL MOD
%token<ival> NUMBER

%%

program	:  stmts;

stmts: stmt stmts
    | stmt
    ;

stmt: LOAD NUMBER {load($2);}
    | INC   {inc();}
    | DEC   {dec();}
    | ADD   {add();}
    | SUB   {sub();}
    | MUL   {mul();}
    | MOD   {mod();}

%%
void yyerror (){
    printf("Invalid format\n");
	exit(0);
}


int main()
{
	/** You can write any code in main function **/
	yyparse();
    if(sp > 0)
        yyerror();
    printf("%d", stack[sp]);
	return 0;
}
