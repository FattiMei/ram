#ifndef _ARRAY_H_INCLUDED
#define _ARRAY_H_INCLUDED

typedef struct Array{
	unsigned int blksize;
	unsigned int numel;
	void *data;
} Array;


Array ArrayBuild     (void *, unsigned int, unsigned int);
void *ArrayGetElement(Array *, unsigned int);

#endif
