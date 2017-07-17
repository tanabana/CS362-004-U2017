#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

/******************************
This is a unit test for the drawCard function in dominion.c.

Description:
Adds a card from the deck to the player’s hand. If the deck is empty the deck is
shuffled, and if still empty returns -1.
Input:
● struct gameState state – holds a pointer to a gameState variable
● int player
Return:
Returns -1 if the deck is empty after shuffling
Returns 0 on success
Data structures:
● int count
● int deckCounter
Dependencies:
● local
○ shuffle()
● stdio
○ printf()


Basic Steps in Unit Test:
1.  Set up the “initial conditions” for the test
-initialize game using initializeGame() function
-hard code number of cards in player's hand, deck, and discard

2.  Carry out/execute the action(s) you want to test
-call drawCard()

3.  Check/assert that the desired/expected result(s) were achieved

Test 1: TEST OF ACTION WHEN PLAYER'S HAND, DECK, DISCARD ARE NOT EMPTY.
	-check if hand increased by one.
	-check if deck decreased by one.
	-check if new hand card is equal to deck card drawn from the top.
	-check that 0 is returned.

Test 2: TEST OF ACTION WHEN DECK IS EMPTY BUT DISCARD IS NOT.
	-check that drawn card is one of the previous deck cards
	-check that 0 is returned by drawCard function

Test 3: TEST OF ACTION WHEN DECK AND DISCARD IS EMPTY.
	-check that hand count is unchanged
	-check that -1 is returned

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
void cardNumToName(int card, char *name);
void printCards(struct gameState *game);

int main()
{
    int seed = 1000;
    int numPlayer = 2;
    int r;
    int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};
    struct gameState G;
    int numOfFails = 0;
	int cardToDraw;
	int preHandCount;
	int preDeckCount;
	
    r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
    
//Test 1: TEST OF ACTION WHEN PLAYER'S HAND, DECK, DISCARD ARE NOT EMPTY (NON-EDGE CASE).
	G.handCount[0] = 3; //Player 0 has 3 cards in hand.
	G.hand[0][0] = copper;
	G.hand[0][1] = smithy;
	G.hand[0][2] = estate;
	
	G.deckCount[0] = 3; //Player 0 has 3 cards in deck.
	G.deck[0][0] = gold;
	G.deck[0][1] = mine;
	G.deck[0][2] = curse;
	
	G.discardCount[0] = 3; //Player 0 has 3 cards in discard.
	G.discard[0][0] = feast;
	G.discard[0][1] = village;
	G.discard[0][2] = gardens;
	
	//This card on top of deck pile and should end up in hand.
	cardToDraw = curse;
	
	preHandCount = G.handCount[0];
	preDeckCount = G.deckCount[0];
	
	printf("Hand count = %d\n", G.handCount[0]);
	printf("Deck count = %d\n", G.deckCount[0]);
	printf("Discard count = %d\n\n", G.discardCount[0]);
	
	printf("Before Drawing...\n");
	printCards(&G); //See precondition of set of cards.
	
	r = drawCard(0, &G); // Draw a card from the deck.
	
	printf("After Drawing 1...\n");
	printCards(&G); //See postcondition of set of cards.	
	
	printf("\nHand count = %d\n", G.handCount[0]);
	printf("Deck count = %d\n", G.deckCount[0]);
	printf("Discard count = %d\n\n", G.discardCount[0]);

	//check if hand increased by one.
	assertTrue(preHandCount + 1, G.handCount[0], &numOfFails, 1);
	
	//check if deck decreased by one.
	assertTrue(preDeckCount - 1, G.deckCount[0], &numOfFails, 1);

	//Should return 0 on success.
	assertTrue(0, r, &numOfFails, 1);
	
	//After drawing a card, last card in deck should now be last card in hand.
	assertTrue(cardToDraw, G.hand[0][3], &numOfFails, 1);

	
	//Test 2: TEST OF ACTION WHEN DECK IS EMPTY (EDGE CASE).
	memset(&G, 23, sizeof(struct gameState));   // clear the game state
	r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
	
	G.handCount[0] = 3; //Player 0 has 3 cards in hand.
	G.hand[0][0] = copper;
	G.hand[0][1] = smithy;
	G.hand[0][2] = estate;
	
	G.deckCount[0] = 0; //Player 0 has 0 cards in deck.
	
	G.discardCount[0] = 3; //Player 0 has 3 cards in discard.
	G.discard[0][0] = feast;
	G.discard[0][1] = village;
	G.discard[0][2] = gardens;
	
	
	
	r = drawCard(0, &G); // Draw a card from the deck.
	
	printf("After Drawing from Empty Deck...\n");
	printCards(&G); //See postcondition of set of cards.	
	
	printf("\nHand count = %d\n", G.handCount[0]);
	printf("Deck count = %d\n", G.deckCount[0]);
	printf("Discard count = %d\n\n", G.discardCount[0]);
	

	//Should return 0 on success.
	assertTrue(0, r, &numOfFails, 2);
	
	printf("------------------------------\n");
	printf("\nNOTE: ONLY ONE OUT OF THE FOLLOWING THREE NEEDS TO PASS.\n");
	//After drawing a card, last card in deck should now be last card in hand.
	cardToDraw = feast;
	assertTrue(cardToDraw, G.hand[0][3], &numOfFails, 2);
	cardToDraw = village;
	assertTrue(cardToDraw, G.hand[0][3], &numOfFails, 2);
	cardToDraw = gardens;
	assertTrue(cardToDraw, G.hand[0][3], &numOfFails, 2);
	numOfFails = numOfFails - 2;
	printf("------------------------------\n");
	
//Test 3: TEST OF ACTION WHEN DECK AND DISCARD IS EMPTY (EDGE CASE).
	memset(&G, 23, sizeof(struct gameState));   // clear the game state
	r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
	
	G.handCount[0] = 3; //Player 0 has 3 cards in hand.
	G.hand[0][0] = copper;
	G.hand[0][1] = smithy;
	G.hand[0][2] = estate;
	
	G.deckCount[0] = 0; //Player 0 has 0 cards in deck.
	G.discardCount[0] = 0; //Player 0 has 0 cards in discard.
	
	preHandCount = G.handCount[0]; // to compare to after drawing a card
	
	r = drawCard(0, &G); // Draw a card from the deck.

	printf("After Drawing from Empty Deck with Empty Discard...\n");
	printCards(&G); //See postcondition of set of cards.	
	
	printf("\nHand count = %d\n", G.handCount[0]);
	printf("Deck count = %d\n", G.deckCount[0]);
	printf("Discard count = %d\n\n", G.discardCount[0]);

	//check that hand count is unchanged
	assertTrue(preHandCount, G.handCount[0], &numOfFails, 3);
	
	//Should return -1 when both deck and discard are empty initially.
	assertTrue(-1, r, &numOfFails, 3);
	
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

//Converts number to card name. Borrowed/Copied from interface.c
void cardNumToName(int card, char *name)
{
  switch(card){
  case curse: strcpy(name,"Curse");
    break;
  case estate: strcpy(name,"Estate");
    break;
  case duchy: strcpy(name,"Duchy");
    break;
  case province: strcpy(name,"Province");
    break;
  case copper: strcpy(name,"Copper");
    break;
  case silver: strcpy(name,"Silver");
    break;
  case gold: strcpy(name,"Gold");
    break;
  case adventurer: strcpy(name,"Adventurer");
    break;
  case council_room: strcpy(name,"Council Room");
    break;
  case feast: strcpy(name,"Feast");
    break;
  case gardens: strcpy(name,"Gardens");
    break;
  case mine: strcpy(name,"Mine");
    break;
  case remodel: strcpy(name,"Remodel");
    break;
  case smithy: strcpy(name,"Smithy");
    break;
  case village: strcpy(name,"Village");
    break;
  case baron: strcpy(name,"Baron");
    break;
  case great_hall: strcpy(name,"Great Hall");
    break;
  case minion: strcpy(name,"Minion");
    break;
  case steward: strcpy(name,"Steward");
    break;
  case tribute: strcpy(name,"Tribute");
    break;
  case ambassador: strcpy(name,"Ambassador");
    break;
  case cutpurse: strcpy(name,"Cutpurse");
    break;
  case embargo: strcpy(name,"Embargo");
    break;
  case outpost: strcpy(name,"Outpost");
    break;
  case salvager: strcpy(name,"Salvager");
    break;
  case sea_hag: strcpy(name,"Sea Hag");
    break;
  case treasure_map: strcpy(name,"Treasure Map");
    break;

  default: strcpy(name,"?");
  }
}

//Prints out the cards for hand, deck and discard in order.
void printCards(struct gameState *game)
{
	int i, deck, hand, discard;
	char cardName[32];
	printf("\n");
	for(i = 0; i < game->handCount[0]; i++)
	{
		hand = game->hand[0][i];
		//h = hand;
		cardNumToName(hand, cardName);
		printf("Hand card %d = %s\n", i, cardName);


	}
	printf("\n");
	for(i = 0; i < game->deckCount[0]; i++)
	{
		deck = game->deck[0][i];
		//d = deck;
		cardNumToName(deck, cardName);

		printf("Deck card %d = %s\n", i, cardName);
	}
	printf("\n");
	for(i = 0; i < game->discardCount[0]; i++)
	{
		discard = game->discard[0][i];
		//d = deck;
		cardNumToName(discard, cardName);

		printf("Discard card %d = %s\n", i, cardName);
	}

}
