/*
 * Author: Alex Adams
 */

#include "lcg.h"

unsigned long findPrime(unsigned long n);

struct LinearCongruentialGenerator makeLCG(unsigned long m, unsigned long c)
{
  struct LinearCongruentialGenerator genny;
  unsigned int a;
  if(m%4==0) a= 1+(2*findPrime(m));
  else a=1+ findPrime(m);
  genny.m = m;
  genny.c = c;
  genny.x = c;
  genny.a = a;

  if(a>=m || a==0 || m==0 || c>m) genny.a = genny.m = genny.c = genny.x = 0;
  return genny;
}

unsigned long getNextRandomValue(struct LinearCongruentialGenerator* lcg)
{
  unsigned long next = lcg->x;
  lcg->x = ((lcg->x)*(lcg->a)+(lcg->c))%(lcg->m);
  return next;
}

unsigned long findPrime(unsigned long n)
{
  unsigned long testDivisor=2;
  unsigned long primes=1;

  while(1)
  {
    if(testDivisor*testDivisor > n)
    {
      primes*=n;
      return primes;
    }
    if(n%testDivisor==0)
    {
      primes*=testDivisor;
      while(n%testDivisor==0)
      {
        n/=testDivisor;
      }
    }
    else testDivisor++;
  }

}