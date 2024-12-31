%{
    #include <stdio.h>
    #include <string.h>
    #include "Mini-LISP.h"

    int varCnt = 0;

    struct Variable variables[1024];

    void typeError();
    void yyerror (const char *message);
    int yylex();
%}
%code requires
{
    #include "Mini-LISP.h"
}

%union{
    struct Variable var;
}

%token LBRC RBRC TOKEN_PLUS TOKEN_MINUS TOKEN_MUL TOKEN_DIV TOKEN_MOD 
%token TOKEN_GT TOKEN_LT TOKEN_EQ TOKEN_AND TOKEN_OR TOKEN_NOT
%token PRINT_NUM PRINT_BOOL DEFINE FUN IF 
%token<var> BOOL NUMBER ID

%type<var> EXP EXPS VARIABLE
%type<var> PLUS MINUS MUL DIV MOD GT LT EQ AND OR NOT
%type<var> NUM_OP LOGICAL-OP FUN-EXP FUN-CALL IF-EXP


%%

program: stmts;
stmts: stmt stmts | stmt ;

/* STMT ::= EXP | DEF-STMT | PRINT-STMT */
stmt : EXP | DEF-STMT | PRINT_STMT;

/* DEF-STMT ::= (define VARIABLE EXP) */
DEF-STMT: LBRC DEFINE VARIABLE EXP RBRC {
    strcpy(variables[varCnt].name, $3.name);
    if($4.type == T_NUM){
        variables[varCnt].ival = $4.ival;
        variables[varCnt].type = T_NUM;
    }
    else {
        variables[varCnt].bval = $4.bval;
        variables[varCnt].type = T_BOOL;
    }
    varCnt++;
    // printf("Variable %s defined. Value is %d\n", $3.cval, $4.ival);
};

/* PRINT-STMT ::= (print-num EXP) | (print-bool EXP) */
PRINT_STMT: LBRC PRINT_NUM EXP RBRC {
        if($3.type == T_ID) {// ID
            struct Variable var;
            if(findVar($3.name, 0, &var))
                printf("%d\n", var.ival);
            else {
                typeError("int", getTypeName(var.type));
                return 0;
            }
        }
        else if($3.type == T_NUM) {
            printf("%d\n", $3.ival);
        }
        else {
            typeError();
            return 0;
        }
            
    }
    | LBRC PRINT_BOOL EXP RBRC {
        if($3.type == T_ID) {// ID
            struct Variable var;
            if(findVar($3.name, 1, &var))
                printf("%s\n", var.bval ? "#t" : "#f");
            else {
                typeError("bool", getTypeName(var.type));
                return 0;
            }
        }
        else if($3.type == T_BOOL) {
            printf("%s\n", $3.bval ? "#t" : "#f");
        }
        else {
            typeError("bool", getTypeName($3.type));
            return 0;
        }
    };
/*
EXP ::= bool-val | number | VARIABLE | NUM-OP | LOGICAL-OP 
    | FUN-EXP | FUN-CALL | IF-EXP 
*/

EXPS: EXP EXPS {
        $$ = $1;
        if($1.type == T_NUM){
            if($2.type == T_NUM){
                $$.type = T_NUM;
                $$.result_plus = $1.ival + $2.result_plus;
                $$.result_multi = $1.ival * $2.result_multi;
                $$.result_equal = ($1.ival == $2.ival && $2.result_equal);
            } else {
                typeError("int", getTypeName($2.type));
                return 0;
            }
        }
        else if($1.type == T_BOOL){
            if($2.type == T_BOOL){
                $$.result_equal = false;
                if($1.ival == $2.ival)
                    $$.result_equal = true;
                $$.result_equal = $$.result_equal && $2.result_equal;
                $$.result_and = $1.bval && $2.result_and;
                $$.result_or = $1.bval || $2.result_or;
            } else {
                typeError("bool", getTypeName($2.type));
                return 0;
            }
        }
    }
    | EXP {
        $$ = $1;
        if($1.type == T_NUM){
            // printf("NUMBER %d\n", $1.ival);
            $$.type = T_NUM, $$.ival = $1.ival;
            $$.result_plus = $1.ival;
            $$.result_multi = $1.ival;
            $$.result_equal = true;
        }
        else if($1.type == T_BOOL){
            // printf("BOOL %d\n", $1.bval);
            $$.type = T_BOOL, $$.bval = $1.bval;
            $$.result_equal = true;
            $$.result_and = $1.bval;
            $$.result_or = $1.bval;
        }
    };


EXP: NUM_OP {$$ = $1;}
    | LOGICAL-OP {$$ = $1;}
    /* | FUN-EXP 
    | FUN-CALL  */
    | IF-EXP {$$ = $1;}   
    | VARIABLE {
        struct Variable var;
        if(findVar($1.name, 0, &var)){
            $$ = var;
        } else {
            printf("Variable %s not declared\n", $1.name);
            return 0;
        }
    }
    | BOOL {$$.type = $1.type, $$.bval = $1.bval;}
    | NUMBER {$$.type = $1.type, $$.ival = $1.ival;}
    ;

/* 
NUM-OP ::= PLUS | MINUS | MULTIPLY | DIVIDE | MODULUS | GREATER
    | SMALLER | EQUAL
*/
NUM_OP: PLUS { $$.type = T_NUM, $$.ival = $1.result_plus;}
    | MINUS { $$.type = T_NUM, $$.ival = $1.ival;}
    | MUL { $$.type = T_NUM, $$.ival = $1.result_multi;}
    | DIV { $$.type = T_NUM, $$.ival = $1.ival;}
    | MOD { $$.type = T_NUM, $$.ival = $1.ival;}
    | GT 
    | LT 
    | EQ;

LOGICAL-OP: AND { $$.type = T_BOOL, $$.bval = $1.result_and;}
    | OR { $$.type = T_BOOL, $$.bval = $1.result_or;}
    | NOT { $$.type = T_BOOL, $$.bval = $1.bval;};

PLUS: LBRC TOKEN_PLUS EXP EXPS RBRC {
    if($3.type != T_NUM || $3.type != $4.type){
        typeError("int", getTypeName($4.type));
        return 0;
    } else {
        $$.type = T_NUM;
        $$.result_plus = $3.ival + $4.result_plus;
    }
}

MINUS: LBRC TOKEN_MINUS EXP EXP RBRC {
    if($3.type != T_NUM || $3.type != $4.type){
        typeError("int", getTypeName($4.type));
        return 0;
    } else {
        $$.type = T_NUM;
        $$.ival = $3.ival - $4.ival;
    }
};

MUL:  LBRC TOKEN_MUL EXP EXPS RBRC {
    if($3.type != T_NUM || $3.type != $4.type){
        typeError("int", getTypeName($4.type));
        return 0;
    } else {
        $$.type = T_NUM;
        $$.result_multi = $3.ival * $4.result_multi;
    }
};

DIV: LBRC TOKEN_DIV EXP EXP RBRC{
    if($3.type != T_NUM || $3.type != $4.type){
        typeError("int", getTypeName($4.type));
        return 0;
    } else {
        $$.type = T_NUM;
        $$.ival = $3.ival / $4.ival;
    }
};

MOD: LBRC TOKEN_MOD EXP EXP RBRC{
    if($3.type != T_NUM || $3.type != $4.type){
        typeError("int", getTypeName($4.type));
        return 0;
    } else {
        $$.type = T_NUM;
        $$.ival = $3.ival % $4.ival;
        // printf("%d mod %d = %d\n", $3.ival, $4.ival, $$.ival);
    }
};

GT: LBRC TOKEN_GT EXP EXP RBRC {
    if($3.type != T_NUM || $3.type != $4.type){
        typeError("int", getTypeName($4.type));
        return 0;
    } else {
        $$.type = T_BOOL;
        $$.bval = $3.ival > $4.ival;
    }
};
LT: LBRC TOKEN_LT EXP EXP RBRC {
    if($3.type != T_NUM || $3.type != $4.type){
        typeError("int", getTypeName($4.type));
        return 0;
    } else {
        $$.type = T_BOOL;
        $$.bval = $3.ival < $4.ival;
    }
};
EQ: LBRC TOKEN_EQ EXP EXPS RBRC {
    if($3.type != T_NUM || $3.type != $4.type){
        typeError("int", getTypeName($4.type));
        return 0;
    } else {
        $$.type = T_BOOL;
        $$.bval = (($3.ival == $4.ival) && $4.result_equal);
    }
};

AND: LBRC TOKEN_AND EXP EXPS RBRC {
    if($3.type != T_BOOL || $3.type != $4.type){
        typeError("bool", getTypeName($4.type));
        return 0;
    } else {
        $$.type = T_BOOL;
        $$.result_and = $3.bval && $4.result_and;
    }
}
OR: LBRC TOKEN_OR EXP EXPS RBRC {
    if($3.type != T_BOOL || $3.type != $4.type){
        typeError("bool", getTypeName($4.type));
        return 0;
    } else {
        $$.type = T_BOOL;
        $$.result_or = $3.bval || $4.result_or;
    }
}
NOT: LBRC TOKEN_NOT EXP RBRC {
    if($3.type != T_BOOL){
        typeError("bool", getTypeName($3.type));
        return 0;
    } else {
        $$.type = T_BOOL;
        $$.bval = !$3.bval;
    }
}

/* IF-EXP ::= (if TEST-EXP THAN-EXP ELSE-EXP) */
IF-EXP: LBRC IF EXP EXP EXP RBRC {
    // printf("IF-EXP\n");
    if($3.type != T_BOOL){
        typeError("bool", getTypeName($3.type));
        return 0;
    } else {
        if($3.bval)
            $$ = $4;
        else
            $$ = $5;
    }
};

FUN-EXP: {};
FUN-CALL: {};
VARIABLE: ID { $$ = $1; };

%%

void typeError(char* exp, char* get){
    printf("Type Error: %s expected but got %s\n", exp, get);
}   

void yyerror (const char *message){
    printf("%s\n", message);
}

bool findVar(char *name, int type, struct Variable *var){
    for(int i = 0; i < varCnt; i++){
        if(strcmp(variables[i].name, name) == 0){
            if(variables[i].type == type){
                *var = variables[i];
                return true;
            }
        }
    }
    return false;
}

int main(){
	yyparse();
	return 0;
}
