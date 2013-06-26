#include "stdlib.h"
#include "stdio.h"
#include "symboltable.h"
#include "ir_generator.h"


FILE* file;
ir_code_t* ir;
scope_list_t* st;

void fprintGlobalVar()
{
    fprintf(file,".data\n");

    scope_list_t *entry = st;
    for(;entry!=NULL;entry=entry->next)
    {
        if(entry->type==VAR)
        {
            fprintf(file, "%s:\n",entry->name);
            
            if(entry->size == 1)
                fprintf(file, "  .word %d\n", entry->var.value);
            else
                fprintf(file, "  .word 0 : %d\n", entry->size);
            fprintf(file, "  .align 4\n");
        }
    }
    fprintf(file,"\n");
}


void fprintFunc(ir_code_t* funcIr)
{
    fprintf(file, "%s:\n", funcIr->firstPara->name);
    fprintf(file, "  ADDI $sp, $sp, -8\n"); 
    fprintf(file, "  LI $5, 0\n"); 
    fprintf(file, "  SW $5, 4($sp)\n"); 
    fprintf(file, "  LI $5, 0\n"); 
    fprintf(file, "  SW $5, 0($sp)\n"); 

    ir_code_t* entry = funcIr->next;

    for(;entry != NULL; entry=entry->next)
    {
        switch(entry->opcode)
        {
            case OP_ASSC:   fprintf(file,"\t%s = %d", entry->firstPara->name, entry->firstPara->var.value);break;
            case OP_ASS:    fprintf(file,"\t%s = %s", entry->firstPara->name, entry->secondPara->name);break;
            case OP_ADD:    fprintf(file,"\t%s = %s + %s", entry->firstPara->name, entry->secondPara->name, entry->thirdPara->name);break;
            case OP_SUB:    fprintf(file,"\t%s = %s - %s", entry->firstPara->name, entry->secondPara->name, entry->thirdPara->name);break;
            case OP_MUL:    fprintf(file,"\t%s = %s * %s", entry->firstPara->name, entry->secondPara->name, entry->thirdPara->name);break;
            case OP_DIV:    fprintf(file,"\t%s = %s / %s", entry->firstPara->name, entry->secondPara->name, entry->thirdPara->name);break;
            case OP_MOD:    fprintf(file,"\t%s = %s %% %s", entry->firstPara->name, entry->secondPara->name, entry->thirdPara->name);break;
            case OP_LOR:    fprintf(file,"\t%s = %s | %s", entry->firstPara->name, entry->secondPara->name, entry->thirdPara->name);break;
            case OP_LAND:   fprintf(file,"\t%s = %s & %s", entry->firstPara->name, entry->secondPara->name, entry->thirdPara->name);break;
            case OP_SL:     fprintf(file,"\t%s = %s << %s", entry->firstPara->name, entry->secondPara->name, entry->thirdPara->name);break;
            case OP_SR:     fprintf(file,"\t%s = %s >> %s", entry->firstPara->name, entry->secondPara->name, entry->thirdPara->name);break;
            case OP_NEG:    fprintf(file,"\t%s = -%s", entry->firstPara->name, entry->secondPara->name);break; 
            case OP_EQ:     fprintf(file,"\t%s = %s == %s", entry->firstPara->name, entry->secondPara->name, entry->thirdPara->name);break;
            case OP_NE:     fprintf(file,"\t%s = %s != %s", entry->firstPara->name, entry->secondPara->name, entry->thirdPara->name);break;
            case OP_GT:     fprintf(file,"\t%s = %s > %s", entry->firstPara->name, entry->secondPara->name, entry->thirdPara->name);break;
            case OP_GE:     fprintf(file,"\t%s = %s >= %s", entry->firstPara->name, entry->secondPara->name, entry->thirdPara->name);break;
            case OP_LT:     fprintf(file,"\t%s = %s < %s", entry->firstPara->name, entry->secondPara->name, entry->thirdPara->name);break;
            case OP_LE:     fprintf(file,"\t%s = %s <= %s", entry->firstPara->name, entry->secondPara->name, entry->thirdPara->name);break;
            case OP_GO:     fprintf(file,"\tgoto %s", (entry->firstPara?entry->firstPara->name:"?"));break;
            case OP_GOT:    fprintf(file,"\tif %s != 0 goto %s", (entry->secondPara?entry->secondPara->name:"?\0"), (entry->firstPara?entry->firstPara->name:"?\0"));break;
            case OP_GOF:    fprintf(file,"\tif %s == 0 goto %s", (entry->secondPara?entry->secondPara->name:"?\0"), (entry->firstPara?entry->firstPara->name:"?\0"));break;
            case OP_RET:    fprintf(file,"\treturn\n");return;break;
            case OP_RETN:   fprintf(file,"\treturn %s\n", entry->firstPara->name);return;break;
            case OP_CAL:    fprintf(file,"\tcall %s", entry->firstPara->name);break;
            case OP_CALN:   fprintf(file,"\tcallN ");break;
            case OP_AL:     fprintf(file,"\t%s = %s[ %s ]", entry->firstPara->name, entry->secondPara->name, entry->thirdPara->name);break;
            case OP_AS:     fprintf(file,"\t%s[ %s] = %s", entry->secondPara->name, entry->thirdPara->name, entry->firstPara->name);break;
            case OP_LNOT:   fprintf(file,"\t%s = !%s", entry->firstPara->name, entry->secondPara->name);break;
            case LABEL:     entry->firstPara->name[0]!='.'?fprintf(file, "%s", entry->firstPara->name):fprintf(file,"\t%s",entry->firstPara->name);
        }
        fprintf(file,"\n");
    }
}

void fprintGlobalFunc()
{
    ir_code_t* entry = ir;
    for(;entry != NULL; entry = entry->next)
    {
        if(entry->opcode == LABEL && entry->firstPara->name[0]!='.')
        {
            fprintFunc( entry);
        }
    }
}

int genAssembly( char const * const _file_name)
{
    file = fopen(_file_name,"w");
    
    ir = getQuadList();
    st = getSymbolTable();
    
    fprintGlobalVar();

    fprintf(file, ".text\n\n_start:\n  JAL main\n\n");

    fprintGlobalFunc();

    fclose(file);
    
    return 0;
}


