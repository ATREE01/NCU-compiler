#ifndef _AST_H
#define _AST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void yyerror(const char *);
int yylex(void);

// ast tree node
struct Node {
    char type;
    char* id;
    int value;
    struct Node* left;
    struct Node* right;
};

struct symbol_table_node {
    char type; // 'F', 'I'
    char* id;
    int value; 
    int scope;
    struct Node* fun;
};

struct Result {
    char type; // 'B', 'I'
    long long value; 
    int tmp; // this is used to store the value when doing the multiple compare
};

extern struct Node* root;

extern int scope_count, symbol_table_count, param_stack_count;
// for debug print tree
extern int depth;

// declare function here 
struct Node* create_node(struct Node* left, struct Node* right, char type);
// put variable to symbol table
void insert_symbol_table(char type, char* id, int value, int scope, struct Node* fun);
// get the index of the variable you want
int get_symbol_table_index(char* id);
// travese AST tree
struct Result traverseAST(struct Node* root); 
void remove_symbol_table_entry(int scope);

#endif