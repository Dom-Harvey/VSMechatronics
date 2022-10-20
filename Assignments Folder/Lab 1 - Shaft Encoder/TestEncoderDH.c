#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define bool int
#define byte unsigned char
#define numChars  32

void initialiseEncoderStateMachine();
void updateEncoderStateMachine();

// Here we are using global variables simply to be compatible with the Arduino program structure
// This is REALLY bad practice so please don't do it otherwise!
long int count = 0;
long int error = 0;

enum states {state1=1, state2, state3, state4};
bool channelAState=0;
bool channelBState=0;

enum states state;
char receivedChars[numChars]="00";   // an array to store the received data

int main()
{
    // Replicates the setup in Arduino
    printf("Enter a pair of characters representing initial states of channels A and B\n");
    scanf("%s",receivedChars);
    channelAState = receivedChars[0]!='0';
    channelBState = receivedChars[1]!='0';

    initialiseEncoderStateMachine();
    printf("State %d, count %ld, error %ld\n", state, count, error);
    printf("Enter a pair of characters representing channels A and B, enter 99 to end\n");

    // Replicates the loop in Arduino
    do
    {
        scanf("%s",receivedChars);
        if (strcmp(receivedChars, "99")==0)
        {
            break;
        }
        channelAState = receivedChars[0]!='0';
        channelBState = receivedChars[1]!='0';
        updateEncoderStateMachine();
        printf("State %d, count %ld, error %ld\n", state, count, error);

    }
    while(1);
    return 0;
}
void initialiseEncoderStateMachine()
{
    /* If initially A is 0 and B is 0, system starts in State 1
       If initially A is 1 and B is 0, system starts in State 2
       If initially A is 1 and B is 1, system starts in State 3
       If initially A is 0 and B is 1, system starts in State 4 */

    if (channelAState)
    {
        if (channelBState)                                                      //and if channel A is high and channel B is high
        {
            state = state3;                                                     //A = 1, B = 1, state = 3
        }
        /* else ....  lots of code goes here */
        else                                                                    //else is sufficient, as channelXState variable is a boolean
        {
            state = state2;                                                     //A = 1, B = 0, state = 2
        }
    }
    else                                                                        //else is sufficient, as channelXState variable is a boolean
    {
        if (channelBState)
        {
            state = state4;                                                     //A = 0, B = 1, state = 4
        }
        else
        {
            state = state1;                                                     //A = 0, B = 0, state = 1
        }
    }

}

void updateEncoderStateMachine()
{
    switch (state)
    {
    case state1:
        /*  If A is 0 and B is 0, do nothing and stay in State 1
            If A is 1 and B is 0, add 1 to main counter and go to State 2
            If A is 0 and B is 1, subtract 1 to main counter and go to State 4
            If A is 1 and B is 1, do nothing to main counter but add 1 to error counter and go to state 3 */

        if (channelAState && !channelBState)
        {
            count++;
            state = state2;
        }
        /* else ....  lots of code goes here */
        
        else if (!channelAState && channelBState)
        {
            count--;
            state = state4;
        }

        else if (channelAState && channelBState)
        {
            error++;
            state = state3;
        }
        else
        {
            state = state1;
        }
        break; /* don't forget break at the end of each case! */

    case state2:
        /*  If A is 0 and B is 0, subtract 1 from the main counter and go to State 1
            If A is 1 and B is 0, do nothing and stay in State 2
            If A is 0 and B is 1, add 1 to error counter and go to State 4
            If A is 1 and B is 1, add 1 to main counter and go to State 3 */

        if (!channelAState && !channelBState)
        {
            count--;
            state = state1;
        }

        else if (!channelAState && channelBState)
        {
            error++;
            state = state4;
        }

        else if (channelAState && channelBState)
        {
            count++;
            state = state3;
        }

        else
        {
            state = state2;
        }
        break;

    case state3:
        /*  If A is 0 and B is 0, add 1 to error counter and go to State 1
            If A is 1 and B is 0, subtract 1 from main counter and go to state 2
            If A is 0 and B is 1, add 1 to main counter and go to state 4
            If A is 1 and B is 1, do nothing and stay in state 3 */

        if (!channelAState && !channelBState)
        {
            error++;
            state = state1;
        }

        else if (channelAState && !channelBState)
        {
            count--;
            state = state2;
        }

        else if (!channelAState && channelBState)
        {
            count++;
            state = state4;
        }

        else
        {
            state = state3;
        }
        break;

    case state4:
        /*  If A is 0 and B is 0, add 1 to main counter and go to state 1
            If A is 1 and B is 0, add 1 to error counter and go to state 2
            If A is 0 and B is 1, do nothing and stay in state 4
            If A is 1 and B is 1, subtract 1 from main counter and go to state 3*/

        if (!channelAState && !channelAState)
        {
            count++;
            state = state1;
        }

        else if (channelAState && !channelBState)
        {
            error++;
            state = state2;
        }

        else if (channelAState && channelBState)
        {
            count--;
            state = state3;
        }

        else
        {
            state = state4;
        }
        break;
    }
}
