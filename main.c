#include <stdio.h>
#include <stdlib.h>
#include "stream.h"
#include "ram.h"


#define numel(v) (sizeof(v)/sizeof(*v))


int main(){
	int V[] = {1, 2, 3, 4, 5};
	struct Instruction Programma[] = {
		/* 0 */  {LOAD,   {NUMERO,    0}}
		/* 1 */ ,{JBLANK, {ETICHETTA, 5}}
		/* 2 */ ,{READ,   {REGISTRO,  1}}
		/* 3 */ ,{ADD,    {REGISTRO,  1}}
		/* 4 */ ,{JUMP,   {ETICHETTA, 1}}
		/* 5 */ ,{WRITE,  {REGISTRO,  0}}
		/* 6 */ ,{HALT,   {NESSUNO,   0}}
	};


	Stream S = StreamBuild(V, numel(V), sizeof(*V));
	Stream P = StreamBuild(Programma, numel(Programma), sizeof(struct Instruction));

	Ram M = RamBuild();
	RamRun(&M, &S, &P);
	return 0;
}
