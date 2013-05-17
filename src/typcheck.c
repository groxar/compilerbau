#pragma once

	#include "diag.h"
    #include "string.h"
    #include "stdio.h"
    #include "symboltable.h"
    #include "ir_generator.h"




void function_definition1(char* $1){
	addLabel($1);
	        if(setN_Para($1,n_para))
	            yyerror("Different number of parameters");
	        crntFunc = getSymbol($1);
}

void function_definition2(char* $1){
    if(endFunction($1,1))
        yyerror("Function was previously defined");
    crntFunc = (scope_list_t*) 0;
}

void function_declaration1(char* $1){
    if(setN_Para($1,n_para))
        yyerror("Different number of parameters");
    if(endFunction($1,0))
        yyerror("Function was previously defined");
}

