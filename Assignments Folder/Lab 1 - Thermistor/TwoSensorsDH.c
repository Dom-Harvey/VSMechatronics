#include <stdio.h>
#include <math.h>

float NISTdegCtoMilliVoltsKtype(float tempDegC)
/* returns EMF in millivolts */
{
    int i;
    float milliVolts = 0;
    if(tempDegC >= -170 && tempDegC < 0)
    {
        const float coeffs[11] =
        {
            0.000000000000E+00,
            0.394501280250E-01,
            0.236223735980E-04,
            -0.328589067840E-06,
            -0.499048287770E-08,
            -0.675090591730E-10,
            -0.574103274280E-12,
            -0.310888728940E-14,
            -0.104516093650E-16,
            -0.198892668780E-19,
            -0.163226974860E-22
        };
        for (i=0; i<=10; i++)
        {
            milliVolts += coeffs[i] * pow(tempDegC,i);
        }
    }
    else if(tempDegC >= 0 && tempDegC <= 1372)
    {
        const float coeffs[10] =
        {
            -0.176004136860E-01,
            0.389212049750E-01,
            0.185587700320E-04,
            -0.994575928740E-07,
            0.318409457190E-09,
            -0.560728448890E-12,
            0.560750590590E-15,
            -0.320207200030E-18,
            0.971511471520E-22,
            -0.121047212750E-25
        };
        const float a0 =  0.118597600000E+00;
        const float a1 = -0.118343200000E-03;
        const float a2 =  0.126968600000E+03;

        for (i=0; i<=9; i++)
        {
            milliVolts += coeffs[i] * pow(tempDegC,i);
        }

        milliVolts += a0*exp(a1*(tempDegC - a2)*(tempDegC - a2));
    }
    else
    {
        milliVolts = 99E99;
    }
    return milliVolts;
}

float NISTmilliVoltsToDegCKtype(float tcEMFmV)
// returns temperature in deg C.
{

        int i, j;
        float tempDegC = 0;
        const float coeffs[11][3] =
        {
          {0.0000000E+00,  0.000000E+00, -1.318058E+02},
         {2.5173462E+01,  2.508355E+01,  4.830222E+01},
         {-1.1662878E+00,  7.860106E-02, -1.646031E+00},
         {-1.0833638E+00, -2.503131E-01,  5.464731E-02},
         {-8.9773540E-01,  8.315270E-02, -9.650715E-04},
         {-3.7342377E-01, -1.228034E-02,  8.802193E-06},
         {-8.6632643E-02,  9.804036E-04, -3.110810E-08},
         {-1.0450598E-02, -4.413030E-05,  0.000000E+00},
         {-5.1920577E-04,  1.057734E-06,  0.000000E+00},
         {0.0000000E+00, -1.052755E-08,  0.000000E+00}
       };
       if(tcEMFmV >=-5.891 && tcEMFmV <=0 )
       {
           j=0;
       }
       else if (tcEMFmV > 0 && tcEMFmV <=20.644  )
       {
           j=1;
       }
       else if (tcEMFmV > 20.644 && tcEMFmV <=54.886  )
       {
           j=2;
       }
       else
       {
           return 99E9;
       }

       for (i=0; i<=9; i++)
        {
            tempDegC += coeffs[i][j] * pow(tcEMFmV,i);
        }
    return tempDegC;
}

/* Write a function here to convert ADC value to voltages. (Part a, equation 1)
Call it from the main() function below */
float ADCToVolt(int vRef, float nADC)
{
    float vADC;                                                                         // Create the voltage ADC variable as a float

    vADC = (float)vRef * nADC / 1024.00;                                               // Define the voltage ADC variable as per lab sheet equation
    return vADC;
}

/* Write a function to convert degrees K to degrees C  (Part b, (iv))
Call it from the main() function below */
float KelvinToCelsius(float tempK)
{
    float tempC = tempK - 273.15;                                                       // Define the temperature in celsius variable, and set it to kelvin + 273.15
    return tempC;
}

int main()
{
    // Voltage value set in Volts
    const int vRef = 5;                                                                 // Define the reference voltage in Volts, as a constant

    // Define Thermistor constants
    const float temp0 = 298.15;                                                         // Define temperature-0 in Kelvin, as a constant
    const int res0 = 10;                                                                // Define resistance-0 in kilo ohms, as a constant
    const int thermB = 3975;                                                            // Define logarithmic constant B in Kelvin

    // User input for one pin value to test all outputs
    int nADC;                                                                           // Create the variable for the ADC integer value
    int flag = 0;                                                                       // Create a flag variable for loop checks

    while (flag == 0)
    {
        printf("\nUser input of ADC integer value between 0 and 1023 = ");
        scanf("%d", &nADC);                                                             // Define and display nADC for check

        if (nADC > 1023 || nADC < 0)
        {
            printf("\n\n\n\n\nInteger value must be between 0 and 1023. Please reenter\n");
        }

        else
        {
            flag = 1;
        }
    }

    // Calculate thermistor temperature in degrees C ( Part b, i,ii,iii & v)
    float vADC = ADCToVolt(vRef, nADC);                                                 // Define voltage from ADC
    float thermal_res;                                                                  // Create thermal resistance
    float thermal_tempK;                                                                // Create temperature in Kelvin
    float thermal_tempC;                                                                // Create temperature in celsius

    thermal_res = (10 * 3.3 / vADC) - 10;                                               // Define thermal resistance in kilo ohms
    thermal_tempK = pow((1/temp0 + 1/thermB * log(thermal_res/res0)), -1);              // Calculate the temperature in kelvin
    thermal_tempC = KelvinToCelsius(thermal_tempK);                                     // Convert kelvin to celsius

    // Calculate thermocouple temperature in degrees C ( Part c, i - iv)
    float vCouple;                                                                      // Create voltage of the thermocouple
    float vComp;                                                                        // Create compensation voltage
    float vTotal;                                                                       // Create a variable for the summation of the two previous voltages
    float couple_tempC;                                                                 // Create temperature for the thermocouple

    vCouple = (vADC - 0.35)/54.4;                                                       // Define voltage of the thermocouple
    vComp = NISTdegCtoMilliVoltsKtype(thermal_tempC);                                   // Define the compensation voltage
    vTotal = vCouple*1000 + vComp;                                                      // Define the total voltage while converting the thermocouple voltage in mV
    couple_tempC = NISTmilliVoltsToDegCKtype(vTotal);                                   // Define the thermocouple temp

    // Output results
    printf("\n\nThermistor temperature (deg C): %.2f \n", thermal_tempC);
    printf("\nThermocouple temperature with CJC (deg C): %.2f \n", couple_tempC);

    return 0;
}