#include <stdio.h>
#include <stdlib.h>

int RecursiveFunction(int x)
{
    while (x <= 10)
    {
        printf('%d', x);
        x = x + 1;
        RecursiveFunction(x);
    }
    
    return 1;
}

int main()
{
    int x = 1;
    RecursiveFunction(x);
    
}

