#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>

int main ()
{
        static struct sembuf ReaderWait = {0, -2, SEM_UNDO};
        static struct sembuf ReaderSignal = {0, 2, SEM_UNDO};
        static struct sembuf Wait = {0, -1, SEM_UNDO}:
        static struct sembuf Signal = {0, 1, SEM_UNDO};
        struct sembuf OpList[2];

        int ch;
        key_t mykey;
        int semid;


        mykey = ftok("./readers.c", 'a');
        semid = semget(mykey, 4, IPC_CREAT | 0600);
        semctl(semid, 0, SETVAL,0); /*lock*/
        semctl(semid, 1, SETVAL,0); /*lock*/


        for(int i = 0; i< 5; i++)
        {

                printf("Reading\n");
                sleep(2);
                printf("Done Reading\n");

                sleep(4);
        }

        /*

        if(fork() == 0)
        {
                OpList[0] = Wait[0];
                OpList[1] = Wait[1]; 
                printf("Waiting\n");
                semop(semid, OpList,2);
                printf("Unlocked\n");
        }
        else
        {
                sleep(3);
                OpList[0] = Signal[0];
                OpList[1] = Signal[1];
                semop(semid, OpList, 2);
                wait3(&ch, 0, NULL);
        }
        */
        return 0;
}