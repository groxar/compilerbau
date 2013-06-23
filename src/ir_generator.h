#pragma once
typedef struct ir_code ir_code_t;

enum opcodes {
    OP_ASSC,
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
    OP_GOT,//Goto on != 0
    OP_GOF,//Goto on == 0
	OP_RET,
	OP_RETN,
	OP_CAL,
	OP_CALN,
	OP_AL,
	OP_AS,
	OP_LOR,
	OP_LAND,
	OP_LNOT,
    OP_SL,
    OP_SR,
    LABEL
};

struct ir_code{
    enum opcodes opcode;
    scope_list_t* firstPara;
    scope_list_t* secondPara;
    scope_list_t* thirdPara;

    ir_code_t* prev;
    ir_code_t* next;
};

void printIR(char const * const _file_name);
void gotoIR(enum opcodes opcode, struct scope_list* label, struct scope_list* term);
void backPatch(int i);
void frontPatch(int i);

scope_list_t* callFuncIR(struct scope_list* func);
scope_list_t* arrayLoadIR(struct scope_list *secondPara, struct scope_list *thirdPara);
scope_list_t* calcIR(enum opcodes opcode, struct scope_list* secondPara, struct scope_list* thirdPara);
scope_list_t* assignIR(struct scope_list *firstPara, struct scope_list *secondPara);
void genRetIR(scope_list_t* sym);
scope_list_t* genConst(int var_type, int value);
scope_list_t* genTemp(int var_type, int value);
scope_list_t* addLabel(char* name);
scope_list_t* genLabel();

ir_code_t* trackLabel(struct scope_list* label);
ir_code_t* trackUnsetGoto();


