#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void SurfaceAreaCalculator(float *h, float *w, float *l, float *surfaceArea, float *volume);

int main()
{
    float h, w, l, surfaceArea, volume;
    printf("h = ");
    scanf("%f", &h);
    printf("w = ");
    scanf("%f", &w);
    printf("l = ");
    scanf("%f", &l);

    SurfaceAreaCalculator(&h, &w, &l, &surfaceArea, &volume);
    printf("Surface area = %.2f and volume = %0.2f", surfaceArea, volume);
}

void SurfaceAreaCalculator(float *h, float *w, float *l, float *surfaceArea, float *volume)
{
    float hF = *h;
    float wF = *w;
    float lF = *l;

    *surfaceArea = (lF * wF) + (lF * sqrt(pow((wF/2), 2) + pow(hF, 2))) + (wF * sqrt(pow((lF/2), 2) + pow(hF, 2)));
    *volume = (lF * wF * hF)/3;
}