#include "symboltable.h";
#include "ir_generator.h";

int reg_counter = 0;
int lbl_counter = 0;
ir_code_t* pos_crnt = (ir_code_t*) 0;


void quadList(enum opcodes opcode, struct scope_list *firstParam, struct scope_list *secondParam, struct scope_list *thirdParam){
	ir_code_t* quad = (ir_code_t*) malloc(sizeof(ir_code_t));
	quad->opcode = opcode;
	quad->firstParam = firstParam;
	quad->secondParam = secondParam;
	quad->thirdParam = thirdParam;


	quad->prev = pos_crnt;
	if(pos_crnt != 0){
		pos_crnt->next = quad;
	}
	pos_crnt = quad;
}

//arithmetische Ausdrücke handhaben
scope_list_t* doEval(enum opcodes opcode, scope_list_t *firstParam, scope_list_t *secondParam){
	//hier wird eine neue temp variable erstellt
	scope_list_t *result = (scope_list_t*) malloc(sizeof(scope_list_t));
	//malloc(größe von symbol)
	//name der tmp variable ist ".t"+reg_counter;
	result->name = (char*) ".t"+reg_counter;
	reg_counter++;


	switch(opcode){
		case OP_PLUS:

			break;
		case OP_MINUS:
		case OP_MUL:
		case OP_DIV:
		case OP_UNARY_MINUS:
		case OP_UNARY_PLUS:
			break;
	}
	// return neue tempvariable und speichere diese im parser in $$
	return result;
}

scope_list_t* generateIRCode(enum opcodes opcode, struct scope_list *firstParam, struct scope_list *secondParam, struct scope_list *thirdParam){
	scope_list_t *result = (scope_list_t*) malloc(sizeof(scope_list_t));

	switch(opcode){
		case OP_ASSIGN:

			break;
		case OP_PLUS:
		case OP_MINUS:
		case OP_MUL:
		case OP_DIV:
		case OP_UNARY_MINUS:
		case OP_UNARY_PLUS:

			result = doEval(opcode, firstParam, secondParam);
			break;
		case OP_EQ:
		case OP_NE:
		case OP_GT:
		case OP_GTEQ:
		case OP_LS:
		case OP_LSEQ:

			break;
/*
		case OP_GOTO:

			break;
		case OP_RETURN:

			break;
		case OP_RETURN_OPNULL:

			break;
		case OP_CALL_ONE:

			break;
		case OP_CALL_TWO:

			break;
		case OP_ARRAY_LD:

			break;
		case OP_ARRAY_ST:

			break;*/

	}
	return result;
}

void doAssign(firstParam, secondParam){

	//generateIRCode("OPASSIGN", firstParam, secondParam, 0);

}





void doGoto(){

}


void doReturnParam(){

}

void doReturn(){

}

void doCallAssign(){

}

void doCall(){

}

void doArrayLd(){

}

void doArraySt(){

}
