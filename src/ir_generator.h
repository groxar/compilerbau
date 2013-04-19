typedef struct ir_code ir_code_t;

enum opcodes {
	OP_ASSIGN,
	OP_PLUS,
	OP_MINUS,
	OP_MUL,
	OP_DIV,
	OP_MOD,
	OP_UNARY_MINUS,
	OP_UNARY_PLUS,
	OP_EQ,
	OP_NE,
	OP_GT,
	OP_GTEQ,
	OP_LS,
	OP_LSEQ,
	OP_GOTO,
	OP_RETURN,
	OP_RETURN_OPNULL,
	OP_CALL_ONE,
	OP_CALL_TWO,
	OP_ARRAY_LD,
	OP_ARRAY_ST,
	OP_LOGICAL_OR,
	OP_LOGICAL_AND,
	OP_LOGICAL_NOT
};

scope_list_t* generateIRCode(enum opcodes opcode, struct scope_list *firstParam, struct scope_list *secondParam, struct scope_list *thirdParam);

struct ir_code{
    enum opcodes opcode;
    scope_list_t* firstParam;
    scope_list_t* secondParam;
    scope_list_t* thirdParam;

    ir_code_t* prev;
    ir_code_t* next;
};
