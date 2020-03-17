#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
        int toParent[2];
        int toChild[2];
        int multiplicand;
        int multiplier;
        int product;
        int forkNum;

        pipe(toParent);
        pipe(toChild);

        forkNum = fork();

        for(int i = 0; i < 5; i++)
        {
                if(forkNum == 0)
                {
                        multiplicand = 1000;
                        multiplier = 1000;
                        close(toParent[0]);
                        close(toChild[1]);

                        if(read(toChild[0], &multiplicand, sizeof(multiplicand)) > 0 && read(toChild[0], & multiplier, sizeof(multiplier)) > 0)
                        {
                                product = multiplier * multiplicand;
                                write(toParent[1], &product, sizeof(product));
                        }
                        else
                        {
                                exit(2);
                        }
                }
                else
                {
                        product = 1000;
                        close(toChild[0]);
                        close(toParent[1]);
                        printf("Multiplicand: ");
                        scanf("%d", &multiplicand);
                        write(toChild[1], &multiplicand, sizeof(multiplicand));
                        printf("Multiplier: ");
                        scanf("%d", &multiplier);
                        write(toChild[1], &multiplier, sizeof(multiplicand));

                        if(argc > 1 && forkNum != 0)
                        {
                                wait3(NULL, 0, NULL);
                        }

                        read(toParent[0], &product, sizeof(product));
                        printf("%d X %d = %d\n\n", multiplier, multiplicand, product);
                }
        }

        return 0;
}
