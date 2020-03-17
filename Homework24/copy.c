#include <rpc/des_crypt.h>
#include <string.h>
#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
int main (int argc, char* argv[])
{
  int file1;
  int file2;

  char filebuf[256]; /* a small bu?????ed to copy the file */
  ssize_t readSize;

  file1 = open(argv[1],O_RDONLY);


if (file1 < 0)
{
  printf("File not found!\n");
  return 0;
}

if((file2 = open(argv[2], O_RDONLY)) >= 0)
{
  printf("File already exists!\n");
  return 0;
}

else
{
file2 = open(argv[2], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
}

char *key = "secretkey";
int datasize = 256;
unsigned int mode;
if (*argv[3] == 'e')
{
   mode = DES_ENCRYPT;
}
else
{
   mode = DES_DECRYPT;
}
  while (( readSize = read(file1, filebuf, sizeof(filebuf)-1)) > 0)
  {
    // I turn it into a string only because I'm using printf;
   // making a copy using "write" would not need this.
    ecb_crypt(key, filebuf, datasize, mode);
    write(file2, filebuf, readSize);
  }
  close(file1);
  close(file2);
  return 0;
}
