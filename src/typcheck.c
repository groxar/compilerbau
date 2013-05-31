#pragma once

#include "diag.h"
#include "string.h"
#include "stdio.h"
#include "symboltable.h"
#include "ir_generator.h"
#include "parser.h"
#include "typcheck.h"


void function_definition_tc1(char* func_begin, int n_para, scope_list_t** crntFunc){
	addLabel(func_begin);
	if(setN_Para(func_begin,n_para))
		yyerror("Different number of parameters");
	*crntFunc = getSymbol(func_begin);
}

void function_definition_tc(char* func_def, scope_list_t** crntFunc){
    if(endFunction(func_def,1))
        yyerror("Function was previously defined");
    *crntFunc = (scope_list_t*) 0;
}

void function_declaration_tc(char* func_dec, int n_para){
    if(setN_Para(func_dec,n_para))
        yyerror("Different number of parameters");
    if(endFunction(func_dec,0))
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
           case -4: yyerror("A variable or a function with this name already exists"); b_func=genLabel();break;
    }
 	return b_func;
}

void return_tc(int var_type, scope_list_t* crntFunc){
    if(crntFunc!=0)
    {
    	if(var_type!=crntFunc->var_type)
        {
            yyerror("Wrong return type");
        }
    }
    else
        yyerror("Return has to be within a function");
}

void return_tc2(scope_list_t* crntFunc){
    if(crntFunc!=0) {
    	if(crntFunc->var_type != VOID){
    		yyerror("No return value defined");
    	}
    }
       else
           yyerror("Return has to be within a function");
}

scope_list_t* function_call_tc(char* id, scope_list_t* callFunc){
	scope_list_t *func_call;
	callFunc= getSymbol(id);
	if(callFunc)
	{
		if(callFunc->var.func_ptr->n_para!=0){
			yyerror("Function doesnt expect any parameter");
		}
		else{
			func_call=callFuncIR(callFunc);
		}
	}
	else
	{
		yyerror("Function not found");
	}
	return func_call;
}

scope_list_t* function_call_tc2(char* id, scope_list_t* callFunc, int n_para, char buffer){
	scope_list_t *func_call;
	callFunc= getSymbol(id);
	if(callFunc)
	{
		if(callFunc->var.func_ptr->n_para!=n_para)
		{
			sprintf(buffer,"Function expects %d parameter" , callFunc->var.func_ptr->n_para);
			yyerror(buffer);
		}
		else
			func_call=callFuncIR(callFunc);
	}
	else
	{
		yyerror("Function not found");
	}
	return func_call;
}

void function_call_parameters_tc(scope_list_t* func_call_para, int n_para, char buffer, scope_list_t* callFunc, scope_list_t* callFuncPara){
	callFuncPara = callFunc->var.func_ptr->scope;
	if(callFunc->var.func_ptr->n_para > n_para && callFuncPara != 0 )
	{
		for(int i = 0 ; i < n_para; i++)
		{
			callFuncPara = callFuncPara->next;
		}

		if(callFuncPara->var_type != func_call_para->var_type)
			yyerror("Function parameter type missmatch");
		else if(callFuncPara->size != func_call_para->size)
		{
			sprintf(buffer,"Array of size %d expected", callFuncPara->size);
			yyerror(buffer);
		}
	}
}
