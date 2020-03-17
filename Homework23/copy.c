#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <openssl/evp.h>
#include <string.h>

int main (int argc, char* argv[]) {
  int src_fd;
  int dst_fd;
  char filebuf[64]; /* a small buffer used to copy the file */
  ssize_t readSize;

  EVP_MD_CTX md_context;
  const  EVP_MD* md = EVP_md5();
  unsigned int md_length;
  unsigned char md_value[EVP_MAX_MD_SIZE]; //64 bytes

  src_fd = open(argv[1],O_RDONLY);

  ssize_t writeSize;
   if(src_fd < 0) {
     printf("file not exist\n");
     return 0;
   }
   if(dst_fd = open(argv[2], O_RDONLY) >= 0) {
     printf("File already exist!\n");
  } else {
     dst_fd = open(argv[2], O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);
  }
  //initialize the digest
  EVP_DigestInit(&md_context, md);
  // Fill a block of memory with a 0 using memset()
  memset(filebuf, '0', sizeof(filebuf));

  while (( readSize = read(src_fd, filebuf, sizeof(filebuf)-1)) > 0)
  {
    write(dst_fd, filebuf,readSize);
    EVP_DigestUpdate(&md_context, filebuf, sizeof(filebuf));

  }
 //get the digest and its length
  EVP_DigestFinal(&md_context, md_value, &md_length);

  for(int i = 0; i < md_length; i++) {
     printf("%x", md_value[i]);
  }
  printf("\n");
  close(dst_fd);
  close(src_fd);
  return 0;
}