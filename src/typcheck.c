#pragma once

#include "diag.h"
#include "string.h"
#include "stdio.h"
#include "symboltable.h"
#include "ir_generator.h"
#include "parser.h"




	static int n_para;
    scope_list_t* crntFunc = (scope_list_t*)0;
    scope_list_t* callFunc = (scope_list_t*)0;
    scope_list_t* callFuncPara = (scope_list_t*)0;
    char buffer[100];


void function_definition_tc1(char* $1){
	addLabel($1);
	        if(setN_Para($1,n_para))
	            yyerror("Different number of parameters");
	        crntFunc = getSymbol($1);
}

void function_definition_tc(char* $1){
    if(endFunction($1,1))
        yyerror("Function was previously defined");
    crntFunc = (scope_list_t*) 0;
}

void function_declaration_tc(char* $1){
    if(setN_Para($1,n_para))
        yyerror("Different number of parameters");
    if(endFunction($1,0))
        yyerror("Function was previously defined");
}

int variable_declaration_tc(int var_type, char* var_name, int arr_size){
 	int var_dec = -1;
	switch(insertSymbol(VAR,var_type,var_name,0,arr_size)){
     	case 0: var_dec = var_type; break;
     	case -1: yyerror("Double variable declaration"); break;
     	case -2: yyerror("Variable type can't be void"); break;
     }
	return var_dec;
}

char* function_begin_tc(int type, char* id){
	char* b_func = "";
 	switch(beginFunction(type, id)) {
           case 0:  b_func=id; break;
           case -1: yyerror("A function with this name already exists"); b_func=genLabel()->name;break;
           case -2: yyerror("Declaration of a function in a function is not allowed"); b_func=genLabel()->name;break;
           case -3: yyerror("Different return value"); b_func=genLabel()->name;break;
           case -4: yyerror("A Variable with this name already exists"); b_func=genLabel();break;
         }
}

void return_tc(int var_type, int crntFunc_var_type){
    if(crntFunc!=0)
    {
    	if(var_type!=crntFunc_var_type)
        {
            yyerror("Wrong return type");
        }
    }
    else
        yyerror("Return has to be within a function");
}

void return_tc2(int crntFunc_var_type){
    if(crntFunc!=0)
    {
    	if(crntFunc_var_type != VOID){
    		yyerror("No return value defined");
    	}
    }
       else
           yyerror("Return has to be within a function");
}
