#include <stdio.h>
#include <stdlib.h>

int main()
{
    int height, i, j;

    printf("Input height: ");
    scanf("%d", &height);

    for (i = 0; i < height; i++)
    {
        for (j = 0; j < i; j++)
        {
            printf(" ");
        }
        printf("##########\n");
    }
}
