%{
    #include "ast.h"
    extern struct Node* root;
%}

%union {
    int ival;
    char* id;
    struct Node* node;
}

%token LBRC RBRC TOKEN_PLUS TOKEN_MINUS TOKEN_MUL TOKEN_DIV TOKEN_MOD 
%token TOKEN_GT TOKEN_LT TOKEN_EQ TOKEN_AND TOKEN_OR TOKEN_NOT
%token PRINT_NUM PRINT_BOOL DEFINE IF FUN

%token<ival> NUMBER BOOL
%token<id> ID

%type<node> exp exps stmt stmts  def_stmt print_stmt
%type<node> num_op logical_op if_exp fun_exp fun_ids fun_body fun_call params param fun_name
%type<node> variable

%%

program : stmts { root = $1;}
    ;

stmts: stmt stmts { $$ = create_node($1, $2, 'T');}
    | stmt {$$ = create_node($1, NULL, 'T');};
    ;

stmt : exp { $$ = $1;}
    | def_stmt { $$ = $1;}
    | print_stmt { $$ = $1;}
    ;

exp: NUMBER {$$ = create_node(NULL, NULL, 'I'); $$->value = $1; }
    | BOOL {$$ = create_node(NULL, NULL, 'B'); $$->value = $1;}
    | num_op {$$ = $1;}
    | logical_op {$$ = $1;}
    | fun_exp {$$ = $1;}
    | fun_call {$$ = $1;}
    | if_exp {$$ = $1;}
    | variable {$$ = $1;}
    ;

exps: exp exps                      { $$ = create_node($1, $2, 'E'); }
    | exp                           { $$ = create_node($1, NULL, 'E'); }
    ;

/* def_stmts: def_stmt def_stmts {$$ = create_node($1, $2, 'D');}
    | {$$ = NULL;}
    ; */

def_stmt : LBRC DEFINE variable exp RBRC {  $$ = create_node($3, $4, 'd'); }
    ;

print_stmt: LBRC PRINT_NUM exp RBRC { $$ = create_node($3, NULL, 'P');} // Capital P for print-num
    | LBRC PRINT_BOOL exp RBRC { $$ = create_node($3, NULL, 'p');};

    ;

variable: ID  { $$ = create_node(NULL, NULL, 'V'); $$->id = $1; }
    ;

num_op: LBRC TOKEN_PLUS exp exps RBRC { $$ = create_node($3, $4, '+');}
    | LBRC TOKEN_MINUS exp exp RBRC { $$ = create_node($3, $4, '-');}
    | LBRC TOKEN_MUL exp exps RBRC { $$ = create_node($3, $4, '*');}
    | LBRC TOKEN_DIV exp exp RBRC { $$ = create_node($3, $4, '/');}
    | LBRC TOKEN_MOD exp exp RBRC { $$ = create_node($3, $4, '%');}
    | LBRC TOKEN_GT exp exp RBRC { $$ = create_node($3, $4, '>');}
    | LBRC TOKEN_LT exp exp RBRC { $$ = create_node($3, $4, '<');}
    | LBRC TOKEN_EQ exp exps RBRC { $$ = create_node($3, $4, '=');}


logical_op: LBRC TOKEN_AND exp exps RBRC { $$ = create_node($3, $4, '&');}
    | LBRC TOKEN_OR exp exps RBRC { $$ = create_node($3, $4, '|');}
    | LBRC TOKEN_NOT exp RBRC { $$ = create_node($3, NULL, '!');}
    ;
if_exp : LBRC IF exp exp exp RBRC { struct Node* tmp = create_node($4, $5, 'X'); $$ = create_node($3, tmp, 'G');}
    ;

fun_call : LBRC fun_exp params RBRC { $$ = create_node($3, $2, 'C');} // anamouns function call
    | LBRC fun_name params RBRC { $$ = create_node($3, $2, 'C');} // named function call
    ;

fun_name: ID { $$ = create_node(NULL, NULL, 'V'); $$->id = $1;}
    ;

fun_exp: LBRC FUN LBRC fun_ids RBRC fun_body RBRC { $$ = create_node($4, $6, 'F');}
    ;

fun_ids: ID fun_ids { struct Node* tmp = create_node(NULL, NULL, 'V'); tmp->id = $1; $$ = create_node(tmp, $2, 'S');}
    | {$$ = NULL;}
    ;

fun_body: exp { $$ = $1;}
    ;

/* fun_body: def_stmts exp { $$ = create_node($1, $2, 'B');}
    ; */

params : param params { $$ = create_node($1, $2, 'A');}
    | {$$ = NULL;}
    ;

param : exp { $$ = $1;}
    ;



%%

int main(void) {
    yyparse();
    traverseAST(root);
    return 0;
}
