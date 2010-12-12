/* This function calculates prime numbers, basically using
 * the Sieve of Eratosthenes algorythm. This algrythmn does
 * it's work on a numberarray. It iterates through it and flags
 * all number that are not primenumbers. To speed things up,
 * it is modified in the following ways:
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
 * - The first number to cross out is the power of the primenumber
 * - Somehow the flags need to go from the memory into the cpu.
 *   This
 *   to do calculations on them. so we don't cross out just
 *   everything in the array but we actually load sections
 *   of the memory to do calcuations on it. The first section
 *   we get is the size of the cache. then we cross out everything
 *   in here. it is not that simple since we can't just adress an
 *   index of the cache, so we need to load some bits and do
 *   our calculations on them.
 */

#include "prime.h"

long sieb8(long n){
	typedef long vecType;				// type of a calcunit
	#define bits (8*sizeof(vecType))	// bits per calcunit

	// b is the number of bits per calcblock
	// the number p is at the index k
	// p = 2k+3
	// the index k is at calcblock x and bit y
	// bx+y = k
	// p = 2(bx+y)+3

	// to calc the size of array we need to make sure
	// that the number n (passed to function) is in the array
	// n = 2(bx+y)+3
	// (n-3)/2 = bx+y
	//
	// the size in bits we need s in defined as
	// s = ((n-3)/2)+1 = (n-1)/2
	// because the size is the highest index +1
	//
	// the amount of calcblocks we need l is
	// l = (n-1)/(2b)
	//
	// so to use the full memory power pass numbers n that
	// fullfill the following equation, where l is an int
	// n = 2bl+1
	long len = (n-1)/(2*bits);
	long lenBits = (n-1)/2;

	// stores the numberarray
	// if a bit is 1 in this array means that the number is not not a prime number
	vecType* prime;

//	vecType* prime = malloc(len*sizeof(vecType));

	if(posix_memalign((void**)&prime,getpagesize(),len*sizeof(vecType)) != 0){
		return -1;
	}
	memset(prime,0,len*sizeof(vecType));

	long sqrt = int_sqrt(len)+1;  // numbers only need to be checked up to the sqrt of the len
	long size = 1;	              // the amount of primenumbers found

	long ax;	// the calcblock index
	long ay;    // the bit index in the calcblock

	// decribed on initialization
	long current;
	long offset;
	long axbits2plus3;

	// we now iterate over the numberarray
	// ax and ay represent the real number 2*(BITS*ax+ay)+3
	// only calculations up to sqrt are nessecary
	for(ax=0 ; ax<=sqrt ; ++ax){
		// calculate a number that is always the same for the same ax
		// makes computations faster later on
		axbits2plus3 = ax*bits*2+3;

		for(ay=0 ; ay<bits ; ++ay){
			// test the actual bit, if true it is a prime
			if((prime[ax] & (vecType)1<<ay) == 0){
				// now cross out all multiples of this prime number

				// the first index we cross out is the current
				// primenumber squared cause all smaller multiples
				// of the prime are cross out already, the bit index k of
				// that sqaure can be calculated as
				// current prime number is
				// p = 2*(bits*ax+ay)+3
				// first crossout number n is
				// n = p^2
				// bit index k of that number is
				// n = 2k+3
				// k = (n-3)/2
				// k = (p^2-3)/2

				// to get from one multiple to the next we just
				// add an offset that we can calculate as
				// primenumber p is
				// p = 2*(bits*ax+ay)+3
				// second crossed out number n is
				// n = p^2+p
				// which is at the index k
				// n = 2k+3
				// k = (n-3)/2
				// k = (p^2+p-3)/2
				// k = (p^2-3)/2 + p/2
				// we see that the second cross out is p/2 after the
				// first crossout which is our offset.
				// but wait, we don't really need to check the
				// even multiples of the prime number since they were
				// removed already. All checked primenumbers are uneven so their
				// square is uneven too. so we only need to check every other offset
				// leading us to an index offset that is exactly the primenumber !!!
				// (this is pure luck)

				// 2*(ax*bits+ay)+3 = 2*ax*bits+3 + 2*ay
				offset = axbits2plus3 + 2*ay;

				// 2*(bits*ax+ay)+3

				// the index after the first index is the number

				current = (offset*offset-3)/2;

				// current and offset are messured in bits
				for( ; current<lenBits ; current+=offset){
					prime[current/bits] |= (vecType)1<<(current%bits);
				}

				++size;
			}
		}
	}

	// now make up for only calculating up to sqrt
	// and add the rest of primenumbers to the size
	for( ; ax<len ; ++ax){
		for(ay=0 ; ay<bits ; ++ay){
			if((prime[ax] & (vecType)1<<ay) == 0){
				++size;
			}
		}
	}

	free(prime);

	return size;

	#undef bits
}

#undef bits
