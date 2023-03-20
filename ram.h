#include "stream.h"

#define NREG 8


struct Operand{
	enum{
		NESSUNO = 0
	   ,NUMERO
	   ,REGISTRO
	   ,PUNTATORE
	   ,ETICHETTA
	} type;

	int data;

};


struct Instruction{
	enum{
		 HALT = 0
		,LOAD
		,ADD
		,SUB
		,MULT
		,DIV
		,STORE
		,READ
		,WRITE
		,JUMP
		,JGTZ
		,JZERO
		,JBLANK
	} code;

	struct Operand op;
};


void InstructionPrettyPrint(struct Instruction I);


struct RAM{
	enum{
		 OK
		,END
		,ERROR
		,INVALID_OPERAND
		,NEGATIVE_INDEX
		,NOT_ENOUGH_REGISTERS
		,BAD_READ
		,BAD_JUMP
	} state;

	int registri[NREG];

	Stream *program;
	Stream *input;
};

void Dump(struct RAM *M);
void Reset(struct RAM *M);
void Init(struct RAM *M, Stream *in, Stream *program);
void Execute(struct RAM *M, struct Instruction I);
void Run(struct RAM *M);
