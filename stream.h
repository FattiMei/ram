typedef struct Stream{
	unsigned int size;
	int *current;
	int *data;
} Stream;


Stream* StreamNew    (int *, unsigned int);
int     StreamIsEmpty(Stream *);
int     StreamPull   (Stream *);
