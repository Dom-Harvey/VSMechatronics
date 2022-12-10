#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include "rs232.h"
#include "serial.h"

#define bdrate 115200               /* 115200 baud */

void SendCommands (char *buffer );

/* STRUCTURE FOR SHAPE COORDINATE SYSTEM */
struct ShapeCoordSystem
{
    char name[50];                  // Name of shape
    int instructions;               // Number of instructions to draw the shape
    int x[100],y[100], z[100];      // Array to hold each of the drawing instructions
};

/* STRUCTURE FOR COMMAND SEQUENCE */
struct CommandArray
{
    int numberOfCommands;                       // Number of commands within the drawing sequence file
    int drawFlag;                               // Flag to indicate whether or not to draw the grid
    int xGrid[9], yGrid[9], shapeMarker[9];     // arrays for grid coordinates - 9 for the 9 possible spaces
    char shape[20];                             // read in name of shape for later use with shape markers
};


/* FUNCTION PROTOTYPING */
int ReadShapeDataFunction(int totalShapes, struct ShapeCoordSystem *);                              // Hand total shapes, and the structure type pointer
int DrawShapesFunction(int totalShapes, struct CommandArray *);                                     // Hand total shapes, and the structure type pointer
int GCodeGenerationFunction(char buffer[100], struct CommandArray *, struct ShapeCoordSystem *);    // Hand the buffer and both structures

int main()
{
    /* Open file */
    FILE *fShapeData;
    fShapeData = fopen ("ShapeStrokeData.txt", "r");

    /* Reads total number of shapes and close file */
    int totNumShapes;        
    fscanf(fShapeData, "%*s %d", &totNumShapes);
    fclose(fShapeData);

    /* Define ShapeCoordSystem and CommandArray */
    struct ShapeCoordSystem shapes[totNumShapes];
    struct CommandArray commandArray[1];            // Only generating one, but leaving in functionality for more than one command array in the event of using multiple machines

    /* Execute function relating to reading in the shape data to memory */
    printf("Extracting Shape Data from file...\n");
    int readShapeDataOutput;
    readShapeDataOutput = ReadShapeDataFunction(totNumShapes, shapes);

    /* Check ShapeDataFunction output for error trapping */
    switch(readShapeDataOutput)
    {
        case 1:
        printf("Shape Data Extracted Successfully!\n");
        break;
        case 2:
        printf("Shape Data file not opened successfully\n");
        break;
        case 3:
        printf("Shape Data file not closed correctly\n");
        break;
        default:
        printf("WARNING! Unexpected behaviour in Read Shape Data Function\n");
    }

    /* Execute function relating to reading in the shape drawing instructions into the command array */
    printf("\nExtracting Drawing Commands from file...\n");
    int drawShapeOutput;
    drawShapeOutput = DrawShapesFunction(totNumShapes, commandArray);

    /* Check DrawShapesFunctino output for error trapping */
    switch(drawShapeOutput)
    {
        case 1:
        printf("Shape Sequence Extracted Successfully!\n");
        break;
        case 2:
        printf("Shape Sequence file not opened successfully\n");
        break;
        case 3:
        printf("Shape Data file not closed correctly\n");
        break;
        case 4:
        printf("Missing critical Shape Data in file. Check shape name format is SHAPE_NAME N");
        break;
        default:
        printf("WARNING! Unexpected behaviour in Read Shape Data Function\n");
    }


    char buffer[10000];
 
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

    /* Execute function relating to generating and sending G-Code */
    printf("\nGenerating G-Code...\n");
    int gCodeOutput;
    gCodeOutput =  GCodeGenerationFunction(buffer, commandArray, shapes);

    switch(gCodeOutput)
    {
        case 1:
        printf("G-Code generated and sent successfully\n");
        break;
        case 2:
        printf("G-Code not generated successfully\n");
        break;
        default:
        printf("WARNING! Unexpected behaviour in G-Code Generation Function\n");
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

int ReadShapeDataFunction(int totalShapes, struct ShapeCoordSystem *ptr1)
{
    /* Open shape data file for reading and check it is opened successfully, then skip over number
    of shapes because this was read in earlier*/
    FILE *fShapeData;
    fShapeData = fopen ("ShapeStrokeData.txt", "r");

    if (fShapeData == NULL)
    {
        return 2;
    }
    fscanf(fShapeData, "%*s %*d");

    /* Reads the next line to get a shape name and number of sides and returns an error if in the 
    incorrect format */
    int j = 0;
    for (j = 0; (j < totalShapes); j++)
    {
        char nameOfShape[20];
        int numInstructions = -1;   // Set intial number of instructions as -1 for error trapping (will never be -1)

        fscanf(fShapeData, "%s %d", nameOfShape, &numInstructions);
        ptr1->instructions = numInstructions;

        if (numInstructions == -1) // Indicates no change in number of instructions
        {
            return 4;
        }

        /* Enters a for loop equal to the number of instructions just read, to populate the struct
        with the x y and pen up/down info*/
        strcpy(ptr1->name, nameOfShape);
        int i = 0;
        for (i = 0; (i < numInstructions); i++)
        {
            fscanf(fShapeData, "%d %d %d", &ptr1->x[i], &ptr1->y[i], &ptr1->z[i]);
        }
        ptr1++; // Move to oopulating the next struct
    }

    /* Close shape data file for reading and check it is closed successfully */
    fclose(fShapeData);
    if (fShapeData == NULL)
    {
        return 3;
    }
    
    /* Function executed successfully if it reaches this point */
    return 1;
}

int DrawShapesFunction(int totalShapes, struct CommandArray *ptr2)
{
    /* Open user selected file  for reading and check it is opened successfullyr*/
    FILE *fDrawShapes;
    char fileToOpen[50];
    printf("Please enter the file you would like to read in name.txt format: ");
    scanf("%s", fileToOpen);
    fDrawShapes = fopen(fileToOpen, "r");
    
    if (fDrawShapes == NULL)
    {
        return 2;
    }

    /* Gain length of file and check if the grid needs to be drawn, then minus 1 from length of
    file to gain number of draw commands given */
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

    char firstLine[20];
    fscanf(fDrawShapes, "%s %d", firstLine, &ptr2->drawFlag);
    counter--;

    /* If more commands are given than grid spaces, the command array will only draw in the first 9*/
    int totalSpace = 9;
    if (totalSpace < counter)
    {
        printf("Too many commands issued for grid space, limiting number of draw commands to %d", counter);
        counter = totalSpace;
    }

    ptr2->numberOfCommands = counter;

    /* Populate the Command array with the x and y locations on the grid, and use the shape char to
    collect the name of the shape and convert it to a number so it can all occupy the same array*/
    int i = 0;
    int newMarker = totalShapes;
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
        else
        {
            printf("\nUnknown Shape detected! Allocating marker value of %d", newMarker);
            ptr2->shapeMarker[i] = newMarker;
            newMarker++;
        }
    }

    /* Close draw commands file for reading and check it is closed successfully */
    if (fDrawShapes == NULL)
    {
        return 3;
    }

    /* Function executed successfully if it reaches this point */
    return 1;
}

int GCodeGenerationFunction(char buffer[100], struct CommandArray *ptrMain, struct ShapeCoordSystem *ptrShape)
{
    /* Open G-Code file for writing to and check it was opened successfully. Any line relating to this can be
    commented out when sending commands to the robot. These lines are indicated by '//F '. */

    //F FILE *fGCode;
    //F fGCode = fopen("GCode.txt", "w");

 
    //F if (fGCode == NULL)
    //F {
    //F     return 2;
    //F }

    /* Initialise the pen properties */
    int penSpeed = 1000;
    int spindle = 3;
    int penUp = 0;

    //F fprintf(fGCode, "F%d M%d S%d", penSpeed, spindle, penUp);
    sprintf(buffer, "F%d\nM%d\nS%d", penSpeed, spindle, penUp);
    SendCommands(buffer);
    
    /* If the draw flag was given, draw the grid*/
    int gridSquare = 90;
    int numRowsColumns = 3;
    if (ptrMain->drawFlag == 1)
    {
        // zero pen
        //F fprintf(fGCode, "\nG0 X0 Y0\n");
        sprintf(buffer, "\nG0 X0 Y0\n");
        SendCommands(buffer);

        //check draw square
        //F fprintf(fGCode, "\nS1000\nG1 X%d Y0\nG1 X%d Y%d\nG1 X0 Y%d\nG0 X0 Y0\n", gridSquare, gridSquare, -gridSquare, -gridSquare);
        sprintf(buffer, "\nS1000\nG1 X%d Y0\nG1 X%d Y%d\nG1 X0 Y%d\nG1 X0 Y0\n", gridSquare, gridSquare, -gridSquare, -gridSquare);
        SendCommands(buffer);

        //check draw columns
        //F fprintf(fGCode, "\nS0\nG0 X%d Y0\nS1000\nG1 X%d Y%d\nS0\nG0 X%d Y0\nS1000\nG1 X%d Y%d\n", gridSquare/numRowsColumns, gridSquare/numRowsColumns, -gridSquare, 2*gridSquare/numRowsColumns, 2*gridSquare/numRowsColumns, -gridSquare);
        sprintf(buffer, "\nS0\nG0 X%d Y0\nS1000\nG1 X%d Y%d\nS0\nG0 X%d Y0\nS1000\nG1 X%d Y%d\n", gridSquare/numRowsColumns, gridSquare/numRowsColumns, -gridSquare, 2*gridSquare/numRowsColumns, 2*gridSquare/numRowsColumns, -gridSquare);
        SendCommands(buffer);

        //check draw rows
        //F fprintf(fGCode, "\nS0\nG0 X0 Y%d\nS1000\nG1 X%d Y%d\nS0\nG0 X0 Y%d\nS100\nG1 X%d Y%d\n", -gridSquare/numRowsColumns, gridSquare, -gridSquare/numRowsColumns, -2*gridSquare/numRowsColumns, gridSquare, -2*gridSquare/numRowsColumns);
        sprintf(buffer, "\nS0\nG0 X0 Y%d\nS1000\nG1 X%d Y%d\nS0\nG0 X0 Y%d\nS100\nG1 X%d Y%d\n", -gridSquare/numRowsColumns, gridSquare, -gridSquare/numRowsColumns, -2*gridSquare/numRowsColumns, gridSquare, -2*gridSquare/numRowsColumns);
        SendCommands(buffer);

    }

    /* Get locations and shape from command array, zero the pen at the desired location, and generate the
    sequence to be drawn for the shape, sending  each command to the buffer in a for loop */
    int i = 0;
    int iMax = ptrMain->numberOfCommands;
    int j = 0;
    float newXZero;
    float newYZero;
    int toDrawShape;
    float scalar = 20/16;
    float offset = 7.5;

    for (i = 0; (i <= iMax); i++)
    {
        int xGridLoc = ptrMain->xGrid[i];
        int yGridLoc = 4 - (ptrMain->yGrid[i]);

        newXZero = (xGridLoc-1) * gridSquare/numRowsColumns + offset;
        newYZero = - yGridLoc * gridSquare/numRowsColumns + offset;
        toDrawShape = ptrMain->shapeMarker[i];

        //fprintf(fGCode,"\nS0\nG0 X%.2f Y%.2f\n", newXZero, newYZero);
        sprintf(buffer,"\nS0\nG0 X%.2f Y%.2f\n", newXZero, newYZero);
        SendCommands(buffer);
        // Generate sequence for current shape
        for (j = 0; (j < ptrShape[toDrawShape].instructions); j++)
        {
            //fprintf(fGCode, "S%d\nG%d X%0.2f Y%.2f\n", 1000*ptrShape[toDrawShape].z[j], ptrShape[toDrawShape].z[j], newXZero + scalar*ptrShape[toDrawShape].x[j], newYZero + scalar*ptrShape[toDrawShape].y[j]);
            sprintf(buffer, "S%d\nG%d X%0.2f Y%.2f\n", 1000*ptrShape[toDrawShape].z[j],  ptrShape[toDrawShape].z[j], newXZero + scalar*ptrShape[toDrawShape].x[j], newYZero + scalar*ptrShape[toDrawShape].y[j]);
            SendCommands(buffer);
            Sleep(1000);
        }
    }
    /* Zero the pen once all drawings have been completed, close the gcode if neccessary and */
    //fprintf(fGCode, "\nS0\nG0 X0 Y0\n");
    sprintf(buffer, "\nS0\nG0 X0 Y0\n");
    SendCommands(buffer);
    Sleep(5000);

    //F fclose(fGCode);
    //F if (fGCode == NULL)
    //F {
    //F     return 3;
    //F }

    /* Function executed successfully if it reaches this point */
    return 1;
}