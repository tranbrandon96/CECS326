#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
        int result;
        int fd;
        int multiplicand = 1000;
        int multiplier = 1000;
        int product = 1000;

        result = mkfifo("server.pipe", 0600);

        while(1)
        {
                fd = open("server.pipe", O_RDONLY);

                if(read(fd, &multiplicand, sizeof(multiplicand)) > 0 && read(fd, &multiplier, sizeof(multiplier)) > 0)
                {
                        product = multiplier * multiplicand;
                        close(fd);
                        fd = open("client.pipe", O_WRONLY);
                        write(fd, &product, sizeof(product));
                        close(fd);
                }
                else
                {
                        exit(2);
                }
        }
        return 0;
}