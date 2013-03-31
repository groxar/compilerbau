#include "stdlib.h"
#include "string.h"
#include "symboltable.h"

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
//error code:
//0 success
//-1 symbol with this name exists
int insertSymbol(int _type, int _var_type, char* _name, int _value)
{
    if(getSymbol(*crnt_scope,_name) != 0)
        return -1;

    scope_list_t* new_variable = (scope_list_t*) malloc(sizeof(scope_list_t));
    int name_length = strlen(_name);

    new_variable->type     = _type;
    new_variable->var_type = _var_type;
    new_variable->name     = (char*) malloc(name_length + 1);
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
//error code
//0 success
//-1 function with this name exists
//-2 currently not in global scope
int beginFunction(int _ret_val, char*_name, int _n_para)
{
    if(*crnt_scope != global_scope)
        return -2;
    
    if(getSymbol(*crnt_scope,_name != 0))
        return -1;

    func_t* new_function = (func_t*) malloc(sizeof(func_t));
    new_function->n_para = _n_para;

    insertSymbol(FUNC, _ret_val, _name, (int) new_function);//!!long || int (depends on architecture)

    crnt_scope = &(new_function->scope);
    crnt_pos = &(new_function->scope);

    return 0;
}

void endFunction()
{
    scope_list_t** end_pos = &global_scope;

    while( *end_pos != 0)
    {
        end_pos = &((*end_pos)->next);
    }

    crnt_pos = end_pos;
    crnt_scope = &global_scope;

}

