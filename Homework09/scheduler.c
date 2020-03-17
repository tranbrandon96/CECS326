/*
   The parser piece for the scheduler assignment
*/
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
/* You will need the integer queue */
#include "IQueue.h"
int main(int argc, char* argv[])
{
  int fd;
  FILE *f;
  char line[80];
  int start_time, duration;
  int i;

  struct process_struct
  {
    int process_id;
    int start_time;
    int duration;
    int time_left;
  };
  struct process_struct process_list[80];
  int process_count;

  if (argc !=2)
  {
    printf("usage a.out file_name\n");
    return 0;
  };
  f = fopen(argv[1],"r");
  process_count = 0;

  while(fgets(line,sizeof(line),f) != NULL)
  {
    sscanf(line, "%d %d", &start_time, &duration);
    process_list[process_count].process_id = process_count;
    process_list[process_count].start_time = start_time;
    process_list[process_count].duration = duration;
    process_list[process_count].time_left = duration;
    process_count++ ;
  }

  int index = 0;
  int time = 0;

  while(1)
  {
    //process_list[index++];

    if(process_list[index].start_time == time)
    {
      IQueueStore(process_list[index].process_id);

//      if(IQueueSize() > 0)
//      {
//        IQueueStore(IQueueRetrieve());
//      }

     // printf("%s      Process%d", time, ps.process_id);
     // ps.time_left = ps.time_left - 5;

     // if(ps.time_left > 0)
     // {
     //   IQueueStore(ps.process_id);
     // }
    }
    else if(index < process_count)
    {
      index--;
    }

    if(IQueueSize() > 0)
    {
      //struct process_struct temp = process_list[IQueueRetrieve()];
      int pid = IQueueRetrieve();

      printf("%d     Process%d\n", time, process_list[pid].process_id);

      //process_list[pid].time_left -= 5;

      if(process_list[pid].time_left > 0)
      {
        IQueueStore(pid);
      }

      process_list[pid].time_left -= 5;
    }
    else
    {
      printf("%d     Idle %d\n", time, process_count);

      if(index >= process_count)
      {
        break;
      }
    }

    time = time + 5;
    index++;

//    if(time == 90)
//      break;
  }
//  for (i = 0 ; i < process_count ; i++)
//  {
//      printf("process %d, start  %d, duration %d, left %d \n",
//      process_list[i].process_id,
//      process_list[i].start_time,
//      process_list[i].duration,
//      process_list[i].time_left);
//  }

  printf("DONE\n");
  close(fd);
};
                                                                                                                                                                                                                            119,1         Bot
