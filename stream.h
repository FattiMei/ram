struct Stream;

struct Stream* StreamNew    (int *, unsigned int);
int            StreamIsEmpty(struct Stream *);
int            StreamPull   (struct Stream *);
