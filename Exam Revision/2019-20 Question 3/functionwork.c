#include <stdio.h>
#include <stdlib.h>

void TimePeriodConversion(int timeInput, int *h, int *m, int *s); //function prototype

int main()
{
    int timeInput;
    int h;
    int m;
    int s;

    printf("Please input your time period in seconds: ");
    scanf("%d", &timeInput);
    TimePeriodConversion(timeInput, &h, &m, &s);
    printf("%d seconds is equal to %d hours, %d minutes and %d seconds", timeInput, h, m, s);
    return 1;
}

void TimePeriodConversion(int timeInput, int *h, int *m, int *s)
{
    int tS = timeInput;

    *h = (tS)/3600;
    *m = (tS - *h * 3600)/60;
    *s = (tS) - ((*h*3600)+(*m*60));
}