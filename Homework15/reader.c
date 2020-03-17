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
        int myshared;
        int semid;
        key_t mykey;
        int counter;

        static struct sembuf WriterWait = {0, -2, SEM_UNDO};
        static struct sembuf WriterSignal = {0, 2, SEM_UNDO};
        static struct sembuf ReaderWait = {1, -3 , SEM_UNDO};
        static struct sembuf ReaderSignal = {1, 3, SEM_UNDO};
        static struct sembuf CounterWait = {2, -1, SEM_UNDO};
        static struct sembuf CounterSignal = {2, 1, SEM_UNDO};

        struct sembuf OpList[3];

        mykey = ftok("writer.c", 'x');
        myshared = shmget(mykey, 3, IPC_CREAT | 0600);

        char *shm = shmat(myshared, 0, 0);

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

        for(int i = 0; i < 5; i++)
        {
                char *val = shm;
                OpList[0] = ReaderWait;
                OpList[1] = CounterWait;
                semop(semid, OpList, 2);

                if(semctl(semid, 3, GETVAL) == 0)
                {
                        OpList[0] = WriterWait;
                        semop(semid, OpList, 1);
                        semctl(semid, 3, SETVAL, 1);
                }

                OpList[0] = CounterSignal;
                OpList[1] = ReaderSignal;

                int val1 = *val++;
                sleep(1);
                int val2 = *val++;
                sleep(1);
                int val3 = *val++;
                printf("%d%d%d\n", val1, val2, val3);
                semop(semid, OpList, 2);
                OpList[0] = CounterWait;
                semop(semid, OpList, 1);
                semctl(semid, 3, SETVAL, 0);

                if(semctl(semid, 3, GETVAL) == 0)
                {
                        OpList[0] = WriterSignal;
                        semop(semid, OpList, 1);
                }

                OpList[0] = CounterSignal;
                semop(semid, OpList, 1);
                sleep(2);
        }
        return 0;
}
                                                                                                   84,1          Bot
