#include "symboltable.h"
#include "stdio.h"

int check_func_def(int _error_code, int _line, int _col, int _name){
   switch(_error_code)
   {
        case -1: printf("Error line %d col %d function with the name %s was previously defined", _line,_col,_name);break;
   }
   return _error_code;
}

int check_func_end(int _error_code, int _line, int _col, int _name){
    switch(_error_code)
    {
    }
    return _error_code;
}
int check_var_def(int _error_code, int _line, int _col, int _name){
    switch(_error_code)
    {
        case -1: printf("Error line %d col %d variable with the name %s was previously defined", _line,_col,_name);break;
    }
    return _error_code;
}
