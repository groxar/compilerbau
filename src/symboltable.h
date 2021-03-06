#pragma once
//ENUMS
enum type{
    FUNC, VAR
};

enum var_type{
    V_INT, V_VOID
};

//TYPEDEFS
typedef struct func func_t;
typedef struct scope_list scope_list_t;

struct func{
    int           n_para;
    scope_list_t* scope;
    int           decl;//true if function was declarednext
};

struct scope_list{
    int   type;
    int   var_type;//return type || variable type
    char* name;
    int   address;
    int   size;

    union {
        int     value;
        func_t* func_ptr;
    } var;

    scope_list_t* next;
};

//global variables
/* extern scope_list_t* global_scope;
extern scope_list_t** crnt_scope;
extern scope_list_t** crnt_pos;
*/

//Functions
scope_list_t* getSymbol( char* _name);

int insertSymbol(int _type, int _var_type, char* _name, int _value, int _size);

int beginFunction(int _ret_val, char* _name);

int setN_Para(char* _name, int _n_para);

int endFunction(char* _name, int _decl);

scope_list_t* getSymbolTable();

scope_list_t* getSymbolInScope(scope_list_t* _scope, char* _name);
