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

struct octnode_i
{
	double *dataptr;

	int *indices;
	int num_indices;

	double l, r, t, b, n, f;

	octnode_i *children;
	octnode_i *parent;

	void init(double *ptr, double r, double l, double t, double b, double n, double f);
	void release();

	void split();
	void merge();

	void classify(int index, double *v);

	int *getp(double *v, int *num);
};

#endif
