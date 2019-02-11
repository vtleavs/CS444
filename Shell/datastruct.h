#ifndef _DATASTRUCT_H_
#define _DATASTRUCT_H_

#include "parse.h"
/*
typedef struct vector_i100 vec_i100;
typedef struct vector_i1024 vec_i1024;
typedef struct vector_s100 vec_s100;
typedef struct vector_s1024 vec_s1024;
*/

typedef struct {
	int vector[100];
	int size;
} vec_i100;

typedef struct {
	int vector[1024];
	int size;
} vec_i1024;

typedef struct {
	char * vector[100];
	int size;
} vec_s100;

typedef struct {
	char * vector[100];
	int size;
} vec_s1024;

int pop_i100(vec_i100 * vector);
int pop_i1024(vec_i1024 * vector);
char * pop_s100(vec_s100 * vector);
char * pop_s1024(vec_s1024 * vector);

int push_i100(vec_i100 * vector, int el);
int push_i1024(vec_i1024 * vector, int el);
int push_s100(vec_s100 * vector, char * el);
int push_s1024(vec_s1024 * vector, char * el);

int contains_i100(vec_i100 * vector, int crit);
int contains_i1024(vec_i1024 * vector, int crit);
int contains_s100(vec_s100 * vector, char * crit);
int contains_s1024(vec_s1024 * vector, char * crit);

int remove_i100(vec_i100 * vector, int index);
int remove_i1024(vec_i1024 * vector, int index);
int remove_s100(vec_s100 * vector, int index);
int remove_s1024(vec_s1024 * vector, int index);

int get_i100(vec_i100 * vector, int index);

#endif /*_DATASTRUCT_H_*/

