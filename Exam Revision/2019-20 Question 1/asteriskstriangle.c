#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
//start time 11:23

int main()
{
    int width_height;
    int i;
    int j;
    
    printf("Please input the integer width/height value of your triangle: ");
    scanf("%d", &width_height);

    while ((width_height > 10) || (width_height < 1))
    {
        printf("Unable to draw triangles greater than size 10 or smaller than size 1. Please enter an integer value within that range: ");
        scanf("%d", &width_height);
    }

    for (i = 0; i < width_height; i++)
    {
        for (j = 0; j < i; j++)
        {
            printf("*");
        }
        printf("\n");
    }
}