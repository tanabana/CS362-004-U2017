#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
/******************************
This is a unit test for the isGameOver function in dominion.c.

In Dominion, game ends when:
1. the supply of Province cards has been depleted, OR
2. three supply piles have been depleted.


Basic Steps in Unit Test:
1.  Set up the “initial conditions” for the test
2.  Carry out/execute the action(s) you want to test
3.  Check/assert that the desired/expected result(s) were achieved 

Test 1: Iterate through Province cards, reducing the number by one until all are gone. Check that game does not end
when # of cards > 0, and that the game ends when # of cards is 0.

Test 2: Check end cases of array, supplyCount[] and arbitrary middle card. Set supplyCount[0], supplyCount[26], supplyCount[11]
to 0, one at at time and check that the game does not end until all three are set to 0.

Test 3: Check middle cases of array, supplyCount[]. Choose three arbitrary supplyCount[] array elements. Set one to zero, check
if game is over, set second one to zero, and check again. Finally check after third one is set to zero, and check if the game
ends as it should.

????any three supply piles have been depleted (all possible combinations?), game ends

******************************/

/* -----------------------------------------------------------------------
 * Include the following lines in your makefile:
 *
 * unittest1: unittest1.c dominion.o rngs.o
 *      gcc -o unittest1 -g  unittest1.c dominion.o rngs.o $(CFLAGS)
 * -----------------------------------------------------------------------
 */


void assertTrue(int truthCondition, int returnVal, int* numOfFails);

int main() 
{
    int seed = 1000;
    int numPlayer = 2;
    int returnVal;
    int r;
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
    struct gameState G, testG;
    int numOfFails = 0;

    printf ("\nTESTING isGameOver() Function:\n");
    printf("TEST 1: When Province cards run out\n");
    printf("isGameOver() returns 1 when game is over, and returns 0 if game is not over.\n\n");

    //memset(&G, 23, sizeof(struct gameState));   // clear the game state
    r = initializeGame(numPlayer, k, seed, &G); // initialize a new game. Returns 0 for successful initialization.
    // copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	if (r == 0) 
    {
        while (testG.supplyCount[province] > 0)
        {
            printf("testG.supplyCount[province] = %d.", testG.supplyCount[province]);
            returnVal = isGameOver(&testG);
            //assert(returnVal == 0); //Returns 1 when game is over. Returns 0 if game is not over.
            assertTrue(0, returnVal, &numOfFails);
            testG.supplyCount[province]--;
            /*
            if(returnVal == 0)
            {
                printf("\tPASS: Game is NOT over.\n");
                testG.supplyCount[province]--;                
            }
            else
            {
                printf("\tFAIL: Game ENDED. It should NOT end.\n");
                testG.supplyCount[province]--;
            }
*/

        }
            printf("testG.supplyCount[province] = %d.", testG.supplyCount[province]);
            returnVal = isGameOver(&testG);
            //assert(returnVal == 1); //Returns 1 when game is over. Returns 0 if game is not over.
            assertTrue(1, returnVal, &numOfFails);
        /*
            if(returnVal == 1)
            {
                printf("\tPASS: Game is OVER.\n");                
            }
            else
            {
                printf("\tFAIL: Game should END.\n");
            }
*/

        //return 0;
    }
    else
    {
        printf("Game initialization failed.\n");
        return -1;
    }

    printf("\n\nTEST 2a: when three of the supply cards run out.\n");
    printf("isGameOver() returns 1 when game is over, and returns 0 if game is not over.\n\n");
    printf("Checking EDGE cases of supplyCount array:\n\n");
               
    //SET PROVICE BACK TO ORIGINAL VALUE OF 8 FOR 2 PLAYER GAME
    //G.supplyCount[province] = 8;
	memcpy(&testG, &G, sizeof(struct gameState));
	
    //CHECK END CASES PLUS MIDDLE OF supplyCount ARRAY
    testG.supplyCount[curse] = 0; //curse defined as 0
    printf("testG.supplyCount[curse] = %d.", testG.supplyCount[curse]);
	
    returnVal = isGameOver(&testG);
    //assert(returnVal == 0); //Returns 1 when game is over. Returns 0 if game is not over.
    //printf("\tPASS: Game is NOT over.\n");
    assertTrue(0, returnVal, &numOfFails);
/*
    if(returnVal == 0)
    {
        printf("\t\tPASS: Game is NOT over.\n");
    }
    else
    {
        printf("\t\tFAIL: Game should not end.\n");
    }
*/

    testG.supplyCount[treasure_map] = 0; //treasure_map defined as 26, last element of array
    printf("testG.supplyCount[treasure_map] = %d.", testG.supplyCount[treasure_map]);
    returnVal = isGameOver(&testG);
    //assert(returnVal == 0); //Returns 1 when game is over. Returns 0 if game is not over.
    //printf("\tGame is NOT over.\n");
    assertTrue(0, returnVal, &numOfFails);
/*
    if(returnVal == 0)
    {
        printf("\tPASS: Game is NOT over.\n");
    }
    else
    {
        printf("\tFAIL: Game should not end.\n");
    }
*/

    testG.supplyCount[mine] = 0; //mine defined as 11
    printf("testG.supplyCount[mine] = %d.", testG.supplyCount[mine]);
    returnVal = isGameOver(&G);
    //assert(returnVal == 1); //Returns 1 when game is over. Returns 0 if game is not over.
    //printf("\tGame is OVER.\n");
    assertTrue(1, returnVal, &numOfFails);
/*
    if(returnVal == 1)
    {
        printf("\t\tPASS: Game is OVER.\n");
    }
    else
    {
        printf("\t\tFAIL: Game should END.\n");
    }
*/


    printf("\n\nTEST 2b: When three of the supply cards run out.\n");
    printf("isGameOver() returns 1 when game is over, and returns 0 if game is not over.\n\n");
    printf("Checking MIDDLE cases of supplyCount array:\n\n");

    //SET CURSE, TREASURE MAP, MINE BACK TO ORIGINAL VALUE OF 10 FOR 2 PLAYER GAME
    //G.supplyCount[curse] = 10;
    //G.supplyCount[treasure_map] = 10;
    //G.supplyCount[mine] = 10;
	memcpy(&testG, &G, sizeof(struct gameState));

    //CHECK END CASES PLUS MIDDLE OF supplyCount ARRAY
    testG.supplyCount[duchy] = 0; 
    printf("testG.supplyCount[duchy] = %d.", testG.supplyCount[duchy]);
    returnVal = isGameOver(&testG);
    //assert(returnVal == 0); //Returns 1 when game is over. Returns 0 if game is not over.
    //printf("\tPASS: Game is NOT over.\n");
    assertTrue(0, returnVal, &numOfFails);
/*
    if(returnVal == 0)
    {
        printf("\tPASS: Game is NOT over.\n");
    }
    else
    {
        printf("\tFAIL: Game should not end.\n");
    }
*/

    testG.supplyCount[smithy] = 0; 
    printf("testG.supplyCount[smithy] = %d.", testG.supplyCount[smithy]);
    returnVal = isGameOver(&G);
    //assert(returnVal == 0); //Returns 1 when game is over. Returns 0 if game is not over.
    //printf("\tGame is NOT over.\n");
    assertTrue(0, returnVal, &numOfFails);
/*
    if(returnVal == 0)
    {
        printf("\tPASS: Game is NOT over.\n");
    }
    else
    {
        printf("\tFAIL: Game should not end.\n");
    }
*/

    testG.supplyCount[salvager] = 0; //mine defined as 11
    printf("testG.supplyCount[salvager] = %d.", testG.supplyCount[salvager]);
    returnVal = isGameOver(&testG);
    //assert(returnVal == 1); //Returns 1 when game is over. Returns 0 if game is not over.
    //printf("\tGame is OVER.\n");
    assertTrue(1, returnVal, &numOfFails);
/*
    if(returnVal == 1)
    {
        printf("\tPASS: Game is OVER.\n");
    }
    else
    {
        printf("\tFAIL: Game should END.\n");
    }
*/
    if (numOfFails > 0)
    {
        printf("\n\n*** Number of Failed Tests: %d ***\n\n", numOfFails);
    }
    else
    {
        printf("\n\n*** All Tests PASSED! ***\n\n");
    }
    return 0;
}

//Counts number of failed tests.
void assertTrue(int truthCondition, int returnVal, int* numOfFails)
{

    if(returnVal == truthCondition)
    {
        if(returnVal == 0)
        {
            printf("\tPASS: Game is NOT over.\n");
        }
        else
        {
            printf("\tPASS: Game is OVER.\n");
        }
    }
    else
    {
        if(returnVal == 0)
        {
            printf("\tFAIL: Game should end.\n");
        }
        else
        {
            printf("\tFAIL: Game should not end.\n");
        }
        (*numOfFails)++;
    }
}