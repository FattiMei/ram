#include <stdio.h>
#include <stdlib.h>
#include "array.h"


Array ArrayBuild(void *data, unsigned int numel, unsigned int blksize){
	return (Array){
		.blksize = blksize,
		.numel   = numel,
		.data    = data
	};
}


void *ArrayGet(Array *A, unsigned int index){
	if(index >= A->blksize * A->numel){
		return NULL;
	}

	return A->data + A->blksize * index;
}


#ifdef _ARRAY_TEST

int main(){
	int x[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

	Array V = ArrayBuild(x, 10, sizeof(int));

	for(unsigned int i = 0; i < V.numel; ++i){
		printf("%d ", *(int *)ArrayGet(&V, i));
	}
	printf("\n");


	return 0;
}


#endif
