#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

/******************************
This is a unit test for the gainCard function in dominion.c.

Description:
Adds a specific card to a players hand, deck, or trash. Checks for enough
supply of the card then puts the card in the directed location.
Input:
● struct gameState state – holds a pointer to a gameState variable
● int supplyPos – enumerated card
● int toFlag – where to put the card
● int player –who to give it to
Return:
Returns -1 if not enough supply
Returns 0 on success
Data structures:
● none
Dependencies:
● local
○ supplyCount()


Basic Steps in Unit Test:
1.  Set up the “initial conditions” for the test
-initialize game using initializeGame() function
-hard code number of cards in player's hand, deck, and discard
-hard code supplyCount[] value of a particular card.

2.  Carry out/execute the action(s) you want to test

3.  Check/assert that the desired/expected result(s) were achieved 

Test 1: Edge case where supplyCount[card] is 0.
Test 2: Edge case where supplyCount[card] is 1.
Test 3: Non-Edge case where supplyCount[card] is greater than 1.
Test 4: Case where you attempt to gain a card that does not exist.
Test 5: Case where gained card is placed in hand.
Test 6: Case where gained card is placed in discard.
Test 7: Case where gained card is placed in deck.

>Tests 5, 6, 7 have been incorporated into Tests 1-4.

******************************/


/************
STRUCT gameState properties:


  int numPlayers; //number of players
  int supplyCount[treasure_map+1];  //this is the amount of a specific type of card given a specific number.
  int embargoTokens[treasure_map+1];
  int outpostPlayed;
  int outpostTurn;
  int whoseTurn;
  int phase;
  int numActions;
  int coins; 
  int numBuys; 
  int hand[MAX_PLAYERS][MAX_HAND];
  int handCount[MAX_PLAYERS];
  int deck[MAX_PLAYERS][MAX_DECK];
  int deckCount[MAX_PLAYERS];
  int discard[MAX_PLAYERS][MAX_DECK];
  int discardCount[MAX_PLAYERS];
  int playedCards[MAX_DECK];
  int playedCardCount;
***************/

void assertTrue(int truthCondition, int returnVal, int* numOfFails, int unitTestNum);

int main()
{
    int seed = 1000;
    int numPlayer = 2;
    int r;

    int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};
    struct gameState G;
    int numOfFails = 0;

   // memset(&G, 23, sizeof(struct gameState));   // clear the game state
    r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
    
//TEST 1: EDGE CASE WHERE supplyCount[card] IS 0.
    G.supplyCount[curse] = 0;
    r = gainCard(curse, &G, 0, 0); //gain curse card toFlag = 0 means add to discard, player 0. Should return -1 if cards have run out.
    assertTrue(-1, r, &numOfFails, 1);
	assertTrue(0, G.supplyCount[curse], &numOfFails, 1); //assert that supplyCount of curse cards is still 0.
	
	r = gainCard(curse, &G, 1, 0); //gain curse card toFlag = 1 means add to deck, player 0. Should return -1 if cards have run out.
    assertTrue(-1, r, &numOfFails, 1);
	assertTrue(0, G.supplyCount[curse], &numOfFails, 1); //assert that supplyCount of curse cards is still 0.

   	r = gainCard(curse, &G, 2, 0); //gain curse card toFlag = 2 means add to hand, player 0. Should return -1 if cards have run out.
    assertTrue(-1, r, &numOfFails, 1);
	assertTrue(0, G.supplyCount[curse], &numOfFails, 1); //assert that supplyCount of curse cards is still 0.
	
	
//TEST 2: EDGE CASE WHERE supplyCount[card] IS 1.
	G.supplyCount[smithy] = 1;
	r = gainCard(smithy, &G, 0, 0); //gain smithy card toFlag = 0 means to discard, player 0. Should return 0 if cards have not run out.
    assertTrue(0, r, &numOfFails, 2);	
	assertTrue(0, G.supplyCount[smithy], &numOfFails, 2); //assert that supplyCount of curse cards is now 0.
	
	G.supplyCount[smithy] = 1;
	r = gainCard(smithy, &G, 1, 0); //gain smithy card toFlag = 1 means to deck, player 0. Should return 0 if cards have not run out.
    assertTrue(0, r, &numOfFails, 2);	
	assertTrue(0, G.supplyCount[smithy], &numOfFails, 2); //assert that supplyCount of curse cards is now 0.
	
	G.supplyCount[smithy] = 1;
	r = gainCard(smithy, &G, 2, 0); //gain smithy card toFlag = 2 means to discard, player 0. Should return 0 if cards have not run out.
    assertTrue(0, r, &numOfFails, 2);	
	assertTrue(0, G.supplyCount[smithy], &numOfFails, 2); //assert that supplyCount of curse cards is now 0.


//TEST 3: NON-EDGE CASE WHERE supplyCount[card] IS GREATER THAN 1.
	G.supplyCount[adventurer] = 5;
	r = gainCard(adventurer, &G, 0, 0); //gain adventurer card toFlag = 0 means to discard, player 0. Should return 0 if cards have not run out.
    assertTrue(0, r, &numOfFails, 3);	
	assertTrue(4, G.supplyCount[adventurer], &numOfFails, 3); //assert that supplyCount is correct.
	
	G.supplyCount[adventurer] = 8;
	r = gainCard(adventurer, &G, 1, 0); //gain adventurer card toFlag = 1 means to deck, player 0. Should return 0 if cards have not run out.
    assertTrue(0, r, &numOfFails, 3);	
	assertTrue(7, G.supplyCount[adventurer], &numOfFails, 3); //assert that supplyCount is correct.
	
	G.supplyCount[adventurer] = 6;
	r = gainCard(adventurer, &G, 2, 0); //gain adventurer card toFlag = 2 means to discard, player 0. Should return 0 if cards have not run out.
    assertTrue(0, r, &numOfFails, 3);	
	assertTrue(5, G.supplyCount[adventurer], &numOfFails, 3); //assert that supplyCount is correct.

//TEST 4: CASE WHERE YOU ATTEMPT TO GAIN A CARD THAT DOES NOT EXIST.
	//printf("Tribute supply card number is %d\n", G.supplyCount[tribute]);
	//G.supplyCount[tribute] = 6;
	r = gainCard(tribute, &G, 0, 0); //gain tribute card toFlag = 0 means to discard, player 0. Should return 0 if cards have not run out.
    assertTrue(-1, r, &numOfFails, 4);	
	assertTrue(-1, G.supplyCount[tribute], &numOfFails, 4); //assert that supplyCount is correct.

    r = gainCard(tribute, &G, 1, 0); //gain tribute card toFlag = 0 means to discard, player 0. Should return 0 if cards have not run out.
    assertTrue(-1, r, &numOfFails, 4);	
	assertTrue(-1, G.supplyCount[tribute], &numOfFails, 4); //assert that supplyCount is correct.
	
	r = gainCard(tribute, &G, 2, 0); //gain tribute card toFlag = 0 means to discard, player 0. Should return 0 if cards have not run out.
    assertTrue(-1, r, &numOfFails, 4);	
	assertTrue(-1, G.supplyCount[tribute], &numOfFails, 4); //assert that supplyCount is correct.
	
	
	r = gainCard(treasure_map, &G, 0, 0); //gain treasure_card toFlag = 0 means to discard, player 0. Should return 0 if cards have not run out.
    assertTrue(-1, r, &numOfFails, 4);	
	assertTrue(-1, G.supplyCount[treasure_map], &numOfFails, 4); //assert that supplyCount is correct.
	

    printf("\n*** TESTS FAILED = %d ***\n", numOfFails);
	return 0;
}

//Counts number of failed tests.
void assertTrue(int truthCondition, int returnVal, int* numOfFails, int unitTestNum)
{
	printf("TEST %d:", unitTestNum);
    if(returnVal == truthCondition)
    {
		printf("\tPASS.\n");
    }
    else
    {
		printf("\tFAIL.\n");
		(*numOfFails)++;
    }
}