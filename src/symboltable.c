#include "stdlib.h"
#include "string.h"
#include "symboltable.h"

static scope_list_t*  global_scope = (scope_list_t*) 0;
static scope_list_t** crnt_scope   = &global_scope; //crnt_scope zeigt auf den aktuellen scope
static scope_list_t** crnt_pos     = &global_scope; //crnt_pos zeigt auf nächste freie Stelle
static int            next_address = 0;

scope_list_t* getSymbolInScope(scope_list_t* _scope, char* _name)
{
    //liefert ein Symbol aus einem spezifischen Scope, wenn nicht vorhanden return 0

    scope_list_t* sym = _scope;

    while(sym != 0 )//ich hätte die schleife in einer zeile machen könne *böses grinsen*
    {
        if(strcmp(sym->name,_name) == 0)
            break;
        sym = sym->next;
    }

    return sym;
}

void freeScope(scope_list_t* _scope)
{
    //gibt Speicherplätze der Symbole frei
    //angefangen von dem Symbol auf dem der Parameter zeigt

    if(_scope->next != 0)
        freeScope(_scope->next);
    free(_scope->name);
    free(_scope);
}

scope_list_t* getSymbol(char* _name)
{
    //liefert ein Symbol aus dem Funktionsscope bzw Globalenscope

    scope_list_t* result = (scope_list_t*) 0;

    result = getSymbolInScope(*crnt_scope,_name);

    if(result == 0 && *crnt_scope != global_scope)
        result = getSymbolInScope(global_scope,_name);

    return result;
}

//error code:
//0 success
//-1 symbol with this name exists
int insertSymbol(int _type, int _var_type, char* _name, int _value, int _size)
{
    //fügt ein beliebiges Symbol ein

    if(getSymbolInScope(*crnt_scope,_name) != 0)
        return -1;

    scope_list_t* new_variable = (scope_list_t*) malloc(sizeof(scope_list_t));
    int name_length = strlen(_name);

    new_variable->type     = _type;
    new_variable->var_type = _var_type;
    new_variable->name     = (char*) malloc(name_length + 1);
    new_variable->address  = next_address;
    new_variable->next     = (scope_list_t*) 0;

    // Da Pointer und int gleiche Byte-Zahl, benutze int im 32-Bit-System
    if(_type == VAR)
        new_variable->var.value     = _value;
    else
        new_variable->var.func_ptr  = (func_t*) _value;

    strcpy(new_variable->name, _name);
    	//(*new_variable).name

    //setzt den aktuellen Scope eins weiter
    *crnt_pos = new_variable;
    crnt_pos  = &((**crnt_pos).next);

    next_address += _size;
    return 0;
}

//0 success
//-1 symbole with this name exists
//-2 currently not in global scope
int beginFunction(int _ret_val, char*_name)
{
    //fügt eine Function in die Symboltabelle ein und ändert den zuzeitgen Scope

    if(*crnt_scope != global_scope)
        return -2;
    
    scope_list_t* sym = getSymbolInScope(global_scope,_name);
    func_t* function;

    if(sym == 0)
    {
        //fügt die Function in die Symboltabelle ein wenn die Funktion noch nicht definiert wurde

        function = (func_t*) malloc(sizeof(func_t));
        function->n_para = 0; //Parameter-Zahl auf 0 setzen
         int result = insertSymbol(FUNC, _ret_val, _name, (int) function,sizeof(func_t*));//!!long || int (depends on architecture)
        if(result!=0) //falls insertSymbol fehlgeschlagen ist
            return result;
    }
    else
    {
        //löscht Einträge und Information der alten Defitionn falls schon in Symboltabelle vorhanden

        function = sym->var.func_ptr;
        function->n_para = 0;
        freeScope(function->scope);
        function->scope = (scope_list_t*)0;
    }

    //ändert den Scope auf die Function ab
    crnt_scope = &(function->scope);
    crnt_pos = &(function->scope);

    return 0;
}



void setN_Para(char* _name, int _n_para)
{
    //setzt die Anzahl von Übergabeparameter einer Funktion

    scope_list_t* sym = getSymbolInScope(global_scope,_name);
    sym->var.func_ptr->n_para = _n_para;
}

void endFunction()
{
    //setzt den aktuellen Scope wieder auf den Globalenscope
    scope_list_t** end_pos = &global_scope;

    while( *end_pos != 0)
    {
        end_pos = &((*end_pos)->next);
    }

    crnt_pos = end_pos;
    crnt_scope = &global_scope;

}

