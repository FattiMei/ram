#include "stream.h"
#include <stdlib.h>


Stream *StreamNew(int *data, unsigned int size){
	Stream *S = malloc(sizeof(Stream));

	if(S != NULL){
		S->data    = data;
		S->current = data;
		S->size    = size;
	}

	return S;
}


int StreamIsEmpty(Stream *S){
	return S->current >= (S->data + S->size);
}


int StreamPull(Stream *S){
	if(StreamIsEmpty(S))
		return 0;

	return *(S->current++);
}
