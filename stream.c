#include <stdio.h>
#include <stdlib.h>
#include "stream.h"


Stream StreamBuild(void *data, unsigned int numel, unsigned int blksize){
	return (Stream){
		.blksize = blksize,
		.numel   = numel,
		.current = data,
		.data    = data
	};
}


int StreamIsEmpty(Stream *S){
	return S->current >= (S->data + S->numel * S->blksize);
}


void *StreamPull(Stream *S){
	if(StreamIsEmpty(S))
		return NULL;

	return S->current++;
}


#ifdef _STREAM_TEST

int main(){
	int array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	Stream input = StreamBuild(array, 10, sizeof(int));


	while(!StreamIsEmpty(&input)){
		int x = *(int *)StreamPull(&input);
		printf("%d ", x);
	}

	printf("\n");
	return 0;
}


#endif
