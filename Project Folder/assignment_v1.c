#include <stdio.h>
#include <stdlib.h>

//KEY:
// // Real comment
// //*** Missing/Unfinished Code
// //err Error in Code

void main()
{
    //*** EXECUTE READ SHAPE DATA
    ReadShapeDataFunction();

    //*** ERROR CHECK
    //*** EXECUTE READ DRAW DATA
    //*** ERROR CHECK
    //*** EXECUTE SEND GCODE
    //*** ERROR CHECK
}

int ReadShapeDataFunction()
{
    int testString = 0;
    //*** OPEN FILE
    FILE *fShapeData;

    fShapeData = fopen ("ShapeStrokeData.txt", "r");

    if (fShapeData == NULL)
    {
        printf("File opened unsuccessfully");
        return -1;
    }

    fscanf(fShapeData, "%*s %d", &testString);
    printf("%d", testString);


    //*** ERROR CHECK - FILE OPEN?


    //*** GAIN TOTAL NUMBER OF SHAPES


    //*** ERROR CHECK - MORE SHAPES THAN EXPECTED? //err FIND WAY TO MAKE NUMBER OF SHAPES IRRELEVENT


    //*** READ SHAPE NAME AND NUMBER OF SIDES


    //*** ERROR CHECK - NO NAME


    //*** ERROR CHECK - NO NUMBER OF SIDES?


    //*** PopulateShapeArrayFunction() //err USE STRUCTS?


    //*** CHECK FOR END OF FILE


    //*** CLOSE FILE


    //*** ERROR CHECK - FILE CLOSED?


}