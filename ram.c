#include <stdio.h>
#include <stdlib.h>
#include "ram.h"


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
	InstructionPrettyPrint(*(struct Instruction*)M->program->current);

	for(int i = 0; i < NREG; ++i){
		printf("%2d -> %d\n", i, M->registri[i]);
	}

	printf("\n");
}


void Reset(struct RAM *M){
	M->state = OK;

	for(int i = 0; i < NREG; ++i){
		M->registri[i] = 0;
	}
}


void Init(struct RAM *M, Stream *in, Stream *program){
	M->input = in;

	M->program = program;
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
						*Access(M, I.op.data) = *((int *)StreamPull(M->input));
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
			if(where_to_jump >= 0 && where_to_jump < M->program->numel)
				StreamSetCurrent(M->program, where_to_jump);
		}
	}
}


void Run(struct RAM *M){
	while(M->state == OK){
		if(StreamIsEmpty(M->program)){
			M->state = BAD_JUMP;
		} else{
			struct Instruction I = *(struct Instruction*)StreamPull(M->program);
			Execute(M, I);
		}
	}
}
