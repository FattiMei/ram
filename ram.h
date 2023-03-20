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


typedef struct Ram{
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
} Ram;


void Dump(Ram *M);
void Reset(Ram *M);
void Init(Ram *M, Stream *in, Stream *program);
void Execute(Ram *M, struct Instruction I);
void Run(Ram *M);
