#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int SurfaceAreaVolumeCalculator(float *radius, float *height, float *volume, float *surfaceArea); //function prototype

int main()
{
    float r, h, v, sA;
    printf("r = ");
    scanf("%f", &r);
    printf("h = ");
    scanf("%f", &h);

    SurfaceAreaVolumeCalculator(&r, &h, &v, &sA); //call function
    printf("Volume = %.3f and Surface area = %.3f", v, sA);
}

int SurfaceAreaVolumeCalculator(float *radius, float *height, float *volume, float *surfaceArea)
{
    float r = *radius;
    float h = *height;

    *volume = M_PI * (r*r) * (h/3.0);
    *surfaceArea = M_PI * r* (r + sqrt((r*r) + (h*h)));
    
    return 0;
}