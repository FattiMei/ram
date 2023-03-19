#include "stream.h"
#include <stdlib.h>


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
