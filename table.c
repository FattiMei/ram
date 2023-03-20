#include <stdlib.h>
#include <memory.h>
#include "table.h"


Table TableBuild(void *data, unsigned int numel, unsigned int blksize){
	memset(data, 0, numel * blksize);

	return (Table){
		.blksize = blksize,
		.numel   = numel,
		.data    = data
	};
}


void *TableAccess(Table *T, int index){
	if(index < 0 || index >= T->numel)
		return NULL;
	
	return T->data + i * blksize;
};
