/*
 * data.h defines functions that manipulate gpat.data and handle
 * data input/output.
 */

#ifndef DATA_HEADER
#define DATA_HEADER

int  print(FILE *);
int  add(char *, char *, char *, FILE *);
int  delete(char *);
int  clearall(void);
int whatif(char *, char *, char *, FILE *);

#endif
