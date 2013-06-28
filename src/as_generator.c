#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "symboltable.h"
#include "ir_generator.h"


FILE*           file;
ir_code_t*      ir;
scope_list_t*   st;
scope_list_t*   aReg[]={NULL,NULL,NULL,NULL};
int             sp=0;

void storeRegInMem(int reg)
{
    if(aReg[reg]->address == -1)
    {
        fprintf(file,"  SUBI $sp, $sp, 4\n  SW $a%d, 0($sp)\n", reg);
        aReg[reg]->address = sp;
        sp+=4;
    }
    else
    {
        int memPos = sp - (aReg[reg]->address);
        fprintf(file,"  ADDI $sp, $sp, %d\n  SW $a%d, 0($sp)\n  SUBI $sp, $sp, %d\n",memPos, reg, memPos);
    //optimize to one SW
    }

}


void loadSymInReg(scope_list_t* sym, int reg)
{
    if(aReg[reg]!=NULL)
    {
        storeRegInMem(reg);
    }
    if(sym->address!=-1)
    {
        int memPos = sp - (aReg[reg]->address);
        fprintf(file,"  ADDI $sp, $sp, %d\n  LW $a%d, 0($sp)\n  SUBI $sp, $sp, %d\n",memPos, reg, memPos);
    }
    aReg[reg]=sym;
}


int getRegNum(scope_list_t* sym)
{
    static int shift = -1;
    shift++;
    shift=(shift)%4;

    //fifo
    for(int i = 0; i<4 ; i++ )
    {
        if(aReg[i]==sym)
        {
            return i;
        }
    }

    loadSymInReg(sym,shift);
    return shift;
}
/*
void assignASSC(scope_list_t* _entry)
{
    int reg = getRegNum(_entry);
    fprintf(file,"  SUBI $sp, $sp, 4\n  LI $a%d, %d\n  SW $a%d, 0($sp)", reg, _entry->var.value, reg);
    _entry->address = sp;
    sp+=4;
}
*/

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
    ir_code_t*      entry = funcIr->next;
    scope_list_t*   symTb = funcIr->firstPara->var.func_ptr->scope;

    fprintf(file, "%s:\n", funcIr->firstPara->name);
    fprintf(file, "  SUBI $sp, $sp, 8\n"); 
    fprintf(file, "  SW $ra, 4($sp)\n"); 
    fprintf(file, "  SW $fp, 0($sp)\n"); 
    fprintf(file, "  MOVE $fp, $sp\n\n"); 


    for(;entry != NULL; entry=entry->next)
    {
        switch(entry->opcode)
        {
            case OP_ASSC:   fprintf(file,"  LI $a%d, %d",getRegNum(entry->firstPara),entry->firstPara->var.value);break;
            case OP_ASS:    fprintf(file,"  MOVE $a%d, $a%d",getRegNum(entry->firstPara),getRegNum(entry->secondPara));break;    
            case OP_ADD:    fprintf(file,"  ADD $a%d, $a%d, $a%d",getRegNum(entry->firstPara),getRegNum(entry->secondPara),getRegNum(entry->thirdPara));break;   
            case OP_SUB:    fprintf(file,"  SUB $a%d, $a%d, $a%d",getRegNum(entry->firstPara),getRegNum(entry->secondPara),getRegNum(entry->thirdPara));break;   
            case OP_MUL:    fprintf(file,"  MUL $a%d, $a%d, $a%d",getRegNum(entry->firstPara),getRegNum(entry->secondPara),getRegNum(entry->thirdPara));break;    
            case OP_DIV:    fprintf(file,"  DIV $a%d, $a%d, $a%d",getRegNum(entry->firstPara),getRegNum(entry->secondPara),getRegNum(entry->thirdPara));break;    
            case OP_MOD:    fprintf(file,"  REM $a%d, $a%d, $a%d",getRegNum(entry->firstPara),getRegNum(entry->secondPara),getRegNum(entry->thirdPara));break;    
            case OP_LOR:    fprintf(file,"  OR $a%d, $a%d, $a%d",getRegNum(entry->firstPara),getRegNum(entry->secondPara),getRegNum(entry->thirdPara));break;    
            case OP_LAND:   fprintf(file,"  AND $a%d, $a%d, $a%d",getRegNum(entry->firstPara),getRegNum(entry->secondPara),getRegNum(entry->thirdPara));break;   
            case OP_SL:     fprintf(file,"  SLL $a%d, $a%d, $a%d",getRegNum(entry->firstPara),getRegNum(entry->secondPara),getRegNum(entry->thirdPara));break;     
            case OP_SR:     fprintf(file,"  SRL $a%d, $a%d, $a%d",getRegNum(entry->firstPara),getRegNum(entry->secondPara),getRegNum(entry->thirdPara));break;     
            case OP_NEG:    fprintf(file,"  SUB $a%d, $zero, $a%d",getRegNum(entry->firstPara),getRegNum(entry->secondPara));break;    
            case OP_EQ:     fprintf(file,"\tADD $a%d, $a%d, $a%d",getRegNum(entry->firstPara),getRegNum(entry->secondPara),getRegNum(entry->thirdPara));break;     
            case OP_NE:     fprintf(file,"\tADD $a%d, $a%d, $a%d",getRegNum(entry->firstPara),getRegNum(entry->secondPara),getRegNum(entry->thirdPara));break;     
            case OP_GT:     fprintf(file,"\tADD $a%d, $a%d, $a%d",getRegNum(entry->firstPara),getRegNum(entry->secondPara),getRegNum(entry->thirdPara));break;     
            case OP_GE:     fprintf(file,"\tADD $a%d, $a%d, $a%d",getRegNum(entry->firstPara),getRegNum(entry->secondPara),getRegNum(entry->thirdPara));break;     
            case OP_LT:     fprintf(file,"\tADD $a%d, $a%d, $a%d",getRegNum(entry->firstPara),getRegNum(entry->secondPara),getRegNum(entry->thirdPara));break;     
            case OP_LE:     fprintf(file,"\tADD $a%d, $a%d, $a%d",getRegNum(entry->firstPara),getRegNum(entry->secondPara),getRegNum(entry->thirdPara));break;    
            case OP_GO:     fprintf(file,"  B %s", (entry->firstPara?entry->firstPara->name:"?"));break;
            case OP_GOT:    fprintf(file,"  BNEQ %s, $zero, %s", (entry->secondPara?entry->secondPara->name:"?\0"), (entry->firstPara?entry->firstPara->name:"?\0"));break;
            case OP_GOF:    fprintf(file,"  BEQZ %s, %s", (entry->secondPara?entry->secondPara->name:"?\0"), (entry->firstPara?entry->firstPara->name:"?\0"));break;
            case OP_RET:    fprintf(file,"  JR $ra");return;break;
            case OP_RETN:   fprintf(file,"  JR $ra");return;break;//return value
            case OP_CAL:    fprintf(file,"  BAL %s", entry->firstPara->name);break;
            case OP_CALN:   fprintf(file,"  BAL %s", entry->firstPara->name);break;//parameter
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


