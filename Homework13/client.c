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

        result = mkfifo("client.pipe", 0600);

        for(int i = 0; i < 5; i++)
        {
                fd = open("server.pipe", O_WRONLY);

                printf("Multiplicand: ");
                scanf("%d", &multiplicand);
                write(fd, &multiplicand, sizeof(multiplicand));
                printf("Multiplier: ");
                scanf("%d", &multiplier);
                write(fd, &multiplier, sizeof(multiplier));
                close(fd);
                fd = open("client.pipe", O_RDONLY);
                read(fd, &product, sizeof(product));
                close(fd);
                printf("%d X %d = %d\n\n", multiplier, multiplicand, product);
        }

        return 0;
}