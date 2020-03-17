/*
   The parser piece for the minishell assignment
   Replace the part that prints the parsed command
   with you code that does the fork, exec, wait.
*/
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include "sys/wait.h"
int main(int argc, char* argv[])
{
  char line[40];
  char *nargs[10];
  char *space = " ";
  char *where;
  while(1)
  {
    int i = 0;
    printf("Your command please: ");
    fgets(line,sizeof(line),stdin);
    line[strlen(line)-1] = '\0';
    where = strtok(line,space);
    nargs[i++] = where;
    while (where != NULL)
    {
      where = strtok(NULL,space);
      nargs[i++] = where;
    }
    /* Print the results */
    printf("Your parsed command was:\n");
    if (fork() > 0)
    {
        /* wait for the child to complete */
        wait3(NULL, 0, NULL);
    }
    else if (fork() == 0)
    {
        /* int execvp(const char *file, char *const argv[]) */
        execvp(nargs[0],nargs);
    }

  }
};
