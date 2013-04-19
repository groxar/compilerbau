#include "ir_generator.h";
#include "symboltable.h";

int reg_counter = 0;
int lbl_counter = 0;

void generateIRCode(enum opcodes op, scope_list_t firstParam, scope_list_t secondParam, scope_list_t thirdParam, jump){
	switch(op){
		case ASSIGN:

			break;
		case PLUS:
		case MINUS:
		case MUL:
		case DIV:
		case UNARY_MINUS:
		case UNARY_PLUS:
			break;
		case EQ:
		case NE:
		case GT:
		case GTEQ:
		case LS:
		case LSEQ:

			break;
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

			break;

	}
}

void doAssign(firstParam, secondParam){

	generateIRCode("ASSIGN", firstParam, secondParam, 0);

}

void doEval(opcode, firstParam, secondParam){
	//hier wird eine neue temp variable erstellt
	scope_list_t *result;
	//malloc(größe von symbol)
	result = (scope_list_t) malloc(scope_list_t);
	//name der tmp variable ist ".t"+reg_counter;
	result->name = (char*) ".t"+reg_counter;
	reg_counter++;
	//generate_ir_code(opcode, neue tempvariable, firstparam, secondparam)

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
