/* Chapter 3 Exercises START
#include <stdlib.h>
#include <stdio.h>

//my first program
int main()
{
    //print some text
    printf("Hello, World!\nThis is my C program\nI hate doing this");
    return(0);
}

Chapter 3 Exercises */

/* Chapter 5 Exercises START */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/*int main()
{
    //declare some variables
    int a = 1, b = 2;
    float f = 1.23f;
    char c = 'H';

    //print the variables
    printf("The variables are:\na = %d\nb = %d\nf = %.2f\nAnd my character is ""%c""\n\n", a, b, f, c);
    
    //exit from main
    return(0);
}*/

int main()
{
    //declare variables
    int r = 5;
    float vol = 0.0f;

    vol = (4* M_PI * (float)r *(float)r) / 3;

    printf("Volume = %.2f", vol);
    return(0);
    
}