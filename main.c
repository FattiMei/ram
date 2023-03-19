#include <stdio.h>
#include <stdlib.h>
#include "stream.h"


#define numel(v) (sizeof(v)/sizeof(*v))


#define NREG 8


struct Operand{
	enum{
		NESSUNO
	   ,NUMERO
	   ,REGISTRO
	   ,PUNTATORE
	   ,ETICHETTA
	} type;

	int data;

};


struct Instruction{
	enum{
		 LOAD
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
		,HALT
	} code;

	struct Operand op;
};


struct RAM{
	enum{
		 OK
		,ERROR
		,INVALID_OPERAND
		,NEGATIVE_INDEX
		,NOT_ENOUGH_REGISTERS
		,BAD_READ
		,BAD_JUMP
	} state;

	int registri[NREG];

	struct Istruzione *P;
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


void ClearErrors(struct RAM *M){
	M->state = OK;
}


void Dump(struct RAM *M){
	printf("State: %s\n\n", stateliteral[M->state]);

	for(int i = 0; i < NREG; ++i){
		printf("%2d -> %d\n", i, M->registri[i]);
	}

	printf("\n");
}


void Reset(struct RAM *M){
	ClearErrors(M);

	M->lc = 0;

	for(int i = 0; i < NREG; ++i){
		M->registri[i] = 0;
	}
}


void Init(struct RAM *M, struct Stream *in){
	M->input = in;
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

	if(M->state == OK){
		I.op = Dereference(M, I.op);

		if(M->state == OK){
			int x;

			switch(I.code){
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
			}
		}
	}
}

/*
	struct Instruction Somma[] = {
		 {LOAD,   {NUMERO,    0}}
		,{JBLANK, {ETICHETTA, 7}}
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
		 {READ,   {REGISTRO,  1}}
		,{ADD,    {REGISTRO,  1}}
		,{READ,   {REGISTRO,  1}}
		,{ADD,    {REGISTRO,  1}}
		,{READ,   {REGISTRO,  1}}
		,{ADD,    {REGISTRO,  1}}
		,{READ,   {REGISTRO,  1}}
		,{ADD,    {REGISTRO,  1}}
		,{READ,   {REGISTRO,  1}}
		,{ADD,    {REGISTRO,  1}}
		,{WRITE,  {REGISTRO,  0}}
	};

	struct RAM M;

	Init(&M, S);

	for(int i = 0; i < numel(Programma); ++i){
		Execute(&M, Programma[i]);
	}


	return 0;
}
