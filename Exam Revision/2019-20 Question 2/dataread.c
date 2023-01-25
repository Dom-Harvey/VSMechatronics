#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>

int main()
{
    FILE *fExamData;
    
    fExamData = fopen("data.txt", "r");

    if (fExamData == NULL)
    {
        printf("File failed to open");
    }

    int readValue = 0;
    int minimum = 0;
    int maximum = 0;
    float mean = 0;
    int runningTot;
    int totalNumberRead = 0;

    fscanf(fExamData, "%*s %d", &readValue);
    while (!feof(fExamData))
    {
        fscanf(fExamData, "%*s %d \n", &readValue);
        fscanf(fExamData, "%*s %d \n", &readValue); //ignore the next \n
        printf("Value Read is %d\n", readValue);
        totalNumberRead++;
        runningTot += readValue;

        if (minimum == 0)
        {
            minimum = readValue;
        }
        if (maximum == 0)
        {
            maximum = readValue;
        }

        if (minimum > readValue)
        {
            minimum = readValue;
        }
        if (maximum < readValue)
        {
            maximum = readValue;
        }
    }

    mean = runningTot / totalNumberRead;

    printf("Maximum = %d\nMinimum = %d\nMean = %f", maximum, minimum, mean);
}