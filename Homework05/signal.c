#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void catch (int sig)
{
 if (SIGALRM == sig)
 {
        printf("Ring Ring\n");
 }
 printf("Got signal %d\n",sig);
}

int main ()
{
  int i;
  signal(SIGINT, catch);
  alarm(6);

  signal(SIGALRM, catch);

  for (i=0;i<20;i++) {
    printf("%d\n",i);
    sleep(1);
  }
  return 0;
}

/* control c prints Got signal 2 *
 after 6 seconds the program stops (Alarm clock) */