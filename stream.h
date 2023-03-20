#ifndef _STREAM_H_INCLUDED
#define _STREAM_H_INCLUDED

typedef struct Stream{
	unsigned int blksize;
	unsigned int numel;
	void *current;
	void *data;
} Stream;


Stream  StreamBuild  (void *, unsigned int, unsigned int);
int     StreamIsEmpty(Stream *);
void   *StreamPull   (Stream *);
int     StreamSetCurrent(Stream *, unsigned int);

#endif
