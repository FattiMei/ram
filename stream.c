#include "stream.h"
#include <stdlib.h>


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
