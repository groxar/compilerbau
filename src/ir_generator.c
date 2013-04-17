#include "ir_generator.h";

int reg_counter = 0;
int lbl_counter = 0;

void generateIRCode(enum opcodes op, firstParam, secondParam, thirdParam, jump){
	switch(op){
		case OP_ASSIGN:

			break;
		case OP_ADD:
		case OP_SUB:
		case OP_MUL:
		case OP_DIV:
		case OP_MINUS:
			break;
		case OP_IFEQ:
		case OP_IFNE:
		case OP_IFGT:
		case OP_IFGE:
		case OP_IFLS:
		case OP_IFLE:

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
	//malloc(größe von symbol)
	//name der tmp variable ist ".t"+reg_counter
	//reg_counter++
	//generate_ir_code(opcode, neue tempvariable, firstparam, secondparam)
	// return neue tempvariable und speichere diese im parser in $$
	insertSymbol();
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
