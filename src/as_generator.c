#include "stdlib.h"
#include "stdio.h"

FILE* file;
ir_code_t const * const ir;
scope_list_t const * const sym;

int genAssembly( char const * const  _file_name, ir_code_t const * const _ir_entry, scope_list_t const * const _sym_table)
{
    file = fopen(_file_name,"w");
    
    ir = _ir_entry;
    sym = _sym_table;
    
    fprintGlobalVar();

    fclose(file);
}

int fprintGlobalVar()
{
    
}
