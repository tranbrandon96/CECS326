#include <stdlib.h> /* Needed for fork */
#include <stdio.h> /* Needed for printf. */
#include <stdbool.h> /* Needed for bool, optional. */
#include <unistd.h> /* Needed for fork and exit. */
#include "sys/wait.h"
#include <pthread.h>

int sum;
//int i;
int result;
int num;

void *ChildThread()
{
     int i;
        for (i=1; i<=10 ; i++)
     {

        printf("%c%d",'C', result);
        printf("%c", num);
    //    sleep(2);

        sum += i;
        printf("sum is %d\n", sum);
        fflush(stdout);
     }
}

int main (int argc, char* argv[])
 {

  sum = 0;

  printf("sleeping\n");
  sleep(5);
  // result = fork(); /* create a new process */

  pthread_t th;
  pthread_attr_t ta;
  pthread_create(&th, NULL, ChildThread, NULL);
  pthread_attr_setdetachstate(&ta, PTHREAD_CREATE_DETACHED);

 int i;
  for (i=1; i<=10 ; i++)
  {

    printf("%c%d",'P', result);
    printf("%c", num);
  //  sleep(1);

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