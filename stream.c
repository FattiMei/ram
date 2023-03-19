#include "stream.h"
#include <stdlib.h>


struct Stream{
	unsigned int size;
	int *current;
	int *data;
};


struct Stream *StreamNew(int *data, unsigned int size){
	struct Stream *S = malloc(sizeof(struct Stream));

	if(S != NULL){
		S->data    = data;
		S->current = data;
		S->size    = size;
	}

	return S;
}


int StreamIsEmpty(struct Stream *S){
	return S->current >= (S->data + S->size);
}


int StreamPull(struct Stream *S){
	if(StreamIsEmpty(S))
		return 0;

	return *(S->current++);
}
