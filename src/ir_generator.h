#pragma once
typedef struct ir_code ir_code_t;

enum opcodes {
	OP_ASS,
	OP_ADD,
	OP_SUB,
	OP_MUL,
	OP_DIV,
    OP_NEG,
	OP_MOD,
	OP_EQ,
	OP_NE,
	OP_GT,
	OP_GE,
	OP_LT,
	OP_LE,
	OP_GO,
	OP_RET,
	OP_RETN,
	OP_CAL,
	OP_CALN,
	OP_AL,
	OP_AS,
	OP_LOR,
	OP_LAND,
	OP_LNOT
};

struct ir_code{
    enum opcodes opcode;
    scope_list_t* firstPara;
    scope_list_t* secondPara;
    scope_list_t* thirdPara;

    ir_code_t* prev;
    ir_code_t* next;
};

void initIR();
scope_list_t* genIRCode(enum opcodes opcode, scope_list_t* firstParam, scope_list_t* secondParam, scope_list_t* thirdParam);


