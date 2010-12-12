#include "prime.h"

// long based array for one bit of info, not calculating even numbers
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

// start cross out at power of the prime
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

// not storing even numbers
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

// character array
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

// bit array
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

// bit array with precalculated values
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

// the sieb6 is threaded

//  only crossout up to sqrt of the number
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
