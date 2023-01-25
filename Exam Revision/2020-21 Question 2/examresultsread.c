#include <stdio.h>
#include <stdlib.h>

struct Student
{
    char firstName[20];
    char surname[30];
    int mark;
};

int main()
{

    FILE *fResults;
    fResults = fopen("data.txt", "r");

    struct Student TestResults;

    while(!feof(fResults))
    {
        fscanf(fResults, "%s %s %d", TestResults.firstName, TestResults.surname, &TestResults.mark);
        if (TestResults.mark >= 70)
        {
            printf("%s %s: First-Class Degree\n", TestResults.firstName, TestResults.surname);
        }

        else if ((TestResults.mark >= 60) && (TestResults.mark < 70))
        {
            printf("%s %s: Upper Second-Class Degree\n", TestResults.firstName, TestResults.surname);
        }

        else if ((TestResults.mark >=  50) && (TestResults.mark < 60))
        {
            printf("%s %s: Lower Second-Class Degree\n", TestResults.firstName, TestResults.surname);
        }

        else if ((TestResults.mark >=  40) && (TestResults.mark < 50))
        {
            printf("%s %s: Third-Class Degree\n", TestResults.firstName, TestResults.surname);
        }

        else if (TestResults.mark < 40)
        {
            printf("%s %s: Fail\n", TestResults.firstName, TestResults.surname);
        }
    }
}