#pragma once

#include "diag.h"
#include "string.h"
#include "stdio.h"
#include "symboltable.h"
#include "ir_generator.h"
#include "typcheck.h"


//#define voidCheck(a) (a!=0&&a->var_type!=VOID)?(a->size==1?a:yyerror("Reference access not allowed")):yyerror("Operations arent allowed on void")

scope_list_t* voidCheck(scope_list_t* _exp){
    
    if(_exp!=0 && _exp->var_type!=V_VOID)
    {
        if(_exp->size!=1)
        {
            yyerror("Reference access not allowed");
        }
    }
    else
    {
        yyerror("Operatrions arent allowed on void");
        _exp=genTemp(V_INT,0);
    }

    return _exp;
}

void function_definition_tc1(char* func_name, int n_para, scope_list_t** crntFunc){
	addLabel(func_name);
	if(setN_Para(func_name,n_para))
		yyerror("Different number of parameters");
	*crntFunc = getSymbol(func_name);
}

void function_definition_tc(char* func_name, scope_list_t** crntFunc){
    if(endFunction(func_name,1))
        yyerror("Function was previously defined");
    *crntFunc = (scope_list_t*) 0;
}

void function_declaration_tc(char* func_name, int n_para){
    if(setN_Para(func_name,n_para))
        yyerror("Different number of parameters");
    if(endFunction(func_name,0))
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
	char* b_func = malloc(sizeof(char)*(strlen(id)+4));
 	switch(beginFunction(type, id)) {
           case 0:  b_func = id; break;
           case -1: yyerror("A function with this name already exists"); 
                    sprintf(b_func,".%s",id);
                    endFunction(id,1);
                    beginFunction(type,b_func);
                    break;
           case -2: yyerror("Declaration of a function in a function is not allowed"); 
                    sprintf(b_func,".%s",id);
                    endFunction(id,1);
                    beginFunction(type,b_func);
                    break;
           case -3: yyerror("Already exist with different return value"); 
                    sprintf(b_func,".%s",id);
                    endFunction(id,1);
                    beginFunction(type,b_func);
                    break;
           case -4: yyerror("A variable with this name already exists"); 
                    sprintf(b_func,".%s",id);
                    beginFunction(type,b_func);
                    break;
    }
 	return b_func;
}

void return_tc(scope_list_t* expression, scope_list_t* crntFunc){
    if(crntFunc!=0)
    {
        genRetIR(expression);
    	if(expression->var_type!=crntFunc->var_type)
        {
            yyerror("Wrong return type");
        }
    }
    else
        yyerror("Return has to be within a function");
}

void return_tc2(scope_list_t* crntFunc){
    if(crntFunc!=0) {
        genRetIR(NULL);
    	if(crntFunc->var_type != V_VOID){
    		yyerror("No return value defined");
    	}
    }
       else
           yyerror("Return has to be within a function");
}

scope_list_t* function_call_tc(char* id, scope_list_t* callFunc){
    char* buffer = malloc(sizeof(char)*50);
	scope_list_t *func_call;
	callFunc= getSymbol(id);
	if(callFunc)
	{
		if(callFunc->var.func_ptr->n_para!=0){
            sprintf(buffer, "Function expects %d parameter", callFunc->var.func_ptr->n_para);
			yyerror(buffer);
		}
                func_call=callFuncIR(callFunc, NULL);
	}
	else
	{
        
		yyerror("Function not found");
	}
	return func_call;
}

scope_list_t* function_call_tc2(char* id, scope_list_t* callFunc, int n_para, para_list_t* pl){
    char* buffer = malloc(sizeof(char)*50);
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
			func_call=callFuncIR(callFunc, pl);
	}
	else
	{
		yyerror("Function not found");
	}
	return func_call;
}

void function_call_parameters_tc(scope_list_t* func_call_para, int n_para, scope_list_t* callFunc, scope_list_t* callFuncPara){
    char* buffer = malloc(sizeof(char)*50);
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
