#include <stdio.h>
#include <stdlib.h>
#include "stream.h"


#define numel(v) (sizeof(v)/sizeof(*v))


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


void InstructionPrettyPrint(struct Instruction I){
	const char *codeliterals[] = {
		 "HALT  " ,"LOAD  " ,"ADD   " ,"SUB   "
		,"MULT  " ,"DIV   " ,"STORE " ,"READ  "
		,"WRITE " ,"JUMP  " ,"JGTZ  " ,"JZERO "
		,"JBLANK"
	};

	printf("%s ", codeliterals[I.code]);

	if (I.op.type == NUMERO){
		printf("=");
	} else if (I.op.type == PUNTATORE){
		printf("*");
	} else{
		printf(" ");
	}

	printf("%d\n", I.op.data);
}


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

	struct Instruction *P;
	int program_size;
	int lc;

	struct Stream *input;
};


const char *stateliteral[] = {
	 [OK]              = "OK"
	,[ERROR]           = "ERROR"
	,[INVALID_OPERAND] = "INVALID_OPERAND"
	,[NEGATIVE_INDEX]  = "NEGATIVE_INDEX"
	,[BAD_READ]        = "BAD_READ"
	,[BAD_JUMP]        = "BAD_JUMP"
};


const int commandtable[] = {
	 [LOAD]   = NUMERO
	,[ADD]    = NUMERO
	,[SUB]    = NUMERO
	,[MULT]   = NUMERO
	,[DIV]    = NUMERO
	,[STORE]  = REGISTRO
	,[READ]   = REGISTRO
	,[WRITE]  = REGISTRO
	,[JUMP]   = ETICHETTA
	,[JGTZ]   = ETICHETTA
	,[JZERO]  = ETICHETTA
	,[JBLANK] = ETICHETTA
	,[HALT]   = ETICHETTA
};


void Dump(struct RAM *M){
	printf("State: %s\n\n", stateliteral[M->state]);
	printf("Current: ");
	InstructionPrettyPrint(M->P[M->lc]);

	for(int i = 0; i < NREG; ++i){
		printf("%2d -> %d\n", i, M->registri[i]);
	}

	printf("\n");
}


void Reset(struct RAM *M){
	M->state = OK;
	M->lc = 0;

	for(int i = 0; i < NREG; ++i){
		M->registri[i] = 0;
	}
}


void Init(struct RAM *M, struct Stream *in, struct Instruction *P, int size){
	M->input = in;

	M->P = P;
	M->program_size = size;
	Reset(M);
}


int* Access(struct RAM *M, int i){
	/* Accedere al registro i-esimo */

	if(i < 0){
		M->state = NEGATIVE_INDEX;
		return NULL;
	}
	else if(i >= NREG){
		M->state = NOT_ENOUGH_REGISTERS;
		return NULL;
	}

	return &(M->registri[i]);
}


struct Operand Dereference(struct RAM *M, struct Operand op){
	struct Operand res = op;

	if(op.type == PUNTATORE){
		int *p = Access(M, op.data);

		if(p != NULL)
			res.data = *p;
	}

	return res;
}


void Execute(struct RAM *M, struct Instruction I){
	/* controlla che l'istruzione accetti il parametro */
	int has_to_jump = 0;
	int where_to_jump = 0;

	if(M->state == OK){
		I.op = Dereference(M, I.op);

		if(M->state == OK){
			int x;

			switch(I.code){
				case HALT:
					M->state = END;
					break;

				case LOAD:
					// questo pattern si ripete per tutti quelle istruzioni
					// che alla fine devono trattare numeri
					if(I.op.type == REGISTRO)
						x = *Access(M, I.op.data);
					else
						x = I.op.data;

					M->registri[0] = x;
					break;

				case STORE:
					*Access(M, I.op.data) = M->registri[0];
					break;

				case ADD:
					if(I.op.type == REGISTRO)
						x = *Access(M, I.op.data);
					else
						x = I.op.data;

					M->registri[0] += x;
					break;

				case SUB:
					if(I.op.type == REGISTRO)
						x = *Access(M, I.op.data);
					else
						x = I.op.data;

					M->registri[0] -= x;
					break;

				case MULT:
					if(I.op.type == REGISTRO)
						x = *Access(M, I.op.data);
					else
						x = I.op.data;

					M->registri[0] *= x;
					break;

				case DIV:
					if(I.op.type == REGISTRO)
						x = *Access(M, I.op.data);
					else
						x = I.op.data;

					M->registri[0] /= x;
					break;

				case READ:
					if(StreamIsEmpty(M->input)){
						M->state = BAD_READ;
					}
					else{
						*Access(M, I.op.data) = StreamPull(M->input);
					}
					break;

				case WRITE:
					printf("%d\n", *Access(M, I.op.data));
					break;

				case JUMP:
					has_to_jump = 1;
					where_to_jump = I.op.data;

					break;

				case JGTZ:
					if(M->registri[0] > 0){
						has_to_jump = 1;
						where_to_jump = I.op.data;
					}

					break;

				case JZERO:
					if(M->registri[0] == 0){
						has_to_jump = 1;
						where_to_jump = I.op.data;
					}

					break;

				case JBLANK:
					if(StreamIsEmpty(M->input)){
						has_to_jump = 1;
						where_to_jump = I.op.data;
					}

					break;
			}
		}

		if(has_to_jump){
			if(where_to_jump >= 0 && where_to_jump < M->program_size)
				M->lc = where_to_jump;
		}
		else if(I.code != HALT){
			(M->lc)++;
		}

	}
}


void Run(struct RAM *M){
	while(M->state == OK){
		Execute(M, *((M->P) + M->lc));
	}
}

/*
	struct Instruction Somma[] = {
		 {LOAD,   {NUMERO,    0}}
		,{JBLANK, {ETICHETTA, 6}}
		,{READ,   {REGISTRO,  1}}
		,{ADD,    {REGISTRO,  1}}
		,{JUMP,   {ETICHETTA, 2}}
		,{WRITE,  {REGISTRO,  0}}
		,{HALT,   {NESSUNO,   0}}
	};
*/


int main(){
	int V[] = {1, 2, 3, 4, 5};
	struct Stream *S = StreamNew(V, numel(V));

	struct Instruction Programma[] = {
		/* 0 */  {LOAD,   {NUMERO,    0}}
		/* 1 */ ,{JBLANK, {ETICHETTA, 5}}
		/* 2 */ ,{READ,   {REGISTRO,  1}}
		/* 3 */ ,{ADD,    {REGISTRO,  1}}
		/* 4 */ ,{JUMP,   {ETICHETTA, 1}}
		/* 5 */ ,{WRITE,  {REGISTRO,  0}}
		/* 6 */ ,{HALT,   {NESSUNO,   0}}
	};

	struct RAM M;

	Init(&M, S, Programma, numel(Programma));
	Run(&M);
	return 0;
}
