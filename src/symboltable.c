#include <stdio.h>

#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_


enum type {
    INTEGER, VOID
};		


int nextRegister = 0; //datentyp checken

typedef struct symbol symbol_t;

typedef struct scope_list{
    symbol_t* sym;
    struct scope_list* next;
} scope_list_t;

typedef struct symbol {
	union {

		struct var {
			int type;
			int address;
			char* name;
		}var_t;

		struct func {
			int type;
			int address;
			char* name;
			scope_list_t* scope;
			scope_list_t* scope_para;
		}func_t;

	};
}symbol_t;

scope_list_t global = {};
scope_list_t function_scope = NULL;



int insertVariable(char* name){
    symbol_t myVar = {INTEGER, nextRegister, name};
    scope_list_t* newElement;
    
    malloc(newElement, sizeof(scope_list_t));
    newElement.sym = myVar;

    scope_list_t globalList;
    globalList = global.next;
    while(globalList != 0){
        globalList = globalList.next;
    }
    // does something
    globalList.next = newElement;

}

int insertFunction(symbol_t* function);

symbol_t* getSymbol(char* name);

//globe-switch
void beginFunction(void);
void endFunction(void);



#endif
