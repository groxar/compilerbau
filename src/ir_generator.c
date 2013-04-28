#include "symboltable.h"
#include "ir_generator.h"
#include "stdlib.h"
#include "stdio.h"
#include "parser.h"

int reg_counter = 0;
int lbl_counter = 0;
ir_code_t* global   = (ir_code_t*) 0;
ir_code_t* pos_crnt = (ir_code_t*) 0;

void printIR();

void initIR()
{
    FILE* file = fopen("ir.log","w");
    fprintf(file,"intermediate code log");
    fclose(file);
}

void quadList(enum opcodes opcode, struct scope_list *firstPara, struct scope_list *secondPara, struct scope_list *thirdPara){

	ir_code_t* quad = (ir_code_t*) malloc(sizeof(ir_code_t));

	quad->opcode        = opcode;
	quad->firstPara     = firstPara;
	quad->secondPara    = secondPara;
	quad->thirdPara     = thirdPara;
	quad->prev          = pos_crnt;

	if(pos_crnt != 0){
		pos_crnt->next = quad;
	}
    else{
        global = quad;
    }

	pos_crnt = quad;
    printIR();
}

scope_list_t* calcIR(enum opcodes opcode, struct scope_list *secondPara, struct scope_list *thirdPara)
{
    scope_list_t* firstPara = (secondPara->name[0]=='#' && thirdPara->name[0]!='#' )? thirdPara : secondPara;

    quadList(opcode, firstPara, secondPara, thirdPara);
    
    return firstPara;
}

scope_list_t* assignIR(struct scope_list *firstPara, struct scope_list *secondPara)
{
    quadList(OP_ASS, firstPara, secondPara, NULL);
    return firstPara;
}



scope_list_t* genLabel(char* name)
{
    return (scope_list_t*)0;
}

//add error handle if name already obtained 
scope_list_t* genTemp(int var_type, int value){

    char buffer[16];
    
    sprintf(buffer, "#%d", reg_counter);
    insertSymbol(VAR, var_type, buffer, value, 1);
    reg_counter++;

    return getSymbol(buffer);
}


void printIR()
{
    ir_code_t* entry = global;
    FILE* file       = fopen("ir.log","w");

    fprintf(file,"\n\n\n");
    while(entry != 0)
    {
        switch(entry->opcode)
        {
            case OP_ASS: fprintf(file,"%s = %s", entry->firstPara->name, entry->secondPara->name);break;
            case OP_ADD: fprintf(file,"%s = %s + %s", entry->firstPara->name, entry->secondPara->name, entry->thirdPara->name);break;
            case OP_SUB: fprintf(file,"%s = %s - %s", entry->firstPara->name, entry->secondPara->name, entry->thirdPara->name);break;
            case OP_MUL: fprintf(file,"%s = %s * %s", entry->firstPara->name, entry->secondPara->name, entry->thirdPara->name);break;
            case OP_DIV: fprintf(file,"%s = %s / %s", entry->firstPara->name, entry->secondPara->name, entry->thirdPara->name);break;
            case OP_MOD: fprintf(file,"%s = %s % %s", entry->firstPara->name, entry->secondPara->name, entry->thirdPara->name);break;
            case OP_LOR: fprintf(file,"%s = %s | %s", entry->firstPara->name, entry->secondPara->name, entry->thirdPara->name);break;
            case OP_LAND: fprintf(file,"%s = %s & %s", entry->firstPara->name, entry->secondPara->name, entry->thirdPara->name);break;
            case OP_SL: fprintf(file,"%s = %s << %s", entry->firstPara->name, entry->secondPara->name, entry->thirdPara->name);break;
            case OP_SR: fprintf(file,"%s = %s >> %s", entry->firstPara->name, entry->secondPara->name, entry->thirdPara->name);break;
            case OP_NEG: fprintf(file,"%s = -%s", entry->firstPara->name, entry->secondPara->name);break; 
            case OP_EQ: fprintf(file,"goto %s if %s == %s", entry->firstPara->name, entry->secondPara->name, entry->thirdPara->name);break;
            case OP_NE: fprintf(file,"goto %s if %s != %s", entry->firstPara->name, entry->secondPara->name, entry->thirdPara->name);break;
            case OP_GT: fprintf(file,"goto %s if %s > %s", entry->firstPara->name, entry->secondPara->name, entry->thirdPara->name);break;
            case OP_GE: fprintf(file,"goto %s if %s >= %s", entry->firstPara->name, entry->secondPara->name, entry->thirdPara->name);break;
            case OP_LT: fprintf(file,"goto %s if %s < %s", entry->firstPara->name, entry->secondPara->name, entry->thirdPara->name);break;
            case OP_LE: fprintf(file,"goto %s if %s <= %s", entry->firstPara->name, entry->secondPara->name, entry->thirdPara->name);break;
            case OP_GO: fprintf(file,"goto %s", entry->firstPara->name);break;
            case OP_RET: fprintf(file,"return");break;
            case OP_RETN: fprintf(file,"return %s", entry->firstPara->name);break;
            case OP_CAL: fprintf(file,"call %s", entry->firstPara->name);break;
            case OP_CALN: fprintf(file,"callN %s", entry->firstPara->name);break;//NEEDs parameter
            case OP_AL: fprintf(file,"%s = %s[ %s ]", entry->firstPara->name, entry->secondPara->name, entry->thirdPara->name);break;
            case OP_AS: fprintf(file,"%s[ %s] = %s", entry->secondPara->name, entry->thirdPara->name, entry->firstPara->name);break;
            case OP_LNOT: fprintf(file,"%s = !%s", entry->firstPara->name, entry->secondPara->name);break;
        }
        fprintf(file,"\n");

        entry = entry->next;
    }


    fclose(file);
}


