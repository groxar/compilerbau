#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "symboltable.h"
#include "ir_generator.h"


FILE*           file;
ir_code_t*      ir;
scope_list_t*   st;
scope_list_t*   aReg[]={NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
int             sp=-4;

void storeRegInMem(int reg)
{
    if(aReg[reg]->address == -1)
    {
        fprintf(file,"  SUBI $sp, $sp, 4\n  SW $t%d, 0($sp)  #push %s in memory\n", reg, aReg[reg]->name);
        sp+=4;
        aReg[reg]->address = sp;
    }
    else
    {
        int memPos = sp - (aReg[reg]->address);
        fprintf(file,"  SW $t%d, %d($sp) #store %s in memory\n", reg, memPos, aReg[reg]->name);
        //fprintf(file,"  ADDI $sp, $sp, %d\n  SW $a%d, 0($sp)\n  SUBI $sp, $sp, %d\n",memPos, reg, memPos);
    //optimize to one SW
    }

}


void loadSymInReg(scope_list_t* sym, int reg)
{
    if(aReg[reg]!=NULL)
    {
        storeRegInMem(reg);
    }
    if(sym!=NULL &&sym->address!=-1)
    {
        int memPos = sp - (sym->address);
        fprintf(file,"  LW $t%d, %d($sp)  #load %s from memory in reg\n", reg, memPos, sym->name);
        //fprintf(file,"  ADDI $sp, $sp, %d\n  LW $a%d, %d($sp)\n  SUBI $sp, $sp, %d\n",memPos, reg, memPos);
    }
    aReg[reg]=sym;
}


int getRegNum(scope_list_t* sym)
{
    static int shift = -1;

    //fifo
    for(int i = 0; i<10 ; i++ )
    {
        if(aReg[i]==sym)
        {
            return i;
        }
    }

    shift++;
    shift=(shift)%10;

    loadSymInReg(sym,shift);
    return shift;
}

void storeVar()
{
    for(int i = 0; i< 10; i++)
    {
        if(aReg[i]!=NULL)
            loadSymInReg(NULL,i);
    }
}

void fprintFuncPara(ir_code_t* entry)
{
    int i = 0;
    para_list_t* pl = entry->paraList;

    //parameter
    while(pl!=NULL)
    {
        fprintf(file,"  MOVE $a%d, $t%d #para %d\n",i,getRegNum(pl->para),i);
        i++;
        pl=pl->next;
    }
    storeVar();
    fprintf(file,"  JAL %s", entry->firstPara->name);
    
    //return parameter
    if(entry->firstPara->var_type!=V_VOID)
        fprintf(file,"\n  MOVE $t%d, $v0 #return value",getRegNum(entry->secondPara));
}

void fprintFuncEnd(scope_list_t* func, scope_list_t* retVal)
{
    if(retVal!=NULL)
        fprintf(file,"  MOVE $v0, $t%d\n",getRegNum(retVal));

    if(!strcmp(func->name,"main"))
    {
        fprintf(file,"  LI $v0, 10\n  SYSCALL\n\n");
    }
    else
    {
        fprintf(file,"  LW $fp, 0($sp)\n");
        fprintf(file,"  LW $ra, 4($sp)\n");
        fprintf(file,"  ADDI $sp, $sp, 8\n");
        fprintf(file,"  JR $ra\n\n");
    }
}

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
    scope_list_t*   symP  = symTb;
    int             n_para= funcIr->firstPara->var.func_ptr->n_para;

    fprintf(file, "%s:\n", funcIr->firstPara->name);
    fprintf(file, "#Function Prolog\n"); 
    fprintf(file, "  SUBI $sp, $sp, 8\n"); 
    fprintf(file, "  SW $ra, 4($sp)\n"); 
    fprintf(file, "  SW $fp, 0($sp)\n"); 
    fprintf(file, "  MOVE $fp, $sp\n\n"); 

    //parameter handling
    for(int i = 0; i < n_para;i++)
    {
        fprintf(file,"  MOVE $t%d, $a%d\n",getRegNum(symP),i);
        symP = symP->next;
    }


    for(;entry != NULL; entry=entry->next)
    {
        switch(entry->opcode)
        {
            case OP_ASSC:   fprintf(file,"  LI $t%d, %d         #%s",getRegNum(entry->firstPara),entry->firstPara->var.value,entry->firstPara->name);break;
            case OP_ASS:    fprintf(file,"  MOVE $t%d, $t%d",getRegNum(entry->firstPara),getRegNum(entry->secondPara));break;    
            case OP_ADD:    fprintf(file,"  ADD $t%d, $t%d, $t%d",getRegNum(entry->firstPara),getRegNum(entry->secondPara),getRegNum(entry->thirdPara));break;   
            case OP_SUB:    fprintf(file,"  SUB $t%d, $t%d, $t%d",getRegNum(entry->firstPara),getRegNum(entry->secondPara),getRegNum(entry->thirdPara));break;   
            case OP_MUL:    fprintf(file,"  MUL $t%d, $t%d, $t%d",getRegNum(entry->firstPara),getRegNum(entry->secondPara),getRegNum(entry->thirdPara));break;    
            case OP_DIV:    fprintf(file,"  DIV $t%d, $t%d, $t%d",getRegNum(entry->firstPara),getRegNum(entry->secondPara),getRegNum(entry->thirdPara));break;    
            case OP_MOD:    fprintf(file,"  REM $t%d, $t%d, $t%d",getRegNum(entry->firstPara),getRegNum(entry->secondPara),getRegNum(entry->thirdPara));break;    
            case OP_LOR:    fprintf(file,"  OR  $t%d, $t%d, $t%d",getRegNum(entry->firstPara),getRegNum(entry->secondPara),getRegNum(entry->thirdPara));break;    
            case OP_LAND:   fprintf(file,"  AND $t%d, $t%d, $t%d",getRegNum(entry->firstPara),getRegNum(entry->secondPara),getRegNum(entry->thirdPara));break;   
            case OP_SL:     fprintf(file,"  SLL $t%d, $t%d, $t%d",getRegNum(entry->firstPara),getRegNum(entry->secondPara),getRegNum(entry->thirdPara));break;     
            case OP_SR:     fprintf(file,"  SRL $t%d, $t%d, $t%d",getRegNum(entry->firstPara),getRegNum(entry->secondPara),getRegNum(entry->thirdPara));break;     
            case OP_NEG:    fprintf(file,"  SUB $t%d, $zero,$t%d",getRegNum(entry->firstPara),getRegNum(entry->secondPara));break;    
            case OP_EQ:     fprintf(file,"  SEQ $t%d, $t%d, $t%d",getRegNum(entry->firstPara),getRegNum(entry->secondPara),getRegNum(entry->thirdPara));break;     
            case OP_NE:     fprintf(file,"  SNE $t%d, $t%d, $t%d",getRegNum(entry->firstPara),getRegNum(entry->secondPara),getRegNum(entry->thirdPara));break;     
            case OP_GT:     fprintf(file,"  SGT $t%d, $t%d, $t%d",getRegNum(entry->firstPara),getRegNum(entry->secondPara),getRegNum(entry->thirdPara));break;     
            case OP_GE:     fprintf(file,"  SGE $t%d, $t%d, $t%d",getRegNum(entry->firstPara),getRegNum(entry->secondPara),getRegNum(entry->thirdPara));break;     
            case OP_LT:     fprintf(file,"  SLT $t%d, $t%d, $t%d",getRegNum(entry->firstPara),getRegNum(entry->secondPara),getRegNum(entry->thirdPara));break;     
            case OP_LE:     fprintf(file,"  SLE $t%d, $t%d, $t%d",getRegNum(entry->firstPara),getRegNum(entry->secondPara),getRegNum(entry->thirdPara));break;    
            case OP_GO:     fprintf(file,"  B %s", (entry->firstPara?entry->firstPara->name:"?"));break;
            case OP_GOT:    fprintf(file,"  BNEZ $t%d, %s", getRegNum(entry->secondPara),entry->firstPara->name);break;
            case OP_GOF:    fprintf(file,"  BEQZ $t%d, %s", getRegNum(entry->secondPara),entry->firstPara->name);break;
            case OP_RET:    fprintFuncEnd(funcIr->firstPara,NULL);return;
            case OP_RETN:   fprintFuncEnd(funcIr->firstPara,entry->firstPara);return;
            case OP_CAL:    
            case OP_CALN:   fprintFuncPara(entry);break;
            case OP_AL:     fprintf(file,"\t%s = %s[ %s ]", entry->firstPara->name, entry->secondPara->name, entry->thirdPara->name);break;
            case OP_AS:     fprintf(file,"\t%s[ %s] = %s", entry->secondPara->name, entry->thirdPara->name, entry->firstPara->name);break;
            case OP_LNOT:   fprintf(file,"\t%s = !%s", entry->firstPara->name, entry->secondPara->name);break;
            case LABEL:     if(entry->firstPara->name[0]=='.')
                            {
                                fprintf(file," %s",entry->firstPara->name);
                                break;
                            }
                            else
                            {
                                fprintFuncEnd(funcIr->firstPara,NULL);
                                return;
                            }
        }
        fprintf(file,"\n");
    }
    fprintFuncEnd(entry->firstPara,NULL); 
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


