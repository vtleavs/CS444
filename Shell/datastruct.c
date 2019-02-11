#include <stdio.h>
#include "datastruct.h"


void init_i100(vec_i100 * vector)
{
	int vec[100];
	vec_i100 vect;
	int i;
	
	for(i = 0; i < 100; i++)
	{
		vec[i] = 0;	
	}

	vect.vector[0] = 0;;
	vect.size = 0;
	
	vector = vect;
}

int pop_i100(vec_i100 * vector) 
{
	int res;
	
	if(vector->size == 0)
		return -1;
	
	res = vector->vector[vector->size - 1];
	remove_i100(vector, vector->size -1);

	return res;
}

int pop_i1024(vec_i1024 * vector) {}
char * pop_s100(vec_s100 * vector) {}
char * pop_s1024(vec_s1024 * vector) {}

int push_i100(vec_i100 * vector, int el) 
{
	printf("%i\n", vector->size);
	/*
	if(vector->size == 100)
		return -1;
	
	vector->vector[vector->size] = el;
	vector->size++;
	return 0;
	*/
}
int push_i1024(vec_i1024 * vector, int el) {}
int push_s100(vec_s100 * vector, char * el) {}
int push_s1024(vec_s1024 * vector, char * el) {}

int contains_i100(vec_i100 * vector, int crit) 
{
	int i;
	for(i = 0; i < vector->size; i++)
	{
		if(vector->vector[i] == crit)
			return 1;
	}
	return 0;
}
int contains_i1024(vec_i1024 * vector, int crit) {}
int contains_s100(vec_s100 * vector, char * crit) {}
int contains_s1024(vec_s1024 * vector, char * crit) {}

int remove_i100(vec_i100 * vector, int index) 
{
	if(index > vector->size || index < 0)
		return -1;
	
	vector->vector[index] = 0;
	vector->size--;

	if(index < vector->size)
	{
		int i;
		for(i = index; i < vector->size; i++)
		{
			vector->vector[i] = vector->vector[i+1];
		}
	}

	return 0;
}
int remove_i1024(vec_i1024 * vector, int index) {}
int remove_s100(vec_s100 * vector, int index) {}
int remove_s1024(vec_s1024 * vector, int index) {}

int get_i100(vec_i100 * vector, int index) 
{
	if(index > vector->size || index < 0)
		return -1;
	
	return vector->vector[index];
}
