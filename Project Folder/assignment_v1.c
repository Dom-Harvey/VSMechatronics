#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//KEY:
// //       Real comment
// //check  Temporary comment
// //***    Missing/Unfinished Code
// //err    Error in Code

int main()
{
    struct ShapeCoordSystem square, invTri, star, raTri, cross;
    {
        int numSides;
        float x[numSides],y[numSides];
        int z[numSides];
    }

    //check EXECUTE READ SHAPE DATA
    ReadShapeDataFunction();

    //*** ERROR CHECK
    //*** EXECUTE READ DRAW DATA
    //*** ERROR CHECK
    //*** EXECUTE SEND GCODE
    //*** ERROR CHECK

    return 1;
}

int ReadShapeDataFunction()
{
    int totNumShapes = 0;
    //*** OPEN FILE
    FILE *fShapeData;

    fShapeData = fopen ("ShapeStrokeData.txt", "r");
    
    //*** ERROR CHECK - FILE OPEN?

    if (fShapeData == NULL)
    {
        printf("File opened unsuccessfully");
        return -1;
    }

    //*** GAIN TOTAL NUMBER OF SHAPES    

    fscanf(fShapeData, "%*s %d", &totNumShapes);


    //*** ERROR CHECK - MORE SHAPES THAN EXPECTED? //err FIND WAY TO MAKE NUMBER OF SHAPES IRRELEVENT


    //*** READ SHAPE NAME AND NUMBER OF SIDES
    char nameOfShape[20];
    int shapeNumSides;

    fscanf(fShapeData, "%s %d", nameOfShape, &shapeNumSides);
    printf("%s", nameOfShape);

    //*** ERROR CHECK - NO NAME


    //*** ERROR CHECK - NO NUMBER OF SIDES?


    //*** PopulateShapeArrayFunction() //err USE STRUCTS?
    if (strcmp(nameOfShape, "SQUARE"))
    {
        printf("Hello World");
        //square.numSides = shapeNumSides;
        //int i;
        //for (i = 0; (i <= shapeNumSides); i++)
        //{
        //    fscanf(fShapeData, "%d %d %d", square.x[i], square.y[i], square.z[i]);
        //}
        //printf("%s", square.x);
    }


    //*** CHECK FOR END OF FILE


    //*** CLOSE FILE


    //*** ERROR CHECK - FILE CLOSED?

    return 1;
}