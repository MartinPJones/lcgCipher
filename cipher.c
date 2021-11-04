#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include "lcg.h"

int encrypt(unsigned long c, unsigned long m, unsigned int line);

int decrypt(unsigned long c, unsigned long m, unsigned int line);

int errorOut(unsigned int line);

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
  int tmp[21];
  char converter[21] = {0};
  int counter = 0;
  int otherCounter = 0;
  unsigned int lineCounter = 0;
  mode = getchar();
  while(ender != 1)
  {
    if(mode != 'e' && mode != 'd')
    {
      lineCounter++;
      errorOut(lineCounter);
      otherCounter = 0;
      mode = getchar();
    }
    tmp[otherCounter] = getchar();
    /* I don't know why this works, but if I try to set tmp to char everything
     * just crashes.*/
    converter[otherCounter] = (char) tmp[otherCounter];
    /* If character is printable */
    if(tmp[otherCounter]<127 && tmp[otherCounter]>31)
    {
      if (tmp[otherCounter] == ',')
      {
        converter[otherCounter] = 0;
        tmp[otherCounter] = 0;
        errno = 0;
        c = strtoul(converter, &endptr, 10);
        if ((errno == ERANGE && (m == LONG_MAX || m == LONG_MIN))
            || (errno != 0 && m == 0))
        {
          ender = errorOut(lineCounter);
          lineCounter++;
        }
        memset(tmp, 0, sizeof tmp);
        memset(converter, 0, sizeof converter);
        otherCounter = 0;
        if (counter == 0)
        {
          m = c;
          counter++;
        }
        else
        {
          lineCounter++;
          switch (mode)
          {
            case 'e':
              ender = encrypt(c, m, lineCounter);
              mode = getchar();
              counter = 0;
              break;
            case 'd':
              ender = decrypt(c, m, lineCounter);
              mode = getchar();
              counter = 0;
              break;
            default:;
              ender = errorOut(lineCounter);
              lineCounter++;
              otherCounter=0;
              counter = 0;
          }
        }
      }
      else otherCounter++;
    }
    else if(tmp[otherCounter]!='\n')
      {
        ender = errorOut(lineCounter);
        lineCounter++;
        otherCounter=0;
        counter=0;
      }
    else
    {
      lineCounter++;
      printf("%5d: Error\n", lineCounter);
      memset(tmp, 0, sizeof tmp);
      memset(converter, 0, sizeof converter);
      otherCounter = 0;
      counter = 0;
      mode = getchar();
    }
    if(otherCounter>20)
    {
      lineCounter++;
      ender = errorOut(lineCounter);
      otherCounter=0;
    }
  }
  return 0;
}

int errorOut(unsigned int line)
{
  int tmp;
  printf("%5d: Error\n", line);
  tmp = getchar();
  while (tmp!='\n' && tmp!=EOF)
  {
    tmp = getchar();
  }
  if(tmp == EOF)
    return 1;
  return 0;
}

int decrypt(unsigned long c, unsigned long m, unsigned int line)
{
  struct LinearCongruentialGenerator lcg = makeLCG(m, c);
  int tmp = getchar();
  int tmp2;
  int rand;

  printf("%5d: ",line);

  while(tmp != '\n')
  {
    if (tmp == EOF) return 1;
    rand = getNextRandomValue(&lcg)%128;
    if(tmp == '*')
    {
      tmp2 = getchar();
      if(tmp2 == '*')
      {
        printf("%c", (char)('*'^rand));
      }
      else if (tmp2 == '#')
      {
        printf("%c", (char)(127^rand));
      }
      else
      {
        if(((tmp2-'@')^rand)>31 && ((tmp2-'@')^rand)<127)
          printf("%c", (char) ((tmp2-'@')^rand));
        else
        {
          printf("Error\n");
          while(tmp!='\n') tmp = getchar();
          return -1;
        }
      }
    }
    else printf("%c",(char)(tmp^rand));
    tmp = getchar();
  }
  printf("\n");
  return 0;
}

int encrypt(unsigned long c, unsigned long m, unsigned int line)
{
  int tmp;
  unsigned long rand;
  struct LinearCongruentialGenerator lcg = makeLCG(m,c);
  if(lcg.m==0)
  {
    return errorOut(line);
  }
  tmp = getchar();

  printf("%5d: ",line);

  while(tmp!='\n')
  {
    if(tmp == EOF) return 1;
    if(tmp<32 || tmp>=127)
    {
      errorOut(line);
      return -1;
    }
    rand = getNextRandomValue(&lcg);
    if((tmp^(rand%128))<32) printf("*%c", (char)(tmp^(rand%128))+'@');
    else if((tmp^rand%128)==127) printf("*#");
    else if((tmp^rand%128)=='*') printf("**");
    else printf("%c", (char)(tmp^(rand%128)));
    tmp=getchar();
  }
  printf("\n");
  return 0;
}
