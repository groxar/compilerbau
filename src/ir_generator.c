#include "symboltable.h"
#include "ir_generator.h"
#include "stdlib.h"
#include "stdio.h"
#include "parser.h"

int reg_counter = 0;
int lbl_counter = 0;
ir_code_t* global   = (ir_code_t*) 0;
ir_code_t* pos_crnt = (ir_code_t*) 0;

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
}

scope_list_t* genIRCode(enum opcodes opcode, struct scope_list *firstPara, struct scope_list *secondPara, struct scope_list *thirdPara){    
    
}

//add error handle if name already obtained 
scope_list_t* genTemp(int var_type, int value){

    char buffer[16];
    
    sprintf(buffer, "t%d", reg_counter);
    insertSymbol(VAR, var_type, buffer, value, 1);
    reg_counter++;

    return getSymbol(buffer);
}

void printIR()
{
    ir_code_t* entry = global;
    FILE* file       = fopen("ir.log","a");

    
    while(entry != 0)
    {
        fprintf(file,"%d, %s, %s, %s",entry->opcode, entry->firstPara->name,entry->secondPara->name,entry->thirdPara->name);

        entry = entry->next;
    }


    fclose(file);
}


