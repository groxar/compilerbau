#include "include/uthash.h"

#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_


enum type {
    VARIABLE, FUNCTION
};				/* ----------  end of enum element  ---------- */


int nextRegister = 0; //datentyp checken


typedef struct symbol {
    int type;
    int address;
    char* name;
    UT_hash_handle hh;
} symbol_t;

typedef struct scope_element{
    symbol_t* sym;
    struct scope_element* next;
};


typedef struct global_scope {
    struct scope_element* function_scope;
};

int insertVariable(symbol_t* variable ){
;
}

int insertFuncition(symbol_t* function);

symbol_t* getSymbol(char* name);

void beginFunction(void);
void endFunction(void);



#endif
