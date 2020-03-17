/* paging.c */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/* Write a value to memory */
void wr_mem(int Location, int Value);

/* Read a value from memory. */
int rd_mem(int Location);

/* These declarations are global so your procedures can use them */

/* File descriptor for the page file */
int pageFile;

/* Main memory, also called RAM */
int memory[5];

/* The page table, we have only one for this assignment.*/
int pagetable[8];

/*
 Put any globals you need to share between the two procdeures here
 You need the various arrays to remember if a page is in memory,
 and to remember if it was reference and to track what is in a frame.
 You also need to remember where you are in your memory scan.
 You put them here so you can initialize them in the main program
 (once) and use them in both your procedures.
*/

// Keep track of the last page scanned
int ref_bit[5];

// To remember if the page is in memory
int page_bit[8];
int count;


int main()
{
  int loopcount;
  /* set up the page file */
  pageFile = open("pagefile.bin",O_RDWR|O_CREAT|O_TRUNC,S_IRUSR | S_IWUSR);
  /* 8 bytes of 0 */
  for (loopcount = 0 ; loopcount < 8 ; loopcount++)
  {
    write(pageFile,"\0",1); /* cheap way to have an array with a 0 */
  }

  /* YOU need to initialize your globals here. */
  for(int i = 0; i < 8; i++)
  {
        if(i < 5)
        {
                memory[i] = -1;
                ref_bit[i] = -1;
        }
        page_bit[i] = -1;
  }

  count = 0;
/*
    YOU DO NOT CHANGE the rest of the main program, this is the part
    that tests to see if you've implemented paging correctly.
  */

  /* initialize all pages to contain a value */
  for (loopcount = 0 ; loopcount < 8 ; loopcount++)
  {
    /* mem[loopcount] = loopcount; */
    wr_mem(loopcount, loopcount);
  };

  /* cause some more paging */
  wr_mem(4, rd_mem(2) + rd_mem(4) ); /* mem[4] = mem[2] + mem[4] */
  wr_mem(6, rd_mem(2) + rd_mem(7) ); /* mem[6] = mem[2] + mem[7] */
  wr_mem(1, rd_mem(1) + rd_mem(3) ); /* mem[1] = mem[1] + mem[3] */
  wr_mem(5, rd_mem(3) + rd_mem(7) ); /* mem[5] = mem[3] + mem[7] */

  /* we are done the test */
  close(pageFile);
  return 0;
};

/*
 This procedure illustrates the use of the lseek command.
 It reads the fourth byte (offset 3) of the page file.
*/
int DummyProcedure()
{
  char ch;
  lseek(pageFile,3,SEEK_SET);
  read(pageFile,&ch,1);
  return ch;
};
/* YOU need to implement wr_mem here. */
void wr_mem(int Location, int Value)
{
        int temp = count % 5;
        if(memory[temp] == -1)
        {
                memory[temp] = Location;
                ref_bit[temp] = 0;
                page_bit[Location] = temp;
                pagetable[Location] = Value;
                printf("Page %d is loaded into frame %d\n", Location, count);
        }
        else
        {
                // This page table is not in memory
                if(page_bit[Location] == -1)
                {
                        while(1)
                        {
                                int LRE_bit = count % 5;
                                if(ref_bit[LRE_bit] == 0)
                                {
                                        int page_out = memory[LRE_bit];
                                        memory[LRE_bit] = Location;
                                        page_bit[page_out] = -1;
                                        page_bit[Location] = LRE_bit;
                                        pagetable[Location] = Value;

                                        printf("Page %d is paged out from frame %d | ", page_out, LRE_bit);
                                        printf("Frame %d is empty | ", LRE_bit);
                                        printf("Page %d is loaded into frame %d\n", Location, LRE_bit);

                                        break;
                                }
                                else
                                {
                                        ref_bit[LRE_bit] = 0;
                                        count++;
                                }
                        }
                }
                else
                {
                        pagetable[Location] = Value;
                        ref_bit[page_bit[Location]] = 1;

                        printf("Page %d is in frame %d\n", Location, page_bit[Location]);

                        count--;
                }
        }
        count++;
};
/* YOU need to implement rd_mem here. */
int rd_mem(int Location)
{
        int val;
        int temp = count % 5;

        if(page_bit[Location] == -1)
        {
                while(1)
                {
                        int LRE_bit = count % 5;

                        if(ref_bit[LRE_bit] == 0)
                        {
                                int page_out = memory[LRE_bit];
                                memory[LRE_bit] = Location;
                                page_bit[page_out] = -1;
                                page_bit[Location] = LRE_bit;
                                val = pagetable[Location];

                                printf("Page %d is paged out from frame %d | ", page_out, LRE_bit);
                                printf("Frame %d is empty | ", LRE_bit);
                                printf("Page %d is loaded into frame %d\n", Location, LRE_bit);
                                break;
                        }
                        else
                        {
                                ref_bit[LRE_bit] = 0;
                                count++;
                        }
                }
        }
        else
        {
                val = pagetable[Location];
                ref_bit[page_bit[Location]] = 1;

                printf("Page %d is in frame %d\n", Location, page_bit[Location]);
                count--;
        }
        count++;

        return val;
};
