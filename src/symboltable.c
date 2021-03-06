#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "symboltable.h"

static scope_list_t*  global_scope = (scope_list_t*) 0;
static scope_list_t** crnt_scope   = &global_scope; //crnt_scope zeigt auf den aktuellen scope
static scope_list_t** crnt_pos     = &global_scope; //crnt_pos zeigt auf nächste freie Stelle

void printTable();

/**
 * \brief Searches for a symbol in a specified scope
 * \param _scope The name of the scope (global or a specified funtion-scope)
 * \param _name The name of the searched symbol
 * \return A pointer of the symbol which was searched
 *
 *         It returns 0 if the searched symbol was not found
 */
scope_list_t* getSymbolInScope(scope_list_t* _scope, char* _name)
{
    scope_list_t* sym = _scope;

    while(sym != 0 )
    {
        if(strcmp(sym->name,_name) == 0)
            break;
        sym = sym->next;
    }

    return sym;
}

/**
 * \brief frees allocated space of a scope
 * It starts with the symbol the parameter is pointing to
 * \param __scope The scope which shall be cleaned / freed
 */
void freeScope(scope_list_t* _scope)
{
    if(_scope->next != 0)
        freeScope(_scope->next);
    free(_scope->name);
    free(_scope);
}

/**
 * \brief Looks up a symbol of the current (function) scope or if actual scope
 * 		  is not a function scope or no local symbol with the specified name is
 * 		  defined in the function scope looks up the symbol in the global scope
 * \param _name The name of the searched symbol
 * \return A pointer of the symbol which was searched
 *
 *         It returns 0 if the searched symbol was not found
 */
scope_list_t* getSymbol(char* _name)
{
    //liefert ein Symbol aus dem Funktionsscope bzw Globalenscope

    scope_list_t* result = (scope_list_t*) 0;

    result = getSymbolInScope(*crnt_scope,_name);

    if(result == 0 && *crnt_scope != global_scope)
        result = getSymbolInScope(global_scope,_name);

    return result;
}

/**
 * \param _type The type of the symbol (FUNC/VAR)
 * \param _var_type The type of the variable (if it is a variable)
 * \param _name The name of the variable
 * \param _value The value of the variable
 * \param _size The size of the variable
 * \return Returns 0 for success
 * 		   Returns -1 if symbol with this name already exists
 */
int insertSymbol(int _type, int _var_type, char* _name, int _value, int _size)
{

    if(getSymbolInScope(*crnt_scope,_name) != 0)
        return -1;
    if(_type == VAR && _var_type == V_VOID)
    	return -2;

    scope_list_t* new_variable = (scope_list_t*) malloc(sizeof(scope_list_t));
    int name_length = strlen(_name);

    new_variable->type     = _type;
    new_variable->var_type = _var_type;
    new_variable->name     = (char*) malloc(name_length + 1);
    new_variable->address  = -1;
    new_variable->size     = _size;
    new_variable->next     = (scope_list_t*) 0;

    // Da Pointer und int gleiche Byte-Zahl, benutze int im 32-Bit-System
    if(_type == VAR)
        new_variable->var.value     = _value;
    else
        new_variable->var.func_ptr  = (func_t*) _value;

    strcpy(new_variable->name, _name);
    //same as (*new_variable).name

    //setzt den aktuellen Scope eins weiter
    *crnt_pos = new_variable;
    crnt_pos  = &((**crnt_pos).next);

    printTable();    
    return 0; 
}

/**
 * \brief Inserts a new function into the symboltable and changes
 * 		  the scope to functionscope of the new function
 * \param _ret_val The ret value INT/VOID
 * \param _name The name of the new function
 * \return Returns 0 for success
 * 		   Returns -1 if a function with this name already exists
 * 		   Returns -2 if currently not in global scope
 * 		   Returns -3 if different return value
 * 		   Returns -4 variable with the samename in global scope
 */
int beginFunction(int _ret_val, char* _name)
{
    scope_list_t* sym = getSymbolInScope(global_scope,_name);
    func_t* function;

    if(sym != 0 && sym->type==VAR)
        return -4;
    if(*crnt_scope != global_scope)
        return -2;
    

    if(sym == 0)
    {
        //fügt die Function in die Symboltabelle ein wenn die Funktion noch nicht definiert wurde

        function = (func_t*) malloc(sizeof(func_t));
        function->scope  = (scope_list_t*) 0;
        function->decl   = 0;
        function->n_para = -1; //Parameter-Zahl auf -1 setzen
        int result = insertSymbol(FUNC, _ret_val, _name, (int) function,0);//!!long || int (depends on architecture)
        if(result!=0) //falls insertSymbol fehlgeschlagen ist
            return result;
    }
    else
    {
        //löscht alte einträge
        function = sym->var.func_ptr;
        if(function->scope!=0)
        {
            freeScope(function->scope);
            function->scope = (scope_list_t*)0;
        }
    }

    //ändert den Scope auf die Function ab
    crnt_scope = &(function->scope);
    crnt_pos = &(function->scope);

    if(sym!=0 && sym->var_type != _ret_val)
        return -3;
    return 0;
}


/**
 * \brief Sets the number of parameters of a function
 * \param __name The name of the function
 * \param _n_para The number of parameters
 * \return -1 different number of parameters
 * 			0 successfull
 */
int setN_Para(char* _name, int _n_para)
{
    //setzt die Anzahl von Übergabeparameter einer Funktion
    
    scope_list_t* sym = getSymbolInScope(global_scope,_name);

    if(sym->var.func_ptr->n_para != _n_para && sym->var.func_ptr->n_para != -1)
        return -1;

    sym->var.func_ptr->n_para = _n_para;

    return 0;
}
/** 
 * \brief Ends the function by changing the current scope back to globalscope
 * \return 0 successfull
 * \return -1 already declared
 */
//needs return type check
int endFunction(char* _name, int _decl)
{
    scope_list_t* function = getSymbol(_name);
    if(function->var.func_ptr->decl)
           return -1;
    function->var.func_ptr->decl = _decl;

    //changes current scope
    scope_list_t** end_pos = &global_scope;

    while( *end_pos != 0)
    {
        end_pos = &((*end_pos)->next);
    }

    crnt_pos = end_pos;
    crnt_scope = &global_scope;
    return 0;
}

scope_list_t* getSymbolTable()
{
    return global_scope;
}

/**
Error in /home/estamm/Gits/compilerbau/test/tests/success/t21.s line 20 column 3: "GT" is not a recognized operator
 * \brief Prints the whole symboltable into a file symboltable.log for debugging
 */
void printTable()
{
    scope_list_t* entry = global_scope;
    scope_list_t* func_entry;
    FILE*         file = fopen("symboltable.log","w");
    const char* string_type [] = {"Function","Variable","Constant"};
    const char* string_var []  = {"int","void"};

    fprintf(file,"\n\nTYP, VAR_TYPE, NAME, ADDRESS, Value, Size\n");
    
    while(entry != 0)
    {
        if(entry->type==VAR )
            fprintf(file,"%s, %s, %s, %d, %d %d\n",string_type[(int)entry->type],string_var[(int)entry->var_type],entry->name,entry->address,entry->var.value, entry->size);
        else            
            fprintf(file,"%s, %s, %s, %d, %d\n",string_type[(int)entry->type],string_var[(int)entry->var_type],entry->name,entry->address,entry->var.func_ptr->n_para);
        
        if(entry->type == FUNC)
        {
            func_entry = entry->var.func_ptr->scope;
            while(func_entry != 0)
            {
                fprintf(file,"\t%s, %s, %s, %d, %d\n",string_type[(int)func_entry->type],string_var[(int)func_entry->var_type],func_entry->name,func_entry->address,func_entry->var.value);
                func_entry = func_entry->next;
            }
        }
        entry = entry->next;
    }


    fclose(file);
}

