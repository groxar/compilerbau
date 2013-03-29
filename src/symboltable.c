#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_


enum type {
    INTEGER, VOID
};		


int nextRegister = 0; //Datentyp checken

typedef struct symbol symbol_t; //symbol_t bekannt machen

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
    global_list = global_scope.next;
    while(global_list != 0){
        global_list = global_list.next;
    } //am Ende von global_list angekommen

    // noch unfertig
    if(function_scope != 0){ //füge Variable in den Scope der Funktion ein
    	global_list.next = newElement;

    }else{ //Füge Variable in den globalen Scope ein
    	global_scope.next = newElement;

    }

    // ToDo: Pointer auf Scopelist setzen und Variable in die Scopelist einfügen

    return newElement; //Pointer auf neue Variable zurückgeben

    // ToDo: Testen


}

int insertFunction(symbol_t* function){
	//ToDo
}


symbol_t* getSymbol(char* name){
	// ToDo
	/* zuerst Scope der Funktion checken und nur wenn wir nicht in einer Funktion
	 * sind oder es das Symbol dort nicht gibt den globalen Scope checken
	 *  (--> lokal überschreibt global)
	 */
	if(function_scope != 0){ //Scope der Funktion

	}else{ //globaler Scope

	}
}

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
