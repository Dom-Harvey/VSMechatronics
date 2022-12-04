#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//KEY:
// //       Real comment
// //check  Temporary comment
// //***    Missing/Unfinished Code
// //err    Error in Code
// //F      Flag term

struct ShapeCoordSystem
{
    char name[20];
    int sides;
    int x[100],y[100], z[100];

    //*** DO DYNAMIC ALLOCATION
    //*** MAKE Z BOOLEAN
};

struct CommandArray
{
    int numberOfCommands;
    int xGrid[9], yGrid[9], shapeMarker[9];
    char shape[20];
};

int ReadShapeDataFunction(struct ShapeCoordSystem *);
int DrawShapesFunction(struct CommandArray *);
int GCodeGenerationFunction(struct CommandArray *, struct ShapeCoordSystem *);

int main(void)
{
    // Define ShapeCoordSystem for shapes square, invTri, star, raTri, cross;
    struct ShapeCoordSystem shapes[5]; //*** DYNAMIC ALLOCATION
    // Define CommandArray
    struct CommandArray commandArray[1]; //err ONLY 1 - DON'T NEED ARRAY

    // Execute ReadShapeDataFunction
    printf("Extracting Shape Data from file...\n");
    int readShapeDataOutput;
    readShapeDataOutput = ReadShapeDataFunction(shapes);

    // Check if Shape Data was read successfully
    if (readShapeDataOutput == 1)
    {
        printf("Shape Data Extracted Successfully\n");
    }
    else
    {
        printf("Shape Data not extracted successfully\n");
        return 1;
    }
    // Execute DrawShapesFunction
    printf("\nExtracting Drawing Commands from file...\n");
    int drawShapeFunctionOutput;
    drawShapeFunctionOutput = DrawShapesFunction(commandArray);

    // Check if Shape Draw Data was read successfully
    if (drawShapeFunctionOutput == 1)
    {
        printf("Drawing Instructions extracted Successfully\n");
    }
    else
    {
        printf("Drawing Instructions not extracted successfully\n");
        return 1;
    }

    // Execute GCodeGenerationFunction
    int gCodeOutput;
    gCodeOutput =  GCodeGenerationFunction(commandArray, shapes);

    // Check if G-Code was generated successfully
    if (gCodeOutput == 1)
    {
        printf("G-Code generated Successfully\n");
    }
    else
    {
        printf("G-Code not generated successfully\n");
        return 1;
    }

    return 1;
}

int ReadShapeDataFunction(struct ShapeCoordSystem *ptr1)
{
    // Open file
    FILE *fShapeData;
    fShapeData = fopen ("ShapeStrokeData.txt", "r");
    
    // Check file is opened successfully
    if (fShapeData == NULL)
    {
        printf("File opening unsuccessful\n");
        return 2;
    }

    // Reads total number of shapes
    int totNumShapes;        
    fscanf(fShapeData, "%*s %d", &totNumShapes);

    //*** ERROR CHECK - MORE SHAPES THAN EXPECTED? //err FIND WAY TO MAKE NUMBER OF SHAPES IRRELEVENT

    // Reads the next line to get a shape name and number of sides
    int j = 0;
    for (j = 0; (j < totNumShapes); j++)
    {
        char nameOfShape[20];
        int shapeNumSides;

        fscanf(fShapeData, "%s %d", nameOfShape, &shapeNumSides);
        ptr1->sides = shapeNumSides;
        //F printf("%s %d\n", nameOfShape, shapeNumSides);

        //*** ERROR CHECK - NO NAME


        //*** ERROR CHECK - NO NUMBER OF SIDES?


        // Populates a struct for each shape
        strcpy(ptr1->name, nameOfShape);
        int i = 0;
        for (i = 0; (i < shapeNumSides); i++)
        {
            fscanf(fShapeData, "%d %d %d", &ptr1->x[i], &ptr1->y[i], &ptr1->z[i]);
            //F printf("%d %d %d\n", ptr1->x[i], ptr1->y[i], ptr1->z[i]);
            //*** OFFSET

        }
        ptr1++;
    }
    // Closes the shape Data and checks that the file was closed successfully
    fclose(fShapeData);

    if (fShapeData == NULL)
    {
        printf("File not closed successfully");
    }

    return 2;
}

int DrawShapesFunction(struct CommandArray *ptr2)
{
    //*** OPEN FILE
    FILE *fDrawShapes;
    fDrawShapes = fopen ("DrawShapes.txt", "r");
    
    //*** CHECK FILE WAS OPENED
    if (fDrawShapes == NULL)
    {
        printf("File opening unsuccessful\n");
        return 2;
    }

    //*** GAIN SIZE OF FILE
    int counter = 0;
    char c;
    for(c = getc(fDrawShapes); c != EOF; c = getc(fDrawShapes))
    {
        if (c == '\n')
        {
            counter++;
        }
    }
    rewind(fDrawShapes);

    //*** SKIP FIRST LINE IF GRID
    char firstLine[20];
    fscanf(fDrawShapes, "%s %*d", firstLine);

    if (strcmp(firstLine, "DRAW_GRID") == 0)
    {
        counter--;
    }

    //*** ERROR CHECK - MORE DRAW COMMANDS THAN GRID SPACE?
    int totalSpace = 9;
    if (totalSpace < counter)
    {
        printf("Too many commands issued for grid space, limiting number of draw commands to %d", counter);
        counter = totalSpace;
    }

    ptr2->numberOfCommands = counter;

    //*** POPULATE COMMAND ARRAY
    int i = 0;
    for (i = 0; (i <= counter); i++)
    {
        fscanf(fDrawShapes, "%d %d %s", &ptr2->xGrid[i], &ptr2->yGrid[i], ptr2->shape);
        if (strcmp(ptr2->shape, "SQUARE") == 0)
        {
            ptr2->shapeMarker[i] = 0;
        }
        else if (strcmp(ptr2->shape, "INVERTED_TRIANGLE") == 0)
        {
            ptr2->shapeMarker[i] = 1;
        }
        else if (strcmp(ptr2->shape, "STAR") == 0)
        {
            ptr2->shapeMarker[i] = 2;
        }
        else if (strcmp(ptr2->shape, "RIGHT_ANGLE_TRIANGLE") == 0)
        {
            ptr2->shapeMarker[i] = 3;
        }
        else if (strcmp(ptr2->shape, "CROSS") == 0)
        {
            ptr2->shapeMarker[i] = 4;
        }

        //F printf("%d %d %s\n", ptr2->xGrid[i], ptr2->yGrid[i], ptr2->shape);
        //F printf("%d %d %d\n", ptr2->xGrid[i], ptr2->yGrid[i], ptr2->shapeMarker[i]);
    }

    //*** CHECK FILE WAS CLOSED
    if (fDrawShapes == NULL)
    {
        printf("File not closed successfully");
        return 2;
    }


    return 1;
}

int GCodeGenerationFunction(struct CommandArray *ptrMain, struct ShapeCoordSystem *ptrShape)
{
    //F printf("%s", ptrShape[1].name);
    //*** OPEN GCODE FILE
    FILE *fGCode;
    fGCode = fopen("GCode.txt", "w");

    //*** CHECK GCODE OPENED
    if (fGCode == NULL)
    {
        printf("File opening unsuccessfull");
        return 2;
    }

    //*** INITIALISE
    int penSpeed = 1000;
    int spindle = 3;
    int penUp = 0;

    fprintf(fGCode, "F%d M%d S%d", penSpeed, spindle, penUp);
    
    //*** DRAW GRID
    int gridSquare = 90;
    int numRowsColumns = 3;
    //check zero pen
    fprintf(fGCode, "\nG0 X0 Y0\n");
    //check draw square
    fprintf(fGCode, "\nS1000\nG1 X%d Y0\nG1 X%d Y%d\nG1 X0 Y%d\nG0 X0 Y0\n", gridSquare, gridSquare, -gridSquare, -gridSquare);
    //check draw columns
    fprintf(fGCode, "\nS0\nG0 X%d Y0\nS1000\nG1 X%d Y%d\nS0\nG0 X%d Y0\nS1000\nG1 X%d Y%d\n", gridSquare/numRowsColumns, gridSquare/numRowsColumns, -gridSquare, 2*gridSquare/numRowsColumns, 2*gridSquare/numRowsColumns, -gridSquare);
    //check draw rows
    fprintf(fGCode, "\nS0\nG0 X0 Y%d\nS1000\nG1 X%d Y%d\nS0\nG0 X0 Y%d\nS100\nG1 X%d Y%d\n", -gridSquare/numRowsColumns, gridSquare, -gridSquare/numRowsColumns, -2*gridSquare/numRowsColumns, gridSquare, -2*gridSquare/numRowsColumns);

    //*** GET LOCATION FROM COMMAND ARRAY
    int i = 0;
    int iMax = ptrMain->numberOfCommands;
    int j = 0;
    int occupiedArray[numRowsColumns][numRowsColumns];
    float newXZero;
    float newYZero;
    int toDrawShape;
    int relocateFlag;
    float scalar = 30/20;
    float offset = 7.5;

    for (i = 0; (i <= iMax); i++)
    {
        int xGridLoc = ptrMain->xGrid[i];
        int yGridLoc = ptrMain->yGrid[i];

        newXZero = (xGridLoc-1) * gridSquare/numRowsColumns + offset;
        newYZero = - yGridLoc * gridSquare/numRowsColumns + offset;
        toDrawShape = ptrMain->shapeMarker[i];

        //*** ERROR CHECK - LOCATION OCCUPIED?
        if (occupiedArray[ptrMain->xGrid[i]][ptrMain->yGrid[i]] == 1)
        {
            relocateFlag = 1;
        }
        
        while (relocateFlag == 1)
        {
            printf("\nLocation occupied! Give new coordinates");
            printf("\nX Coord: ");
            scanf("%d", &xGridLoc);
            printf("\nY Coord: ");
            scanf("%d", &yGridLoc);
            //*** ERROR CHECK - INPUTS VALID?

            if (occupiedArray[xGridLoc][yGridLoc] != 1)
            {
                relocateFlag = 0;
            }
        }

        // Zero pen at desired grid space
        fprintf(fGCode,"\nS0\nG0 X%.2f Y%.2f\n", newXZero, newYZero);
        // Generate sequence for current shape
        for (j = 0; (j < ptrShape[toDrawShape].sides); j++)
        {
            fprintf(fGCode, "G%d X%0.2f Y%.2f\n", ptrShape[toDrawShape].z[j], newXZero + scalar*ptrShape[toDrawShape].x[j], newYZero + scalar*ptrShape[toDrawShape].y[j]);
        }
        // Marks location as occupied once full shape command sequence is formed
        occupiedArray[xGridLoc][yGridLoc] = 1;
    }
    // Zeroes pen now that sequence is complete
    fprintf(fGCode, "\nG0 X0 Y0");

    // Closes G-Code
    fclose(fGCode);

    // Checs G-Code is closed
    if (fGCode == NULL)
    {
        printf("File not closed successfully");
    }

    return 2;
}