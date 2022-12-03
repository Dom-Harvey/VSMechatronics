#include <stdio.h>
#include <stdlib.h>

int PractiseFunction(char c)
{
    printf("%c", c);
    return 1;
}

int main()
{
    FILE *fOpenShapeStrokeData;
    fOpenShapeStrokeData = fopen("ShapeStrokeData.txt", "r");
    
    char c;
    char *pointer = &c;

    fscanf(fOpenShapeStrokeData, "%c\n", &c);
    printf("%c\n", c);
    printf("%c\n", *pointer);

    PractiseFunction(*pointer);

}