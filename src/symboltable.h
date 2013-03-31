#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_

//ENUMS
enum var_type {
    INTEGER, VOID
};
enum type{
    FUNC, VAR
};

//TYPEDEFS
typedef struct func func_t;
typedef struct scope_list scope_list_t;

typedef struct func{
    int           n_para;
    scope_list_t* scope;
}func_t;

typedef struct scope_list{
    int   type;
    int   var_type;//return type || variable type
    char* name;
    int   address;

    union {
        int     value;
        func_t* func_ptr;
    } var;

    scope_list_t* next;
}scope_list_t;

//global variables
extern scope_list_t* global_scope;
extern scope_list_t** crnt_scope;
extern scope_list_t** crnt_pos;

//Functions
scope_list_t* getSymbol(scope_list_t* _scope, char* _name);

int insertSymbol(int _type, int _var_type, char* _name, int _value);

int beginFunction(int _ret_val, char* _name, int _n_para);

void endFunction();
#endif
