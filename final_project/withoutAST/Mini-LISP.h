#ifndef MINI_LISP_H
#define MINI_LISP_H

#include <stdbool.h>

// Enum declaration
typedef enum {
    T_NUM, T_BOOL, T_ID
} Type;

// Struct declaration for variables
struct Variable {
    Type type;  // type of variable (T_NUM, T_BOOL, T_ID)
    char name[256];
    int ival;   // integer value
    bool bval;  // boolean value
    int result_plus; 
    int result_multi;
    bool result_equal;
    bool result_and;  
    bool result_or;
};

// Function declarations
bool findVar(char *name, int type, struct Variable *var);

static inline const char* getTypeName(Type t) {
    switch (t) {
        case T_NUM: return "int";
        case T_BOOL: return "bool";
        case T_ID: return "identifier";
        default: return "Unknown";
    }
}

#endif