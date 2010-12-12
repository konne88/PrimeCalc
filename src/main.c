#include "prime.h"

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

long int_sqrt(long n)
{
	long a;
    for (a=0;n>=(2*a)+1;n-=(2*a++)+1);
    return a;
}

int main(void) {
	long i;
	long n;
	long size;
	int func;
	clock_t start;

	// mapping of the available prime calculation functions
	siebn siebs[] = {
		&sieb0,	// 0
		&sieb1,	// 1
		&sieb2,	// 2
		&sieb3,	// 3
		&sieb4,	// 4
		&sieb5,	// 5
		&sieb7,	// 6
		&sieb8,	// 7
		&sieb9	// 8
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

//	func = 7;

	// iterate throught the prime functions and calc their values
	for(func=8 ; func<9; ++func){
		for(i=400 ; i<=400 ; i+=1){
			start = clock();
			// b is BITS
			// 2x+3 is the value of the number at index x
			// x = (k*b)-1 and also at a Bit Boundary
			// 2*k*b+1
			n = i*1024*1024*8*2*2+1;//78125*64*2+1;
			size = siebs[func](n);

		//	printf("%f\n",((double)clock()-start)/CLOCKS_PER_SEC);
		}
//		printf("%d - %ld\n\n",func,size);
	}

	return EXIT_SUCCESS;
}
