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

scope_list_t global_scope = {};
scope_list_t function_scope = 0;



int insertVariable(char* name){
    symbol_t myVar = {INTEGER, nextRegister, name};
    scope_list_t* newElement;
    
    malloc(newElement, sizeof(scope_list_t));
    newElement.sym = myVar;

    scope_list_t global_list;
    global_list = global.next;
    while(global_list != 0){
        global_list = global_list.next;
    }

    // noch unfertig
    if(function_scope != 0){
    	global_list.next = newElement;

    }else{

    }
    // Pointer auf Scopelist setzen und Variable in die Scopelist einfügen
    // Testen


}

int insertFunction(symbol_t* function);

symbol_t* getSymbol(char* name);

//globe-switch
void beginFunction(void){
	scope_list_t* func_pointer;
	scope_list_t* temp_pointer;

	func_pointer = malloc(sizeof(scope_list_t));
	temp_pointer = global_scope.next;
	while(temp_pointer.next != 0){
		temp_pointer = temp_pointer.next;
	}
	temp_pointer.next = func_pointer;

	//Scope auf Funktion setzen
	function_scope = func_pointer;
}

void endFunction(void){
	function_scope = 0;
}


#endif
