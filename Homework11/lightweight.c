#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>


int main(int argc, char *argv[])
{
  key_t mykey;
  int semid;
  mykey = ftok("lightweight.c", 'x');
  /* request 1 semaphore for my programs only */
  //semid = semget(mykey, 1, IPC_CREAT | 0600);
  //semctl(semid, 0, SETVAL, 1);

  static struct sembuf LightWeight = {0, -2, SEM_UNDO};
  static struct sembuf LightSignal = {0, 2, SEM_UNDO};

  if((semid = semget(mykey, 1, IPC_CREAT | IPC_EXCL | 0600)) != -1)
  {
  //     printf("SEMID = %d\n", semid);
       semctl(semid, 0, SETVAL, 5);
  }
  else
  {
  semid = semget(mykey, 1, 0600);
  }
  printf("SEMID = %d\n", semid);


  for(int i = 0; i < 5; i++)
  {
     semop(semid, &LightWeight, 1);
     printf("LightWeight Starting\n");
     sleep(4);
     printf("LightWeight Ending\n");
     semop(semid, &LightSignal, 1);
     sleep(8);
  }
  return 0;
};