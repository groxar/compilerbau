#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_

#include "stdlib.h"
#include "string.h"


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

scope_list_t*  global_scope = (scope_list_t*) 0;
scope_list_t** crnt_scope   = &global_scope;
scope_list_t** crnt_pos     = &global_scope;
int            next_address = 0;

scope_list_t* getSymbol(scope_list_t* _scope, char* _name)
{
    scope_list_t* sym = _scope;

    while(sym != 0 )//ich hätte die schleife in einer zeile machen könne *böses grinsen*
    {
        if(strcmp(sym->name,_name) == 0)
            break;
        sym = sym->next;
    }

    return sym;
}

//lookup fehlt einfügen nach implementierung von lookup
int insertSymbol(int _type, int _var_type, char* _name, int _value)
{
    scope_list_t* new_variable = malloc(sizeof(scope_list_t));
    int name_length = strlen(_name);
    
    new_variable->type     = _type;
    new_variable->var_type = _var_type;
    new_variable->name     = malloc(name_length + 1);
    new_variable->address  = next_address;
    new_variable->next     = (scope_list_t*) 0;
    if(_type == VAR)
        new_variable->var.value     = _value; 
    else
        new_variable->var.func_ptr  = (func_t*) _value;
 
    strcpy(new_variable->name, _name);

    *crnt_pos = new_variable;
    crnt_pos  = &((**crnt_pos).next);
    next_address += sizeof(int);
    return 0;
}

//lookup fehlt
int beginFunction(int _ret_val, char*_name, int _n_para)
{
    func_t* new_function = malloc(sizeof(func_t));
    new_function->n_para = _n_para;

    insertSymbol(FUNC, _ret_val, _name, (int)new_function);

    crnt_scope = &(new_function->scope);
    crnt_pos = &(new_function->scope);
}

void endFunction()
{
    scope_list_t** end_pos = &global_scope;

    while( end_pos != 0)
    {
        end_pos = &((*end_pos)->next);
    }
    
    crnt_pos = end_pos;
    crnt_scope = &global_scope;
}

#endif
