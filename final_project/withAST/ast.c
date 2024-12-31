#include "ast.h"
typedef struct Node* NodePtr;
typedef struct Result Result;

NodePtr root = NULL;

struct symbol_table_node symbol_table[1024];
int param_stack[1024];

int scope_count = 0, symbol_table_count = 0, param_stack_count = 0;
int depth = 0;

void yyerror(const char *s) {
  fprintf(stderr, "%s\n", s);
  exit(1);
}

NodePtr create_node(NodePtr left, NodePtr right, char type) {
    NodePtr tmp = (NodePtr) malloc(sizeof(struct Node));
    tmp->type = type;
    tmp->id = "";
    tmp->value = 0;
    tmp->left = left;
    tmp->right = right;
    return tmp;
}

// 搜尋symbol table，找對應的下標
//找的時候scope不應該只是考慮同一層的情況
int get_symbol_table_index(char* tmp_id) {    
    int idx = -1;
    for (int i = 0; i < symbol_table_count; i++) {
        // printf("symbol_table[%d].id: %s\n", i, symbol_table[i].id);
        // printf("symbol_table[%d].type: %c\n", i, symbol_table[i].type);
        // printf("symbol_table[%d].scope: %d\n", i, symbol_table[i].scope);

        if ((strcmp(symbol_table[i].id, tmp_id) == 0) && (symbol_table[i].scope <= scope_count)) {
            idx = i; // find the last declared symbol and meet the scope requirement
        }
    }
    return idx; //DNE
}

void remove_symbol_table_entry(int scope) { //移除所以特定function (scope)的所有變數
    for (int i = 0; i < symbol_table_count; i++) 
        if (symbol_table[i].scope == scope){
            symbol_table[i].id = "";
            symbol_table_count--;
        }
}

void insert_symbol_table(char type, char* id, int value, int scope, NodePtr fun) {

    symbol_table[symbol_table_count].type = type;
    symbol_table[symbol_table_count].id = id;
    symbol_table[symbol_table_count].value = value;
    symbol_table[symbol_table_count].scope = scope;
    symbol_table[symbol_table_count++].fun = fun;

    // printf("insert: symbol_table[%d].id: %s\n", symbol_table_count - 1, symbol_table[symbol_table_count - 1].id);

}

Result traverseAST(NodePtr root){
    // if(root)
    //     printf("root->type: %c\n", root->type);
    if(root == NULL) return (Result){.value = -1, .type = 'X'};;
    if(root->type == 'T'){
        traverseAST(root->left);
        traverseAST(root->right);
        return (Result){.value = -1, .type = 'X'};;
    }
    else if(root->type == 'I' ) {// integer and bool
        return (Result){.value = root->value, .type = 'I'};
    }
    else if(root->type == 'B'){
        return (Result){.value = root->value, .type = 'B'};
    }
    else if (root -> type == 'V') { //variable
        int index = get_symbol_table_index(root -> id);
        if (index != -1) { //if node exist
            if (symbol_table[index].type != 'F') { // 如果不是function，即是bool或integer，就直接回傳值
                if(symbol_table[index].type == 'I')
                    return (Result){.value = symbol_table[index].value, .type = 'I'};
                else if(symbol_table[index].type == 'B')
                    return (Result){.value = symbol_table[index].value, .type = 'B'};
            }
        }
        else {
            yyerror("variable DNE");
            return (Result){.value = -1, .type = 'X'};
        }
    } 
    else if(root->type == 'P'){ // print_num
        // I didn't do the type check here cause the requirement didn't mention it.
        Result tmp = traverseAST(root->left);
        printf("%lld\n", tmp.value);
        return (Result){.value = -1, .type = 'X'};
    }
    else if(root->type == 'p'){
        Result tmp = traverseAST(root->left);
        printf(tmp.value == 1 ? "#t\n" : "#f\n");
        return (Result){.value = -1, .type = 'X'};
    }
    else if(root->type == 'd'){ // def_stmt
        // printf("symbol_table[%d].id: %s, scope: %d\n", symbol_table_count, symbol_table[symbol_table_count].id, symbol_table[symbol_table_count].scope);
        if (root -> right -> type == 'F') {    
            symbol_table[symbol_table_count].type = 'F';
            symbol_table[symbol_table_count].fun = root -> right;
            symbol_table[symbol_table_count].value = 0;
        }
        else { //如果右邊是 數字 or boolean
            Result tmp = traverseAST(root -> right);
            symbol_table[symbol_table_count].value = tmp.value;
            if(tmp.type == 'I')
                symbol_table[symbol_table_count].type = 'I';
            else if(tmp.type == 'B')
                symbol_table[symbol_table_count].type = 'B';
            symbol_table[symbol_table_count].fun = NULL;
        }
        symbol_table[symbol_table_count].id = root -> left -> id;
        symbol_table[symbol_table_count].scope = scope_count;
        symbol_table_count++;
    }
    else if(root->type == '+'){
        Result tmp_1 = traverseAST(root -> left);
        Result tmp_2 = (Result){.value = 0, .type = 'I'};
        if (root -> right == NULL) { // end of expression
            return tmp_1;
        }
        else if (root -> right -> type == 'E' || root -> right -> type == '+') {
            root -> right -> type = '+'; // rewrite the type to '+
            tmp_2 = traverseAST(root -> right); 
        } 
        if(!(tmp_1.type == 'I' && tmp_2.type == 'I'))
            yyerror("type error");

        // printf("tmp_1.value: %lld tmp_2.value: %lld\n", tmp_1.value, tmp_2.value);
        return (Result){.value = tmp_1.value + tmp_2.value, .type = 'I'};
    }
    else if (root -> type == '-') {
        Result tmp_1 = traverseAST(root -> left);
        Result tmp_2 = traverseAST(root -> right);
        if(!(tmp_1.type == 'I' && tmp_2.type == 'I'))
            yyerror("type error");
        return (Result){.value = tmp_1.value - tmp_2.value, .type = 'I'};
    } 
    else if (root -> type == '*') {
        Result tmp_1 = traverseAST(root -> left);
        Result tmp_2 = (Result){.value = 1, .type = 'I'};
        if (root->right == NULL) { // end of expression
            return tmp_1;
        }
        else if (root -> right -> type == 'E' || root -> right -> type == '*') {
            root -> right -> type = '*'; // rewrite the type to '*'
            tmp_2 = traverseAST(root -> right); 
        } 
        else {
            printf("unexpected mul\n");
        }

        if(!(tmp_1.type == 'I' && tmp_2.type == 'I'))
            yyerror("type error");
        return (Result){.value = tmp_1.value * tmp_2.value, .type = 'I'};
    } 
    else if (root -> type == '/') {
        Result tmp_1 = traverseAST(root -> left);
        Result tmp_2 = traverseAST(root -> right);
        if(!(tmp_1.type == 'I' && tmp_2.type == 'I'))
            yyerror("type error");
        return (Result){.value = tmp_1.value / tmp_2.value, .type = 'I'};
    }
    else if(root->type == '%'){
        Result tmp_1 = traverseAST(root->left);
        Result tmp_2 = traverseAST(root->right);
        if(!(tmp_1.type == 'I' && tmp_2.type == 'I'))
            yyerror("type error");
        return (Result){.value = tmp_1.value % tmp_2.value, .type = 'I'};
    }
    else if (root -> type == '>') {
        Result tmp_1 = traverseAST(root -> left);
        Result tmp_2 = traverseAST(root -> right);
        if(!(tmp_1.type == 'I' && tmp_2.type == 'I'))
            yyerror("type error");
        return (Result){.value = tmp_1.value > tmp_2.value, .type = 'B'};
    } 
    else if (root -> type == '<') {
        Result tmp_1 = traverseAST(root -> left);
        Result tmp_2 = traverseAST(root -> right);
        if(!(tmp_1.type == 'I' && tmp_2.type == 'I'))
            yyerror("type error");
        return (Result){.value = tmp_1.value < tmp_2.value, .type = 'B'};
    }
    else if (root -> type == '=') { // big bug here
        Result tmp_1 = traverseAST(root -> left);
        Result tmp_2 = {.value = 0, .type = 'I'};
        if (root -> right == NULL) {
            return (Result){.value = 1, .tmp = tmp_1.value, .type = 'I'};
        }
        else if (root -> right -> type == 'E' || root -> right -> type == '=') {
            // this temp value is get from parsing the symbol table so the value is value not the result of and need to exchange it.
            tmp_1.tmp = tmp_1.value, tmp_1.value = 1; 

            root -> right -> type = '=';
            tmp_2 = traverseAST(root -> right); 

            // when its not the root of compare expression the type need to be I
            tmp_2.type = 'I';
        }
        if(!(tmp_1.type == 'I' && tmp_2.type == 'I'))
            yyerror("type error");


        return (Result){.value = (tmp_1.tmp == tmp_2.tmp && tmp_2.value) , .tmp = tmp_1.tmp, .type = 'B'};
    }
    else if (root -> type == '&') {
        Result tmp_1 = traverseAST(root -> left);
        Result tmp_2 = {.value = 0, .type = 'B'};
        if (root -> right == NULL) {
            return tmp_1;
        }
        else if (root -> right -> type == 'E' || root -> right -> type == '&') {
            root -> right -> type = '&';
            tmp_2 = traverseAST(root -> right); 
        }

        if(!(tmp_1.type == 'B' && tmp_2.type == 'B'))
            yyerror("type error");

        return (Result){.value = tmp_1.value && tmp_2.value, .type = 'B'};
    } 
    else if (root -> type == '|') {
        Result tmp_1 = traverseAST(root -> left);
        Result tmp_2 = {.value = 0, .type = 'B'};
        if (root -> right == NULL) {
            return tmp_1;
        }
        else if (root -> right -> type == 'E' || root -> right -> type == '|') {
            root -> right -> type = '|';
            tmp_2 = traverseAST(root -> right); 
        } 
        if(!(tmp_1.type == 'B' && tmp_2.type == 'B'))
            yyerror("type error");
        return (Result){.value = tmp_1.value || tmp_2.value, .type = 'B'};
    } 
    else if (root -> type == '!') { //not
        Result value = traverseAST(root -> left);
        if(value.type != 'B')
            yyerror("type error");
        return (Result){.value = !value.value, .type = 'B'};
    }
    else if(root->type == 'G'){
        Result tmp1 = traverseAST(root->left);
        root->right->value = tmp1.value;
        Result tmp2 = traverseAST(root->right);
        return tmp2;
    }
    else if(root->type == 'X'){
        Result tmp = root->value == 1 ? traverseAST(root->left) : traverseAST(root->right);
        return tmp;
    }
    else if(root->type == 'F'){// fun_exp
        // printf("F root->left->type: %c root->right->type: %c\n", root->left->type, root->right->type);
        traverseAST(root->left);
        return traverseAST(root->right);
    }
    else if(root->type == 'S'){ // fun_ids step2: insert all the param into symbol
        // give the value from params to the id

        insert_symbol_table('I', root->left->id, param_stack[--param_stack_count], scope_count, NULL);
        traverseAST(root->right);
        return (Result){.value = -1, .type = 'X'};
    }
    else if(root->type == 'B'){
        traverseAST(root->left);
        Result tmp = traverseAST(root->right);
        return  tmp;
    }
    else if(root->type == 'C'){ // fun call
        // left is param right is fun_name('V') or fun_exp
        traverseAST(root->left);  // left->type == 'A'
        if(root->right->type == 'V'){
            int index = get_symbol_table_index(root->right->id);
            // printf("%s\n", root->right->id);
            // printf("index: %d\n", index);
            root->right = symbol_table[index].fun;
        }
        scope_count++;
        Result tmp = traverseAST(root->right); //right->type == exp
        remove_symbol_table_entry(scope_count--); // remove the var in function
        return tmp;
    }
    else if(root->type == 'A'){ // step1: get all the param and push into stack
        traverseAST(root->right);
        Result tmp = traverseAST(root->left);
        param_stack[param_stack_count++] = tmp.value;
    }
}