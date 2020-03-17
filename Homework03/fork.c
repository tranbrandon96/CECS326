#include <stdlib.h> /* Needed for fork */
#include <stdio.h> /* Needed for printf. */
#include <stdbool.h> /* Needed for bool, optional. */
#include <unistd.h> /* Needed for fork and exit. */
#include "sys/wait.h"

int sum;
int main (int argc, char* argv[])
 {
  int i;
  int result;
  sum = 0;
  result = fork();  /* create a new process */

  for (i=1; i<=10 ; i++)
  {
     if(result == 0)
     {
        printf("%c%d",'C', result);
        sleep(2);

     }
     else
     {
        printf("%c%d",'P', result);
        sleep(1);
     }
    sum += i;
    printf("sum is %d\n", sum);
    fflush(stdout);
  }
  if(argc > 1 && result != 0)
  {
        wait3(NULL , 0, NULL);
  }
  if(argc < 1 || result == 0)
  {
        return 1;
  }


  return 0;
};