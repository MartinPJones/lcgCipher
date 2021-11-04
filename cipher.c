#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include "lcg.h"

int encrypt(unsigned long c, unsigned long m, unsigned int line);

int decrypt(unsigned long c, unsigned long m, unsigned int line);

void errorOut(unsigned int line);

/*
 * Author: Alex Adams
 */

int main()
{
  int mode;
  int errno;
  int ender = 0;
  unsigned long c;
  unsigned long m;
  char* endptr;
  int tmp[20] = {0};
  int counter = 0;
  int otherCounter = 0;
  unsigned int lineCounter = 0;
  mode = getchar();
  while(ender != 1)
  {
    tmp[otherCounter] = getchar();
    /* If character is printable */
    if(tmp[otherCounter]<127 && tmp[otherCounter]>31)
    {
      if (tmp[otherCounter] == ',')
      {
        errno = 0;
        m = strtol((char *) tmp, &endptr, 10);
        if ((errno == ERANGE && (c == LONG_MAX || c == LONG_MIN))
            || (errno != 0 && c == 0))
        {
          errorOut(lineCounter);
          lineCounter++;
          otherCounter=0;
        }
        if (counter == 0)
        {
          c = m;
          otherCounter = 0;
          counter++;
        }
        else
        {
          lineCounter++;
          switch (mode)
          {
            case 'e':
              ender = encrypt(c, m, lineCounter);
              break;
            case 'd':
              ender = decrypt(c, m, lineCounter);
              break;
            default:;
              errorOut(lineCounter);
              lineCounter++;
              otherCounter=0;
          }
        }
      }
    }
    else if(tmp[otherCounter]!='\n')
      {
        errorOut(lineCounter);
        lineCounter++;
        otherCounter=0;
      }
    else lineCounter++;
    otherCounter++;
    if(otherCounter>20)
    {
      errorOut(lineCounter);
      lineCounter++;
      otherCounter=0;
    }
  }
  return 0;
}

void errorOut(unsigned int line)
{
  int tmp;
  printf("%5d: Error", line);
  tmp = getchar();
  while (tmp!='\n')
  {
    tmp = getchar();
  }
}

int decrypt(unsigned long c, unsigned long m, unsigned int line)
{
  struct LinearCongruentialGenerator lcg = makeLCG(m, c);
  int tmp = getchar();
  int tmp2;

  printf("%5d: ",line);

  while(tmp != '\n')
  {
    if (tmp == EOF) return 1;
    if(tmp == '*')
    {
      tmp2 = getchar();
      if(tmp2 == '*')
      {
        printf("%c", (char)('*'^(getNextRandomValue(&lcg)%128)));
      }
      else if (tmp2 == '#')
      {
        printf("%c", (char)(127^(getNextRandomValue(&lcg)%128)));
      }
      else
      {
        if(tmp2-'@'>32 && tmp2-'@'<127) printf("%c", (char)((tmp2-'@')^
            (getNextRandomValue(&lcg)%128)));
        else
        {
          printf("%5d: Error", line);
          return -1;
        }
      }
    }
    else printf("%c",(char)(tmp^(getNextRandomValue(&lcg)%128)));
    tmp = getchar();
  }
  return 0;
}

int encrypt(unsigned long c, unsigned long m, unsigned int line)
{
  int tmp;
  unsigned long rand;
  struct LinearCongruentialGenerator lcg = makeLCG(m,c);
  if(lcg.m==0)
  {
    printf("%5d: Error", line);
    return -1;
  }
  tmp = getchar();

  printf("%5d: ",line);

  while(tmp!='\n')
  {
    if(tmp == EOF) return 1;
    if(tmp<32 || tmp>=127)
    {
      printf("%5d: Error", line);
      return 1;
    }
    rand = getNextRandomValue(&lcg);
    if((tmp^(rand%128))<32) printf("*%c", (char)(tmp^(rand%128)));
    else if((tmp^rand%128)==127) printf("*#");
    else printf("%c", (char)(tmp^(rand%128)));
    tmp=getchar();
  }
  return 0;
}
