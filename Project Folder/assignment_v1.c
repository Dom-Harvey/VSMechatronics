#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//KEY:
// //       Real comment
// //check  Temporary comment
// //***    Missing/Unfinished Code
// //err    Error in Code

struct ShapeCoordSystem
    {
        char name[20];
        int x[100],y[100];
        int z[100];

        //*** DO DYNAMICALLY ALLOCATING
    };

int ReadShapeDataFunction(struct ShapeCoordSystem *);

int main(void)
{
    struct ShapeCoordSystem shapes[5]; // square, invTri, star, raTri, cross;

    //check EXECUTE READ SHAPE DATA
    printf("Extracting Data from file...\n");
    int readShapeDataOutput;
    readShapeDataOutput = ReadShapeDataFunction(shapes);

    //*** ERROR CHECK
    if (readShapeDataOutput == 1)
    {
        printf("Data Extracted Successfully");
    }
    else
    {
        printf("Data not extracted successfully");
        return 1;
    }
    //*** EXECUTE READ DRAW DATA

    //*** ERROR CHECK

    //*** EXECUTE SEND GCODE

    //*** ERROR CHECK


    return 1;
}

int ReadShapeDataFunction(struct ShapeCoordSystem *ptr)
{
    // Open file
    FILE *fShapeData;
    fShapeData = fopen ("ShapeStrokeData.txt", "r");
    
    // Check file is opened successfully
    if (fShapeData == NULL)
    {
        printf("File opening unsuccessful\n");
        return 1;
    }

    // Reads total number of shapes
    int totNumShapes;        
    fscanf(fShapeData, "%*s %d", &totNumShapes);

    //*** ERROR CHECK - MORE SHAPES THAN EXPECTED? //err FIND WAY TO MAKE NUMBER OF SHAPES IRRELEVENT

    // Reads the next line to get a shape name and number of sides
    int j;
    for (j = 0; (j < totNumShapes); j++)
    {
        char nameOfShape[20];
        int shapeNumSides;

        fscanf(fShapeData, "%s %d", nameOfShape, &shapeNumSides);

        //*** ERROR CHECK - NO NAME


        //*** ERROR CHECK - NO NUMBER OF SIDES?


        // Populates a struct for each shape
        strcpy(ptr->name, nameOfShape);
        int i;
        for (i = 0; (i < shapeNumSides); i++)
        {
            fscanf(fShapeData, "%d %d %d", &ptr->x[i], &ptr->y[i], &ptr->z[i]);
        }
        ptr++;
    }
    // Closes the struct and checks that the file was closed successfully
    fclose(fShapeData);

    if (fShapeData == NULL)
    {
        printf("File not closed successfully");
    }

    return 1;
}