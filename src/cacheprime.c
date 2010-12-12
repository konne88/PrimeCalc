// Don't be scared, without comments this file has only 77 lines of code.

/* This function calculates prime numbers, basically using
 * the Sieve of Eratosthenes algorithm. This algrythmn does
 * it's work on a numberarray. It iterates through it and flags
 * all number that are not primenumbers (crosses them out).
 *  To speed things up, it is modified in the following ways:
 * - The numberarray only flags uneven number higher or equal to 3
 *   so the flag at index k in the memory represents the real
 *   number p = 2k+3
 * - Each flag is saved as a bit and not as a byte, so the
 *   previously mentioned index i is actually the index of the
 *   bit to use, which is not adressable.
 * - Since we can not directly calculate on a bit we need to load
 *   a bigger unit of information from the memory and do calculations
 *   on it, let us call it calcblock. We actually do calculations
 *   on the standard variable (which is a long) of the processor,
 *   in most cases 32 or 64 bit, the bits of this variable shall be b.
 *   Let k be the index of the bit (as previously defined) and
 *   x be the index of the calcblock and y the index of the bit in
 *   the calcblock then
 *   k = bx+y
 *   p = 2(bx+y)+3 = 2bx+2y+3
 * - We only crossout multiples of primenumber up to the sqrt of the
 *   size of the array. Everything bigger is already crossed correctly
 * - The first number to cross out is the power of the primenumber.
 * - Somehow the flags need to go from the memory into the cpu.
 *   If we read from the entire memory it is very slow cause
 *   we have a lot of cache misses. In order to avoid that we do
 *   calculations only on a limited area of the memory, so big
 *   that it fits into the cache of the cpu. We start by crossing out
 *   numbers in a section the size of the cache. All found primes are
 *   stored in a list, then we load the next cache block cross out
 *   everything including the list of primes we already found, and so on.
 */

#include "prime.h"
#include "stdio.h"

/* We need to use found prime numbers to crossout numbers
 * in our numberarray.
 * Since we move through the set of primenumbers in blocks
 * (see cache optimization) we need to store all the prime
 * numbers, that we found in an old block already, in a list.
 * Each prime_cross_iter represents one primenumber.
 * Current is the position where the last croosout occured
 * (but it is not quite right, it is actually the offset from
 * the first bit of the current cache to to the last crossout)
 * Offset is used to find the next crossout we need to do
 * after the current one.
 */
typedef struct {
	long offset;
	long current;
} prime_cross_iter;

#define PRINT

long sieb9(long n){
	typedef long vecType;						// type of a calcunit
	#define bits ((long)(8*sizeof(vecType)))	// bits per calcunit
	long cachebytes = 2*1024*1024;				// size of the cache in bytes
	long calcs = cachebytes/sizeof(vecType);	// calcunits per cache
	long cachebits = calcs*bits;

	// there are some basic mathematical transformations
	// that are introduced here

	// b is the number of bits per calcblock
	// c the number of calcblocks per cache
	// the number p is at the index k
	// p = 2k+3
	// the index k is at cache z in calcblock x and bit y
	// cbz+bx+y = k
	// p = 2(cbz+bx+y)+3

	// to calc the size of the numberarray we need to make sure
	// that the number n (passed to function) is in the array
	// n = 2(cbz+bx+y)+3
	// (n-3)/2 = cbz+bx+y
	//
	// the size in bits s we need is defined as
	// s = ((n-3)/2)+1 = (n-1)/2
	// because the size is the highest index +1
	//
	// the amount of calcblocks l we need is
	// l = (n-1)/(2b)
	//
	// so to use the full memory power pass numbers n that
	// fullfill the following equation, where l is an int
	// n = 2*b*c*l+1
	long caches = (n-1)/(2*bits*calcs);

	// stores the numberarray
	// if a bit is 1 in this array, it means that the number is not not a prime number
	vecType* prime;
	if(posix_memalign((void**)&prime,getpagesize(),cachebytes) != 0){
		return -1;
	}

	// this is the size of the list we use to store the found
	// prime numbers. We only store primenumbers up to the
	// sqrt of the highest number we should check which we will call n.
	// the size of this array is just a guess and will run out
	// if n is too big
	long lenPrimeList = 1024*1024*sizeof(prime_cross_iter);
	// stores already found primenumbers
	prime_cross_iter* primeList;
	if(posix_memalign((void**)&primeList,getpagesize(),lenPrimeList) != 0){
		return -1;
	}
	// the index to insert the next prime
	long nextPrime = 0;

	// primes only need to be stored up to the sqrt of the numbers to be checked
	long sqrt = int_sqrt(n)+1;

	// number of primes already found
	// the value is 1 because 2 is found implicitly
	long size = 1;
#ifdef PRINT
	printf("2\n");
#endif

	long ac; // the cachepage index
	long ax; // the calcblock index
	long ay; // the bit index

	long passedBits;	// cache offset in bits
	long computed;		// a computed number used for optimization
	long checkedPrime;	// the prime number we are checking right now

	// we now iterate over the numberarray, this means that
	// we go throught each odd number there is and check
	// if it is a primenumber or not.
	// ac represents the cachepage we are currently at
	// ax is the calcunit in the cache and
	// ay represent the bit in the calcunit
	// the currently looked at number p is therefore
	// p = 2*(BITS*(ac*calcs+ax)+ay)+3
	// only calculations up to sqrt are nessecary
	for(ac=0 ; ac < caches ; ++ac){
		// clear the cache memory
		memset(prime,0,cachebytes);

		// the number of bits that we processed already
		passedBits = ac*cachebits;

		// crossout all already found primes in the cache
		long i;
		for(i = 0 ; i<nextPrime ; ++i){
			prime_cross_iter* iter = primeList+i;
			// adopt to the fact, that indexes of our cache
			// now represent new numbers, because current
			// is a relative offset to the current cache
			iter->current -= cachebits;
			// if you don't understand this see below
			while(iter->current < cachebits){
				prime[iter->current/bits] |= (vecType)1<<(iter->current%bits);
				iter->current += iter->offset;
			}
		}

		// iterate over all calcunits in the cache
		for(ax=0 ; ax<calcs ; ++ax){
			// calculate a number that is always the same for the same ax
			// makes computations faster later on
			computed = 2*(passedBits+ax*bits)+3;

			// iterate over the bits of the current calcunit
			for(ay=0 ; ay<bits ; ++ay){
				// test the actual bit, if it is 0 we have a prime,
				// if it is 1 it isn't and we don't have to care about it
				if((prime[ax] & (vecType)1<<ay) == 0){
					// currently looked at primenumber
					// 2*(bits*(ac*calcs + ax) + ay)+3 = 2*bits*(ac*calcs+ax)+3 + 2*ay
					checkedPrime = computed + 2*ay;

					// we only need to store the primenumber if it is less than the square
					if(checkedPrime < sqrt){
						// store the primenumber we just found in the list of primes
						prime_cross_iter* iter = primeList+nextPrime;

						// now cross out all multiples of this prime number

						// the index of the first number we need to crossout
						// is called current or n. It is the currently looked at
						// primenumber squared, cause all smaller multiples
						// of the prime are cross out already, the bit index k of
						// that sqaure can be calculated as
						// the currenty looked at prime number is
						// p = 2*(calcs*bits*ac+bits*ax+ay)+3
						// first crossout number n is
						// n = p^2
						// bit index k of that number is
						// n = 2k+3
						// k = (n-3)/2
						// k = (p^2-3)/2

						// to get from one multiple to the next we just
						// add an offset that we can calculate as
						// currently looked at primenumber p is
						// p = 2*(calcs*bits*ac+bits*ax+ay)+3
						// the second number we need to crossout n is
						// n = p^2+p
						// which is at the index k
						// n = 2k+3
						// k = (n-3)/2
						// k = (p^2+p-3)/2
						// k = (p^2-3)/2 + p/2
						// we see that the second cross out is p/2 after the
						// first crossout, so p/2 is our offset.
						// but wait, we don't really need to check the
						// even multiples of the prime number since they were
						// removed already. All checked primenumbers are uneven so their
						// square is uneven too. so we only need to check every other offset
						// leading us to an index offset that is exactly the primenumber !!!
						// (this is pure luck)
						// current and offset are messured in bits
						iter->offset = checkedPrime;

						// the first number we need to crossout is at this bit position
						// in the current cache
						iter->current = (checkedPrime*checkedPrime-3)/2 - ac*cachebits;

						// now crossout all number that are in the current cache
						while(iter->current < cachebits){
							// indicate that this number is not a prime
							prime[iter->current/bits] |= (vecType)1<<(iter->current%bits);
							// check the next number
							iter->current += checkedPrime;
						}

						// increase the prime list position
						++nextPrime;
					}

#ifdef PRINT
					printf("%ld\n",checkedPrime);
#endif
					// increase the amount of found primes
					++size;
				}
			}
		}
	}

	free(primeList);
	free(prime);
	return size;

	#undef bits
}

#undef bits
