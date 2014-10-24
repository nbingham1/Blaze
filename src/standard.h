#ifndef standard_h
#define standard_h

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>

char* better_fgets(char *line, int len, FILE *in_file);
char* int_to_char(int Num);
int fsize(FILE *file);

#endif
