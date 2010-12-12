/*
 ============================================================================
 Name        : Sieb.c
 Author      : Konstantin Weitz
 Version     :
 Copyright   : 
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

//#define TREADED

void printbin(long size, char* val) {
	char buf[9];
	buf[8] = '\0';
	long i;
	long b;

	for(i=0;i<size;++i){
		for(b=0;b<8;++b){
			buf[b] = '0'+((*(val+i) & 1 << (7-b)) != 0);
		}
		printf("%s",buf);
		if(i<size-1) printf("|");
	}
}

long sieb0(long n){
	long a;
	long b;
	long size = 1;

	long* prime = calloc(n,sizeof(long));

	prime[0] = 1;
	prime[1] = 1;

	for(a=3 ; a<n ; a+=2){
		if(prime[a] == 0){
			for(b=a+a; b<n ; b+=a){
				prime[b] = 1;
			}
			++size;
		}
	}

	free(prime);

	return size;
}

long sieb1(long n){
	long a;
	long b;
	long size = 1;

	long* prime = calloc(n,sizeof(long));

	prime[0] = 1;
	prime[1] = 1;

	for(a=3 ; a<n ; a+=2){
		if(prime[a] == 0){
			for(b=a*a; b<n ; b+=a){
				prime[b] = 1;
			}
			++size;
		}
	}

	free(prime);

	return size;
}

long sieb2(long n){
	long a;
	long b;
	long offset;
	long size = 1;

	// represents 2k+3, uint k
	int len = (n-1)/2;
	long* prime = calloc(len,sizeof(long));

	// set 1 to be not a prime number

	// a represents the primenumber 2a+3
	for(a=0 ; a<len ; ++a){
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
			++size;
		}
	}

	free(prime);

	return size;
}

long sieb3(long n){
	long a;
	long b;
	long offset;
	long size = 1;

	// represents 2k+3, uint k
	int len = (n-1)/2;
	char* prime = calloc(len,sizeof(char));

	// set 1 to be not a prime number

	// a represents the primenumber 2a+3
	for(a=0 ; a<len ; ++a){
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
			++size;
		}
	}

	free(prime);

	return size;
}

long sieb4(long n){
	long ax;
	long ay;
	long b;
	long offset;
	long size = 1;
	typedef char vecType;
	#define bits (8*sizeof(vecType))

	// k is the number of bits per unit
	// the primary number p is at the index k
	// p = 2k+3
	// the index k is at byte x and bit y
	// bx+y = k
	// p = 2bx+2y+3

	// to calc the size of array we need to make sure
	// that the number n (passed to function) is in the array
	// n = 2bx+2y+3
	// (n-3)/2 = bx+y
	// (n-3)/2 = bx+y

	// the max number passed is desribed by
	// l is the amount of bytes needed
	// n = 2kl+1
	// s is the anount of bits
	// n = 2k+1

	// since y cant be greater b adding one will cover all cases
	// add another one cause the array size starts at 1
	long len = (n-1)/(2*bits);
	long lenBits = (n-1)/2;	// 129
	vecType* prime = calloc(len,sizeof(vecType));

	// set 1 to be not a prime number

	// ax and ay represent the number 2*BITS*ax+2*ay+3
	for(ax=0 ; ax<len ; ++ax){
		for(ay=0 ; ay<bits ; ++ay){
			if((prime[ax] & (vecType)1<<ay) == 0){
				// b represents the number 2b+3
				// k is BITS
				// ax is x, ay is y

				// 2kx+2y+3
				// (2kx+2y+3)*(2kx+2y+3)
				// = 4kxkx + 4kxy + 6kx +
				//   4kxy  + 4yy  + 6y  +
				//   6kx   + 6y   + 9

				// = 4kxkx + 8kxy + 12kx +
				//         + 4yy  + 12y  +
				//                + 9

				// = (4kxkx + 8kxy + 12kx + 4yy + 12y + 6) / 2 = b
				// = 2kkxx + 4kxy + 6kx + 2yy + 6y + 3 = b

				// b ist nun der start index

				// 2b+3+2(2kx+2y+3) = 2c+3
				// 2b+2(2kx+2y+3) = 2c
				// b+2kx+2y+3 = c
				// dann weise b das c zu.
				offset = 2*bits*ax+2*ay+3;

				for(b=2*bits*bits*ax*ax+6*bits*ax+3+4*bits*ax*ay+6*ay+2*ay*ay; b<lenBits ; b+=offset){
					prime[b/bits] |= (vecType)1<<b;
				}
				++size;
			}
		}
	}


	free(prime);

	return size;

	#undef bits
}

long sieb5(long n){
	long ax;
	long ay;
	long b;
	long offset;
	long sum;
	long fac;
	long size = 1;
	typedef long vecType;
	#define bits (8*sizeof(vecType))

	// #define STACK_ALLOC

	// k is the number of bits per unit
	// the primary number p is at the index k
	// p = 2k+3
	// the index k is at byte x and bit y
	// bx+y = k
	// p = 2bx+2y+3

	// to calc the size of array we need to make sure
	// that the number n (passed to function) is in the array
	// n = 2bx+2y+3
	// (n-3)/2 = bx+y
	// (n-3)/2 = bx+y

	// the max number passed is desribed by
	// l is the amount of bytes needed
	// n = 2kl+1
	// s is the anount of bits
	// n = 2k+1

	// since y cant be greater b adding one will cover all cases
	// add another one cause the array size starts at 1
	long len = (n-1)/(2*bits);
	long lenBits = (n-1)/2;	// 129

	#ifndef STACK_ALLOC
		vecType* prime = calloc(len,sizeof(vecType));
	#else
		vecType* prime = alloca(len*sizeof(vecType));
		int i;
		for(i=0 ; i<len ; ++i){
			prime[i] = 0;
		}
	#endif

	// set 1 to be not a prime number

	// ax and ay represent the number 2*BITS*ax+2*ay+3
	for(ax=0 ; ax<len ; ++ax){
		// 2kkxx + 6kx + 3 = b
		sum = 2*bits*bits*ax*ax + 6*bits*ax + 3;
		// 6y + 4kxy ++ 2yy
		fac = 4*bits*ax;

		for(ay=0 ; ay<bits ; ++ay){
			if((prime[ax] & (vecType)1<<ay) == 0){
				// b represents the number 2b+3
				// k is BITS
				// ax is x, ay is y

				// 2kx+2y+3
				// (2kx+2y+3)*(2kx+2y+3)
				// = 4kxkx + 4kxy + 6kx +
				//   4kxy  + 4yy  + 6y  +
				//   6kx   + 6y   + 9

				// = 4kxkx + 8kxy + 12kx +
				//         + 4yy  + 12y  +
				//                + 9

				// = (4kxkx + 8kxy + 12kx + 4yy + 12y + 6) / 2 = b
				// = 2kkxx + 4kxy + 6kx + 2yy + 6y + 3 = b

				// b ist nun der start index

				// 2b+3+2(2kx+2y+3) = 2c+3
				// 2b+2(2kx+2y+3) = 2c
				// b+2kx+2y+3 = c
				// dann weise b das c zu.
				offset = 2*bits*ax+2*ay+3;

				// b=sum+fac*ay+6*ay+2*ay*ay
				// b=sum+ay*(fac+6+2*ay)

				for(b=sum+ay*(fac+6+2*ay) ; b<lenBits ; b+=offset){
					prime[b/bits] |= (vecType)1<<b;
				}
				++size;
			}
		}
	}

	#ifndef STACK_ALLOC
		free(prime);
	#endif

	return size;

	#undef bits
	#undef stack_alloc
}

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

inline long int_sqrt(long n){
	long a;
    for (a=0;n>=(2*a)+1;n-=(2*a++)+1);
    return a;
}

long sieb7(long n){
	long ax;
	long ay;
	long b;
	long offset;
	long sum;
	long fac;
	long size = 1;
	typedef long vecType;
	#define bits (8*sizeof(vecType))

	#define STACK_ALLOC

	// b is the number of bits per unit
	// the primary number p is at the index k
	// p = 2k+3
	// the index k is at byte x and bit y
	// bx+y = k
	// p = 2bx+2y+3

	// to calc the size of array we need to make sure
	// that the number n (passed to function) is in the array
	// n = 2bx+2y+3
	// (n-3)/2 = bx+y
	// (n-3)/2 = bx+y

	// the max number passed is desribed by
	// l is the amount of bytes needed
	// n = 2kl+1
	// s is the anount of bits
	// n = 2k+1

	// since y cant be greater b adding one will cover all cases
	// add another one cause the array size starts at 1
	long len = (n-1)/(2*bits);
	long lenBits = (n-1)/2;
	long sqrt = int_sqrt(len)+1;

	#ifndef STACK_ALLOC
		vecType* prime = calloc(len,sizeof(vecType));
	#else
		vecType* prime = alloca(len*sizeof(vecType));
		int i;
		for(i=0 ; i<len ; ++i){
			prime[i] = 0;
		}
	#endif

	// set 1 to be not a prime number

	// ax and ay represent the number 2*BITS*ax+2*ay+3
	for(ax=0 ; ax<=sqrt ; ++ax){
		// 2kkxx + 6kx + 3 = b
		sum = ax*(2*bits*bits*ax+6*bits)+3;
		// 6y + 4kxy ++ 2yy
		fac = 4*bits*ax;

		for(ay=0 ; ay<bits ; ++ay){
			if((prime[ax] & (vecType)1<<ay) == 0){
				// b represents the number 2b+3
				// k is BITS
				// ax is x, ay is y

				// 2kx+2y+3
				// (2kx+2y+3)*(2kx+2y+3)
				// = 4kxkx + 4kxy + 6kx +
				//   4kxy  + 4yy  + 6y  +
				//   6kx   + 6y   + 9

				// = 4kxkx + 8kxy + 12kx +
				//         + 4yy  + 12y  +
				//                + 9

				// = (4kxkx + 8kxy + 12kx + 4yy + 12y + 6) / 2 = b
				// = 2kkxx + 4kxy + 6kx + 2yy + 6y + 3 = b

				// b ist nun der start index

				// 2b+3+2(2kx+2y+3) = 2c+3
				// 2b+2(2kx+2y+3) = 2c
				// b+2kx+2y+3 = c
				// dann weise b das c zu.

				offset = 2*bits*ax+2*ay+3;

				// b=sum+fac*ay+6*ay+2*ay*ay
				// b=sum+ay*(fac+6+2*ay)

				for(b=sum+ay*(fac+6+2*ay) ; b<lenBits ; b+=offset){
					prime[b/bits] |= (vecType)1<<b;
				}

				++size;
			}
		}
	}

	for( ; ax<len ; ++ax){
		for(ay=0 ; ay<bits ; ++ay){
			if((prime[ax] & (vecType)1<<ay) == 0){
				++size;
			}
		}
	}

	#ifndef STACK_ALLOC
		free(prime);
	#endif

	return size;

	#undef bits
	#undef stack_alloc
}

typedef long (*siebn)(long n);

typedef long vecType;
#define BITS (8*sizeof(vecType))
#define SIZE 100000001
#define LEN ((SIZE-1)/(2*BITS))
#define LEN_BITS ((SIZE-1)/2)
#define SQRT 884
vecType prime[LEN];

inline long compiled_sieb(){
	long ax;
	long ay;
	long b;
	long offset;
	long sum;
	long fac;
	long size = 1;

/*	int i;
	for(i=0 ; i<LEN ; ++i){
		prime[i] = 0;
	}
*/

	for(ax=0 ; ax<=SQRT ; ++ax){
		sum = ax*(2*BITS*BITS*ax+6*BITS)+3;
		fac = 4*BITS*ax;

		for(ay=0 ; ay<BITS ; ++ay){
			if((prime[ax] & 1L<<ay) == 0){
				offset = 2*BITS*ax+2*ay+3;

				for(b = sum+ay*(fac+6+2*ay) ; b<LEN_BITS ; b+=offset){
					prime[b/BITS] |= 1L<<b;
				}
				++size;
			}
		}
	}

	for( ; ax<LEN ; ++ax){
		for(ay=0 ; ay<BITS ; ++ay){
			if((prime[ax] & (vecType)1<<ay) == 0){
				++size;
			}
		}
	}

	return size;
}


int main(void) {
	long i;
	long n;
	long size;
	int func;
	clock_t start;
	siebn siebs[] = {
		&sieb0,
		&sieb1,
		&sieb2,
		&sieb3,
		&sieb4,
		&sieb5,
		&sieb6,
		&sieb7
	};

	//puts("2");
	/*int k;
	for(k=0 ; k < 100 ; ++k){
		start = clock();
		size = compiled_sieb();
		printf("Size: %ld Time: %f\n",size,((double)clock()-start)/CLOCKS_PER_SEC);
	}
	return 0;
*/
	/*
	func = 7;
	start = clock();

	size = siebs[func](500000001);
	printf("Size: %ld Time: %f\n",size,((double)clock()-start)/CLOCKS_PER_SEC);

	func = 5;
	start = clock();

	size = siebs[func](500000001);
	printf("Size: %ld Time: %f\n",size,((double)clock()-start)/CLOCKS_PER_SEC);


	return 0;
*/

	func = 7;

	//for(func=0 ; func<8; ++func){
		for(i=1 ; i<=10 ; ++i){
			start = clock();
			// b is BITS
			// 2x+3 number is of that form
			// x = (k*b)-1 and also at a Bit Boundary
			// 2*k*b+1
			n = i*78125*64*2+1;
			size = siebs[func](n);

			printf("%f\n",((double)clock()-start)/CLOCKS_PER_SEC);
		}
		printf("%ld\n\n",size);
	//}

	return EXIT_SUCCESS;
}
