#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

int main()
{
        static struct sembuf WriterWait = {0, -2, SEM_UNDO};
        static struct sembuf WriterSignal = {0, 2, SEM_UNDO};
        static struct sembuf ReaderWait = {1, -3 , SEM_UNDO};
        static struct sembuf ReaderSignal = {1, 3, SEM_UNDO};

        struct sembuf OpList[2];

        int myshared;
        key_t mykey;
        int semid;
        int ch;

        mykey = ftok("writer.c", 'x');
        myshared = shmget(mykey, 3, IPC_CREAT | 0600);

        char *shm = shmat(myshared, 0, 0);
        char *val = shm;

        for(int i = 0; i < 3; i++)
        {
                *val++ = 1;
        }

        if((semid = semget(mykey, 4, IPC_CREAT | IPC_EXCL | 0600)) != -1)
        {
                semctl(semid, 0, SETVAL, 5);
                semctl(semid, 1, SETVAL, 5);
                semctl(semid, 2, SETVAL, 1);
                semctl(semid, 3, SETVAL, 0);
        }
        else
        {
                semid = semget(mykey, 1, 0600);
        }

        printf("semid: %d\n", semid);

        int inc = 1;

        for(int i = 1; i <= 5; i++)
        {
                inc++;
                char *write = shm;
                OpList[0] = WriterWait;
                OpList[1] = ReaderWait;
                semop(semid, OpList, 2);
                *write++ = inc;
                sleep(1);
                *write++ = inc;
                sleep(1);
                *write = inc;
                OpList[0] = WriterSignal;
                OpList[1] = ReaderSignal;
                semop(semid, OpList, 2);
                sleep(3);
        }
        return 0;
}