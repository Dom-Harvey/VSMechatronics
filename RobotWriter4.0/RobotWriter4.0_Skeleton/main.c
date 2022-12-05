#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include "rs232.h"
#include "serial.h"

#define bdrate 115200               /* 115200 baud */

void SendCommands (char *buffer );

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
    int drawFlag;
    int xGrid[9], yGrid[9], shapeMarker[9];
    char shape[20];
};

int ReadShapeDataFunction(struct ShapeCoordSystem *);
int DrawShapesFunction(struct CommandArray *);
int GCodeGenerationFunction(char buffer[100], struct CommandArray *, struct ShapeCoordSystem *);

int main()
{
    // Define ShapeCoordSystem for shapes square, invTri, star, raTri, cross;
    struct ShapeCoordSystem shapes[6]; //*** DYNAMIC ALLOCATION
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


    char buffer[100];
 
    // If we cannot open the port then give up immediatly
    if ( CanRS232PortBeOpened() == -1 )
    {
        printf ("\nUnable to open the COM port (specified in serial.h) ");
        exit (0);
    }

    // Time to wake up the robot
    printf ("\nAbout to wake up the robot\n");

    // We do this by sending a new-line
    sprintf (buffer, "\n");
    // printf ("Buffer to send: %s", buffer); // For diagnostic purposes only, normally comment out
    PrintBuffer (&buffer[0]);
    Sleep(100);

    // This is a special case - we wait  until we see a dollar ($)
    WaitForDollar();

    printf ("\nThe robot is now ready to draw\n");

    //These commands get the robot into 'ready to draw mode' and need to be sent before any writing commands
    //sprintf (buffer, "G1 X0 Y0 F1000\n");

    // These are sample commands to draw out some information - these are the ones you will be generating.
    //sprintf (buffer, "G0 X-13.41849 Y0.000\n");
    // Execute GCodeGenerationFunction
    printf("\nGenerating G-Code...\n");
    int gCodeOutput;
    gCodeOutput =  GCodeGenerationFunction(buffer, commandArray, shapes);

    // Check if G-Code was generated successfully
    if (gCodeOutput == 1)
    {
        printf("G-Code generated and sent Successfully\n");
    }
    else
    {
        printf("G-Code not generated successfully\n");
        return 1;
    }
    
    

    // Before we exit the program we need to close the COM port
    CloseRS232Port();
    printf("Com port now closed\n");

    return (0);
}

// Send the data to the robot - note in 'PC' mode you need to hit space twice
// as the dummy 'WaitForReply' has a getch() within the function.
void SendCommands (char *buffer )
{
    // printf ("Buffer to send: %s", buffer); // For diagnostic purposes only, normally comment out
    PrintBuffer (&buffer[0]);
    WaitForReply();
    Sleep(100); // Can omit this when using the writing robot but has minimal effect
    // getch(); // Omit this once basic testing with emulator has taken place
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
        return 1;
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

    return 1;
}

int DrawShapesFunction(struct CommandArray *ptr2)
{
    //*** OPEN FILE
    FILE *fDrawShapes;
    char fileToOpen[50];
    printf("Please enter the file you would like to read in name.txt format: ");
    scanf("%s", fileToOpen);
    fDrawShapes = fopen(fileToOpen, "r");
    
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
    fscanf(fDrawShapes, "%s %d", firstLine, &ptr2->drawFlag);
    counter--;

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
        else if (strcmp(ptr2->shape, "CUSTOM_SHAPE") == 0)
        {
            ptr2->shapeMarker[i] = 5;
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

int GCodeGenerationFunction(char buffer[100], struct CommandArray *ptrMain, struct ShapeCoordSystem *ptrShape)
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

    //fprintf(fGCode, "F%d M%d S%d", penSpeed, spindle, penUp);
    sprintf(buffer, "F%d\nM%d\nS%d", penSpeed, spindle, penUp);
    SendCommands(buffer);
    
    //*** DRAW GRID
    int gridSquare = 90;
    int numRowsColumns = 3;
    if (ptrMain->drawFlag == 1)
    {
        //check zero pen
        //fprintf(fGCode, "\nG0 X0 Y0\n");
        sprintf(buffer, "\nG0 X0 Y0\n");
        SendCommands(buffer);
        //check draw square
        //fprintf(fGCode, "\nS1000\nG1 X%d Y0\nG1 X%d Y%d\nG1 X0 Y%d\nG0 X0 Y0\n", gridSquare, gridSquare, -gridSquare, -gridSquare);
        sprintf(buffer, "\nS1000\nG1 X%d Y0\nG1 X%d Y%d\nG1 X0 Y%d\nG1 X0 Y0\n", gridSquare, gridSquare, -gridSquare, -gridSquare);
        SendCommands(buffer);
        //check draw columns
        //fprintf(fGCode, "\nS0\nG0 X%d Y0\nS1000\nG1 X%d Y%d\nS0\nG0 X%d Y0\nS1000\nG1 X%d Y%d\n", gridSquare/numRowsColumns, gridSquare/numRowsColumns, -gridSquare, 2*gridSquare/numRowsColumns, 2*gridSquare/numRowsColumns, -gridSquare);
        sprintf(buffer, "\nS0\nG0 X%d Y0\nS1000\nG1 X%d Y%d\nS0\nG0 X%d Y0\nS1000\nG1 X%d Y%d\n", gridSquare/numRowsColumns, gridSquare/numRowsColumns, -gridSquare, 2*gridSquare/numRowsColumns, 2*gridSquare/numRowsColumns, -gridSquare);
        SendCommands(buffer);
        //check draw rows
        //fprintf(fGCode, "\nS0\nG0 X0 Y%d\nS1000\nG1 X%d Y%d\nS0\nG0 X0 Y%d\nS100\nG1 X%d Y%d\n", -gridSquare/numRowsColumns, gridSquare, -gridSquare/numRowsColumns, -2*gridSquare/numRowsColumns, gridSquare, -2*gridSquare/numRowsColumns);
        sprintf(buffer, "\nS0\nG0 X0 Y%d\nS1000\nG1 X%d Y%d\nS0\nG0 X0 Y%d\nS100\nG1 X%d Y%d\n", -gridSquare/numRowsColumns, gridSquare, -gridSquare/numRowsColumns, -2*gridSquare/numRowsColumns, gridSquare, -2*gridSquare/numRowsColumns);
        SendCommands(buffer);

    }
    //*** GET LOCATION FROM COMMAND ARRAY
    int i = 0;
    int iMax = ptrMain->numberOfCommands;
    int j = 0;
    int occupiedArray[numRowsColumns][numRowsColumns];
    memset(occupiedArray, 0, sizeof occupiedArray);
    float newXZero;
    float newYZero;
    int toDrawShape;
    int relocateFlag;
    float scalar = 20/16;
    float offset = 7.5;

    for (i = 0; (i <= iMax); i++)
    {
        int xGridLoc = ptrMain->xGrid[i];
        int yGridLoc = 4 - (ptrMain->yGrid[i]);

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
        //fprintf(fGCode,"\nS0\nG0 X%.2f Y%.2f\n", newXZero, newYZero);
        sprintf(buffer,"\nS0\nG0 X%.2f Y%.2f\n", newXZero, newYZero);
        SendCommands(buffer);
        // Generate sequence for current shape
        for (j = 0; (j < ptrShape[toDrawShape].sides); j++)
        {
            fprintf(fGCode, "\nS%d\nG%d X%0.2f Y%.2f\n", 1000*ptrShape[toDrawShape].z[j], ptrShape[toDrawShape].z[j], newXZero + scalar*ptrShape[toDrawShape].x[j], newYZero + scalar*ptrShape[toDrawShape].y[j]);
            sprintf(buffer, "\nS%d\nG%d X%0.2f Y%.2f\n", 1000*ptrShape[toDrawShape].z[j],  ptrShape[toDrawShape].z[j], newXZero + scalar*ptrShape[toDrawShape].x[j], newYZero + scalar*ptrShape[toDrawShape].y[j]);
            SendCommands(buffer);


        }
        // Marks location as occupied once full shape command sequence is formed
        occupiedArray[xGridLoc][yGridLoc] = 1;
    }
    // Zeroes pen now that sequence is complete
    fprintf(fGCode, "\nS0\nG0 X0 Y0\n");
    Sleep(5000);

    // Closes G-Code
    fclose(fGCode);

    // Checs G-Code is closed
    if (fGCode == NULL)
    {
        printf("File not closed successfully");
    }

    return 1;
}