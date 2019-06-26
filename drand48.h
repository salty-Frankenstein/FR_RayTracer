#pragma once
#ifndef DRAND48_H  
#define DRAND48_H  

#include <stdlib.h>  

#define M 0x100000000LL  
#define C 0xB16  
#define A 0x5DEECE66DLL  

static unsigned long long seed = 1;

double drand48(void)
{
	seed = (A * seed + C) & 0xFFFFFFFFFFFFLL;
	unsigned int x = seed >> 16;
	return  ((double)x / (double)M);

}

void srand48(unsigned int i)
{
	seed = (((long long int)i) << 16) | rand();
}

#endif 