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
    ir_code_t* temp;

	quad->opcode        = opcode;
	quad->firstPara     = firstPara;
	quad->secondPara    = secondPara;
	quad->thirdPara     = thirdPara;
	quad->prev          = pos_crnt;
    quad->next          = (ir_code_t*) 0;
    
   	if(pos_crnt != 0){
        temp = pos_crnt->next;//insertion between quads
        if(temp != 0)
        {
            temp->prev = quad;
            quad->next = temp;
        }

		pos_crnt->next = quad;
	}
    else{
        global = quad;
    }

	pos_crnt = quad;
    printIR();
}

void backPatch(int i){
    for(; 0 < i && pos_crnt->prev != 0;i--)
    {
        pos_crnt = pos_crnt->prev;
    }
}

void frontPatch()
{
    while(pos_crnt->next != 0)
    {
        pos_crnt = pos_crnt->next;
    }
}

ir_code_t* trackLabel(struct scope_list* target){

    ir_code_t* pos = global;

    while(pos != 0)
    {
        if(pos->opcode == LABEL && pos->firstPara == target)
            return pos;
        pos = pos->next;
    }

    return (ir_code_t*) 0;
}

ir_code_t* trackUnsetGoto(){


    printf("/n/n%d",(int)pos_crnt);
    fflush(stdout);
    ir_code_t* pos = pos_crnt;

    while(pos != 0)
    {
        if((pos->opcode == OP_GO || pos->opcode == OP_GOT || pos->opcode == OP_GOF )&& pos->firstPara == 0)
            return pos;
        pos = pos->prev; 
    }
    return (ir_code_t*) 0;
}

void gotoIR(enum opcodes opcode, struct scope_list* label, struct scope_list* term){
    quadList(opcode,label,term,NULL);
}

scope_list_t* arrayLoadIR( struct scope_list *secondPara, struct scope_list *thirdPara){
    
    scope_list_t* temp = genTemp(INT,0);
    quadList(OP_AL, temp, secondPara, thirdPara);
    return temp;
}

scope_list_t* calcIR(enum opcodes opcode, struct scope_list *secondPara, struct scope_list *thirdPara){

    scope_list_t* firstPara = (secondPara->name[0]=='#' && thirdPara->name[0]!='#' )? thirdPara : secondPara; //möglicherweise ändern (assign on temp var)

    quadList(opcode, firstPara, secondPara, thirdPara);
    
    return firstPara;
}

scope_list_t* assignIR(struct scope_list *firstPara, struct scope_list *secondPara){

    quadList(OP_ASS, firstPara, secondPara, NULL);
    return firstPara;
}

scope_list_t* addLabel(char* name){

    scope_list_t* label = getSymbol(name);
    quadList(LABEL,label,NULL,NULL);
    return label;
}

scope_list_t* genLabel(){
    
    char buffer[16];
    scope_list_t* label;

    sprintf(buffer, "#l%d", lbl_counter);
    insertSymbol(FUNC,VOID,buffer,0,0);
    lbl_counter++;

    label = getSymbol(buffer);

    quadList(LABEL,label,NULL,NULL);

    return label;
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
    FILE* file       = fopen("ir.log","a");

    fprintf(file,"\n\n\n");
    while(entry != 0)
    {
        switch(entry->opcode)
        {
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
            case OP_GOT:    fprintf(file,"\tgoto %s if %s != 0", (entry->firstPara?entry->firstPara->name:"?\0"), entry->secondPara->name);break;
            case OP_GOF:    fprintf(file,"\tgoto %s if %s == 0", (entry->firstPara?entry->firstPara->name:"?\0"), entry->secondPara->name);break;
            case OP_RET:    fprintf(file,"\treturn");break;
            case OP_RETN:   fprintf(file,"\treturn %s", entry->firstPara->name);break;
            case OP_CAL:    fprintf(file,"\tcall %s", entry->firstPara->name);break;
            case OP_CALN:   fprintf(file,"\tcallN %s", entry->firstPara->name);break;//NEEDs parameter
            case OP_AL:     fprintf(file,"\t%s = %s[ %s ]", entry->firstPara->name, entry->secondPara->name, entry->thirdPara->name);break;
            case OP_AS:     fprintf(file,"\t%s[ %s] = %s", entry->secondPara->name, entry->thirdPara->name, entry->firstPara->name);break;
            case OP_LNOT:   fprintf(file,"\t%s = !%s", entry->firstPara->name, entry->secondPara->name);break;
            case LABEL:     entry->firstPara->name[0]!='#'?fprintf(file, "%s", entry->firstPara->name):fprintf(file,"\t%s",entry->firstPara->name);
        }
        fprintf(file,"\n");

        entry = entry->next;
    }


    fclose(file);
}


