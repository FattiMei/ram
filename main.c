#include <stdio.h>
#include <stdlib.h>
#include "stream.h"
#include "ram.h"


#define numel(v) (sizeof(v)/sizeof(*v))


int main(){
	int V[] = {1, 2, 3, 4, 5};
	Stream S = StreamBuild(V, numel(V));

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

	Init(&M, &S, Programma, numel(Programma));
	Run(&M);
	return 0;
}
