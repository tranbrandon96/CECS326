#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
int main (int argc, char* argv[])
{

  int src_fd;
  int dest_fd;
  char filebuf[256]; /* a small buffer used to copy the file */
  ssize_t readSize;

  printf("uid: %d euid %d\n", getuid(), geteuid());
  src_fd = open(argv[1],O_RDONLY);
  dest_fd = open("/home/012746628/xfer.tmp", O_WRONLY | O_CREAT , S_IRUSR | S_IWUSR);

//if (dest_fd < 0)
//{
// dest_fd = open(argv[2],O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
//}
//else
//{
//printf("File is already created");
//}

  while (( readSize = read(src_fd, filebuf, sizeof(filebuf)-1)) > 0)
  {
    // I turn it into a string only because I'm using printf;
   // making a copy using "write" would not need this.
    filebuf[readSize] = '\0';
    printf("%s",filebuf);
    write(dest_fd, filebuf, readSize);
  }
  close(src_fd);
  close(dest_fd);
  return 0;
}
