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
    int x[100],y[100], z[100];

    //*** DO DYNAMICALLY ALLOCATING
};

struct CommandArray
{
    int xGrid[9], yGrid[9], zGrid[9];
    char shape[20];
};

int ReadShapeDataFunction(struct ShapeCoordSystem *);
int DrawShapesFunction(struct CommandArray *);

int main(void)
{
    // Define ShapeCoordSystem for shapes square, invTri, star, raTri, cross;
    struct ShapeCoordSystem shapes[5];
    struct CommandArray commandArray[1];

    // Execute ReadShapeDataFunction
    printf("Extracting Shape Data from file...\n");
    int readShapeDataOutput;
    readShapeDataOutput = ReadShapeDataFunction(shapes);

    //*** ERROR CHECK
    if (readShapeDataOutput == 1)
    {
        printf("Shape Data Extracted Successfully\n");
    }
    else
    {
        printf("Shape Data not extracted successfully\n");
        return 1;
    }
    //*** EXECUTE READ DRAW DATA
    printf("\nExtracting Drawing Commands from file...\n");
    int drawShapeFunctionOutput;
    drawShapeFunctionOutput = DrawShapesFunction(commandArray);

    //*** ERROR CHECK
    if (drawShapeFunctionOutput == 1)
    {
        printf("Drawing Instructions extracted Successfully\n");
    }
    else
    {
        printf("Drawing Instructions not extracted successfully\n");
        return 1;
    }

    //*** EXECUTE SEND GCODE

    //*** ERROR CHECK


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
    // Closes the struct and checks that the file was closed successfully
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
    fDrawShapes = fopen ("DrawShapes.txt", "r");
    
    //*** CHECK FILE WAS OPENED
    if (fDrawShapes == NULL)
    {
        printf("File opening unsuccessful\n");
        return 1;
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

    //*** POPULATE COMMAND ARRAY
    int i = 0;
    for (i = 0; (i <= counter); i++)
    {
        fscanf(fDrawShapes, "%d %d %s", &ptr2->xGrid[i], &ptr2->yGrid[i], ptr2->shape);
        //F printf("%d %d %s\n", ptr2->xGrid[i], ptr2->yGrid[i], ptr2->shape);
    }

    //*** CHECK FILE WAS CLOSED
    if (fDrawShapes == NULL)
    {
        printf("File not closed successfully");
    }


    return 1;
}

int GCodeGenerationFunction()
{
    return 1;
}