#include <stdio.h>
#include <stdlib.h>
#include "stream.h"


Stream StreamBuild(int *data, unsigned int size){
	return (Stream){
		.size    = size,
		.current = data,
		.data    = data
	};
}


int StreamIsEmpty(Stream *S){
	return S->current >= (S->data + S->size);
}


int StreamPull(Stream *S){
	if(StreamIsEmpty(S))
		return 0;

	return *(S->current++);
}


#ifdef _STREAM_TEST

int main(){
	int array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	Stream input = StreamBuild(array, 10);


	while(!StreamIsEmpty(&input)){
		printf("%d ", StreamPull(&input));
	}

	printf("\n");
	return 0;
}


#endif
