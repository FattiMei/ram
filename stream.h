typedef struct Stream{
	unsigned int size;
	int *current;
	int *data;
} Stream;


Stream  StreamBuild  (int *, unsigned int);
int     StreamIsEmpty(Stream *);
int     StreamPull   (Stream *);
