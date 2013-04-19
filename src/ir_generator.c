#include "symboltable.h";
#include "ir_generator.h";

int reg_counter = 0;
int lbl_counter = 0;

void generateIRCode(enum opcodes op, struct scope_list *firstParam, struct scope_list *secondParam, struct scope_list *thirdParam){
	switch(op){
		case OP_ASSIGN:

			break;
		case OP_PLUS:
		case OP_MINUS:
		case OP_MUL:
		case OP_DIV:
		case OP_UNARY_MINUS:
		case OP_UNARY_PLUS:

			doEval(op, firstParam, secondParam);
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
}

void doAssign(firstParam, secondParam){

	//generateIRCode("OPASSIGN", firstParam, secondParam, 0);

}


//arithmetische Ausdrücke handhaben
void doEval(enum opcodes opcode, scope_list_t firstParam, scope_list_t secondParam){
	//hier wird eine neue temp variable erstellt
	scope_list_t *result;
	//malloc(größe von symbol)
	//result = (scope_list_t) malloc(scope_list_t);
	//name der tmp variable ist ".t"+reg_counter;
	result->name = (char*) ".t"+reg_counter;
	reg_counter++;
	//generate_ir_code(opcode, neue tempvariable, firstparam, secondparam)
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
