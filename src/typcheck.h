#pragma once


void function_definition_tc1(char* func_begin, int n_para, scope_list_t* crntFunc);

void function_definition_tc(char* func_def, scope_list_t* crntFunc);

void function_declaration_tc(char* func_dec, int n_para);

int variable_declaration_tc(int var_type, char* var_name, int arr_size);

char* function_begin_tc(int type, char* id);

void return_tc(int var_type, scope_list_t* crntFunc);

void return_tc2(scope_list_t* crntFunc);

scope_list_t* function_call_tc(char* id, scope_list_t* callFunc);

scope_list_t* function_call_tc2(char* id, scope_list_t* callFunc, int n_para, char buffer);

void function_call_parameters_tc(scope_list_t* func_call_para, int n_para, char buffer, scope_list_t* callFunc, scope_list_t* callFuncPara);
