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

/**
 * \brief Writes the log into a file and cleans the old log
 */
void initIR()
{
    FILE* file = fopen("ir.log","w");
    fprintf(file,"intermediate code log");
    fclose(file);
}

/**
 * \brief Adds the quadruples to the list
 * \param opcode A unic identifier of the opcode
 * \param firstPara The first operand
 * \param secondPara the second operand
 * \param thirdPara The third operand
 */
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

/**
 * \brief Takes the position of goto-jumps and goes backwords
 * \param i Number of backsteps
 */
void backPatch(int i){
    for(; i > 0 && pos_crnt->prev != 0;i--)
    {
        pos_crnt = pos_crnt->prev;
    }
}
/**
 * \brief Takes the position of goto-jumps and goes forward
 * \param i Number of backsteps
 */
void frontPatch(int i)
{
    for(;i > 0 && pos_crnt->next != 0;i--)
    {
        pos_crnt = pos_crnt->next;
    }
}
/**
 * \brief Tracks the label
 * \param target The target to track
 */
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

/**
 * \brief Tracks unset GoTos
 * \return A pointer of ir_code_t
 */
ir_code_t* trackUnsetGoto(){

    ir_code_t* pos = pos_crnt;

    while(pos != 0)
    {
        if(pos->opcode == OP_GO && pos->firstPara == 0)
            return pos;

        else if((pos->opcode == OP_GOT || pos->opcode == OP_GOF) && ( pos->firstPara == 0 || pos->secondPara == 0))
            return pos;
        pos = pos->prev; 
    }
    return (ir_code_t*) 0;
}

/**
 * \brief Generates a goto
 * \param opcode A unic identifier of the opcode
 * \param label Name of the label where to go
 * \param term The condition of the go
 */
void gotoIR(enum opcodes opcode, struct scope_list* label, struct scope_list* term){
    quadList(opcode,label,term,NULL);
}

/**
 * \brief Generates a function call
 * \param func the name of the function
 */
scope_list_t* callFuncIR(struct scope_list* func){

    if(func->var.func_ptr->n_para)
        quadList(OP_CALN, func, NULL, NULL);
    else
        quadList(OP_CAL, func, NULL, NULL);
   
    return genTemp(func->var_type,1);
}

/**
 * \brief Generates the IR-Code for an array
 * \param secondPara Name of the array
 * \param thirdPara The value of the arrayfield
 * \return Returns a pointer to the temp variable with the IR-code of the array
 */
scope_list_t* arrayLoadIR( struct scope_list *secondPara, struct scope_list *thirdPara){
    
    scope_list_t* temp = genTemp(INT,0);
    quadList(OP_AL, temp, secondPara, thirdPara);
    return temp;
}

/**
 * \brief Generic methode to generate the IR code for all binary operators
 * \param opcode A unic identifier of the opcode
 * \param firstPara The first operand
 * \param secondPara the second operand
 * \return Returns a pointer to the temp variable with the IR-code
 */
scope_list_t* calcIR(enum opcodes opcode, struct scope_list *secondPara, struct scope_list *thirdPara){

    scope_list_t* firstPara = (secondPara->name[0]=='#' && thirdPara->name[0]!='#' )? secondPara : thirdPara; //möglicherweise ändern (assign on temp var)

    quadList(opcode, firstPara, secondPara, thirdPara);
    
    return firstPara;
}

/**
 * \brief Generates the IR-code for assign
 * \param firstPara The variable to assign to
 * \param secondPara The value to assign
 * \return Returns a pointer to the temp variable with the IR-code
 */
scope_list_t* assignIR(struct scope_list *firstPara, struct scope_list *secondPara){

    quadList(OP_ASS, firstPara, secondPara, NULL);
    return firstPara;
}

/**
 * \brief Adds a label to the list
 * \param name The name of the function / label
 * \return Returns a pointer to the label
 */
scope_list_t* addLabel(char* name){
//ToDo: umbenennen zu addLabelIR
    scope_list_t* label = getSymbol(name);
    quadList(LABEL,label,NULL,NULL);
    return label;
}

/**
 * \brief Generates a label
 * \return Returns a pointer to the generated label
 */
scope_list_t* genLabel(){
//ToDo: umbenennen zu genLabelIR()
    char buffer[16];
    scope_list_t* label;
    func_t* function;
    sprintf(buffer, "#l%d", lbl_counter);
    
    //just needed for fail/t06
    function = (func_t*) malloc(sizeof(func_t));
    function->scope = (scope_list_t*) 0;
    function->n_para = -1; 
    //

    insertSymbol(FUNC,VOID,buffer,(int)function,0);
    lbl_counter++;

    label = getSymbol(buffer);

    quadList(LABEL,label,NULL,NULL);

    return label;
}

/**
 * \brief Generates a temp variable
 * \param var_type The name of the function / label
 * \return Returns a pointer to the generated temp variable
 */
//add error handle if name already obtained 
scope_list_t* genTemp(int var_type, int value){

    char buffer[16];
    
    sprintf(buffer, "#%d", reg_counter);
    insertSymbol(VAR, var_type, buffer, value, 1);
    reg_counter++;

    return getSymbol(buffer);
}

/**
 * \brief Prints the IR code to the file ir.log
 */
void printIR()
{
    ir_code_t* entry = global;
    FILE* file       = fopen("ir.log","w");

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
            case OP_GOT:    fprintf(file,"\tgoto %s if %s != 0", (entry->firstPara?entry->firstPara->name:"?\0"), (entry->secondPara?entry->secondPara->name:"?\0"));break;
            case OP_GOF:    fprintf(file,"\tgoto %s if %s == 0", (entry->firstPara?entry->firstPara->name:"?\0"), (entry->secondPara?entry->secondPara->name:"?\0"));break;
            case OP_RET:    fprintf(file,"\treturn");break;
            case OP_RETN:   fprintf(file,"\treturn %s", entry->firstPara->name);break;
            case OP_CAL:    fprintf(file,"\tcall %s", entry->firstPara->name);break;
            case OP_CALN:   fprintf(file,"\tcallN %s %d", entry->firstPara->name, entry->firstPara->var.func_ptr->n_para);break;
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


