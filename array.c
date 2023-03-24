#include <stdlib.h>
#include "array.h"


Array ArrayBuild(void *data, unsigned int numel, unsigned int blksize){
	return (Array){
		.blksize = blksize,
		.numel   = numel,
		.data    = data
	};
}


void *ArrayGetElement(Array *A, unsigned int index){
	if(index >= A->blksize * A->numel){
		return NULL;
	}

	return A->data + A->blksize * index;
}
