#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

int main()
{
  int myqueue_snd;
  int myqueue_rcv;
  key_t mykey_snd;
  key_t mykey_rcv;
  int status;
  int multiplicand;
  int multiplier;

  int msgsz = 4;

  struct Mybuf
  {
    long mtype;
    int num;
  };

  struct Mybuf bufrcv;
  struct Mybuf bufsnd;

  mykey_rcv = ftok("servermes.c", 'x');
  mykey_snd = ftok("clientmes.c", 'x');

  myqueue_rcv = msgget(mykey_rcv, IPC_CREAT | 0600);
  myqueue_snd = msgget(mykey_snd, IPC_CREAT | 0600);

  if(myqueue_rcv == -1 || myqueue_snd == -1)
  {
    printf("error in msgget\n");
    return 1;
  }

  while(1)
  {
    if(msgrcv(myqueue_rcv, (struct msgbuf *)&bufrcv, sizeof(msgsz), 0, 0) == -1)
    {
      printf("error in msgrcv\n");
      return 1;
    }
    multiplicand = bufrcv.num;

    if(msgrcv(myqueue_rcv, (struct msgbuf *)&bufrcv, sizeof(msgsz), 0, 0) == -1)
    {
      printf("error in msgrcv\n");
      return 1;
    }
    multiplier = bufrcv.num;

    bufsnd.mtype = 1;
    bufsnd.num = multiplier * multiplicand;

    status = msgsnd(myqueue_snd, (struct msgbuf *)&bufsnd, sizeof(msgsz), 0);

    if(status == -1)
    {
      printf("error in msgsnd %s \n", strerror(errno));
      return 1;
    }
  }

  return 0;
}