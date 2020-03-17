#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

int main()
{
  int myqueue_rcv;
  int myqueue_snd;
  key_t mykey_rcv;
  key_t mykey_snd;
  int status;
  int multiplicand;
  int multiplier;
  int product;

  int msgsz = 4;

  struct Mybuf
  {
    long mtype;
    int num;
  };

  struct Mybuf bufrcv;
  struct Mybuf bufsnd;

  mykey_snd = ftok("servermes.c", 'x');
  mykey_rcv = ftok("clientmes.c", 'x');

  myqueue_snd = msgget(mykey_snd, IPC_CREAT | 0600);
  myqueue_rcv = msgget(mykey_rcv, IPC_CREAT | 0600);

  if(myqueue_snd == -1 || myqueue_rcv == -1)
  {
    printf("error in msgget");
    return 1;
  }

  for(int i = 0; i < 5; i++)
  {
    bufsnd.mtype = 1;

    printf("Multiplicand: ");
    scanf("%d", &multiplicand);
    bufsnd.num = multiplicand;

    status = msgsnd(myqueue_snd, (struct msgbuf *)&bufsnd, sizeof(msgsz), 0);
    if(status == -1)
    {
      printf("error in msgsnd %s \n", strerror(errno));
      return 1;
    }
   printf("Multiplier: ");
    scanf("%d", &multiplier);
    bufsnd.num = multiplier;

    status = msgsnd(myqueue_snd, (struct msgbuf *)&bufsnd, sizeof(msgsz), 0);
    if(status == -1)
    {
      printf("error in msgsnd %s \n", strerror(errno));
      return 1;
    }

    if(msgrcv(myqueue_rcv, (struct msgbuf *)&bufrcv, sizeof(msgsz), 0, 0) == -1)
    {
      printf("error in msgrcv\n");
      return 1;
    }

    product = bufrcv.num;
    printf("%d x %d = %d\n\n", multiplier, multiplicand, product);
  }

  return 0;
}