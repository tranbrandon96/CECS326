#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>

int main ()
{
        struct sembuf Wait[2] = {{0, -1, SEM_UNDO}, {1, -1, SEM_UNDO}};
        struct sembuf Signal[2] = {{0, 1, SEM_UNDO}, {1, 1, SEM_UNDO}};
        //struct sembuf Wait = {0, -1, SEM_UNDO};
        //struct sembuf Signal = {0, 1, SEM_UNDO}; 
        struct sembuf OpList[2];

        int ch;
        key_t mykey;
        int semid;


        mykey = ftok("./writers.c", 'a');
        semid = semget(mykey, 1, IPC_CREAT | 0600);
        semctl(semid, 0, SETVAL, 0); /*lock*/ //lock 1 writer
        semctl(semid, 1, SETVAL, 0); /*lock*/  //lock 3 readers


        for(int i = 0; i< 5; i++)
        {

                OpList[0] = Wait[0];
                OpList[1] = Wait[1];
                printf("Writing\n");
                semop(semid, OpList, 2);
                sleep(4);
                printf("Done Writing\n");
                OpList[0] = Signal[0];
                OpList[1] = Signal[1];
                semop(semid, OpList, 2);
                sleep(8);
        }

        return 0;


}