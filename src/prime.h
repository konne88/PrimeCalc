/*
 * prime.h
 *
 *  Created on: Sep 10, 2010
 *      Author: konne
 */

#ifndef PRIME_H_
#define PRIME_H_

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

long int_sqrt(long n);

typedef long (*siebn)(long n);

long sieb0(long n);
long sieb1(long n);
long sieb2(long n);
long sieb3(long n);
long sieb4(long n);
long sieb5(long n);
long sieb6(long n);
long sieb7(long n);
long sieb8(long n);
long sieb9(long n);

#endif /* PRIME_H_ */
