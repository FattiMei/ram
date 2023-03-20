#ifndef _TABLE_H_INCLUDED
#define _TABLE_H_INCLUDED 

typedef struct Table{
	unsigned int blksize;
	unsigned int numel;
	void *data;
} Table;


Table   TableBuild(void *, unsigned int, unsigned int);
void   *TableAccess(Table *, int);
void   *TablePointerAccess(Table *, int);

#endif
