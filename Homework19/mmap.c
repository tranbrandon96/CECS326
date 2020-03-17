#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>

int main()
{
        int fd;
        char *filePtr;
        struct stat buf;

        stat("mmap.txt", &buf);

        fd = open("mmap.txt", O_RDWR);
        filePtr = mmap(0, buf.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

        int position;
        char letter;

        for(int i = 0; i < 3; i++)
        {
                printf("Position in the file: ");
                scanf("%d", &position);

                if(position < 0 || position > buf.st_size)
                {
                        printf("Out of bound\n");
                        i--;
                        continue;
                }

                printf("New letter for position %d: ", position);
                scanf(" %c", &letter);

                filePtr[position] = letter;
        }

        munmap(filePtr, buf.st_size);
        close(fd);
        return 0;
}
