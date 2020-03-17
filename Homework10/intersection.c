#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>


int main(int argc, char *argv[])
{
  key_t mykey;
  int semid;
  mykey = ftok("intersection.c", 'x');
  /* request 1 semaphore for my programs only */
  semid = semget(mykey, 1, IPC_CREAT | 0600);
  semctl(semid, 0, SETVAL, 1);

  static struct sembuf Wait = {0, -1, SEM_UNDO};
  static struct sembuf Signal = {0, 1, SEM_UNDO};
  printf("SEMID = %d\n", semid);
  int result= fork();
  int i = 0;
  while(1)
  {
     semop(semid, &Wait, 1);
     if(result == 0)
     {
        printf("N/S car entering intersection\n");
        sleep(1);
        printf("N/S car leaving intersection\n");
     }
     else
     {
        printf("E/W car entering intersection\n");
        sleep(1);
        printf("E/W car leaving intersection\n");
     }
     semop(semid, &Signal, 1);
     i++;
     if(i == 10)
        break;
  }

  return 0;
};