#include "prime.h"

typedef struct {
	char* data;
	long start;
	long len;
} thread_data;

void* thread(void* tdp){
	long a;
	long b;
	long offset;

	long len = ((thread_data*)tdp)->len;
	char* prime = ((thread_data*)tdp)->data;

	// set 1 to be not a prime number

	// a represents the primenumber 2a+3
	for(a=((thread_data*)tdp)->start ; a < len ; a+=2){
		if(prime[a] == 0){
			// b represents the primenumber 2b+3

			// (2a+3)*(2a+3) = 4aa+12a+9 = 2b+3
			// => 2aa+6a+3 = b
			// b ist nun der start index

			// 2b+3+2(2a+3) = 2c+3
			// 2b+2(2a+3) = 2c
			// b+2a+3 = c
			// dann weise b das c zu.
			offset = 2*a+3;
			for(b=2*a*a+6*a+3; b<len ; b += offset){
				prime[b] = 1;
			}
		}
	}

	return NULL;
}

long sieb6(long n){
	#ifndef TREADED
		return 0;
	#else
		int i;
		long size = 1;

		pthread_t p1, p2;
		thread_data td1, td2;

		// represents 2k+3, uint k
		int len = (n-1)/2;
		char* prime = calloc(len,sizeof(char));
		td1.data = prime;
		td1.len = len;
		td1.start = 0;
		td2.data = prime;
		td2.len = len;
		td2.start = 1;
		pthread_create (&p1, NULL, thread, (void*)&td1);
		pthread_create (&p2, NULL, thread, (void*)&td2);

		pthread_join (p1, NULL);
		pthread_join (p2, NULL);

		for(i=0 ; i<len ; ++i){
			if(prime[i] == 0){
				++size;
			}
		}

		free(prime);

		return size;
	#endif
}
