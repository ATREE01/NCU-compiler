%{
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define STACK_SIZE 20
int yylex();
void yyerror(const char* message);
struct stack {
    int data[STACK_SIZE];
    int top;
};
typedef struct stack stack_t;
stack_t stack;
int train;
bool isEmpty(){
    return stack.top == -1;
}; // to check if the stack is empty

void dump(){
    printf("Current holding track: ");
    for(int i = 0; i <= stack.top; i++){
        printf("%d ", stack.data[i]);
    }
    printf("\n");
}; // to dump (or print) all the data in stack

void push(int i){
    printf("Push train %d to holding track\n", i);
    stack.data[++stack.top] = i;
    dump();
};
int top(){
    return stack.data[stack.top];
};
void pop(){
    printf("Moving train %d from holding track\n", stack.data[stack.top--]);
};

%}

%union {
    int ival;
}

%token END
%token<ival> NUMBER
%type<ival> number numbers

%%
startsymbol: numbers END {
        if(!isEmpty())
            printf("Error: There is still existing trains in the holding track\n");
        else 
            printf("Success\n");
    }

numbers: numbers number  {
        while(true){
            // printf("Number: %d, top(): %d\n", $2, top());
            if($2 == train){
                printf("Train %d passing through\n", train++);
                break;
            }
            else if(!isEmpty() && (top() == $2)){
                pop();
                break;
            }

            else if(isEmpty() && $2 < train){
                printf("Error: Impossible to rearrange\n");
                printf("There is no any train in the holding track\n");
                yyerror("");
            }
            else {
                if(top() > $2){
                    printf("Error: Impossible to rearrange\n");
                    printf("The first train in the holding track is train %d instead of train %d\n", top(), $2);
                    yyerror("");
                }
                push(train++);
            }
        }
    }   
    | number { 
        while(true){
            if($1 == train){
                printf("Train %d passing through\n", train++);
                break;
            }
            else {
                if(top() > $1){
                    printf("Error: Impossible to rearrange\n");
                    printf("The first train in the holding track is train %d instead of train %d", top(), $1);
                    yyerror("");
                }
                push(train++);
            }
        }
     }


number: NUMBER { $$ = $1; }

%%
void yyerror (const char *s){
	exit(0);
}

int main(){
    train = 1;
    stack.top = -1;
    yyparse();
    return 0;
}