/*
 * cardtest1.c
 */

/******************************
This is a unit test for the playCardSmithy function in dominion.c.

Description:
Adds 3 cards from the deck to the player’s hand, and discards the Smithy card from hand into the discard pile. 

Input:
● struct gameState state – holds a pointer to a gameState variable
● int player
● int handPos - hand card position number

Return:
Returns 0 on success

Dependencies:
● drawCard
	○ shuffle()
● discardCard



Basic Steps in Unit Test:
1.  Set up the “initial conditions” for the test
-initialize game using initializeGame() function
-hard code number of cards in player's hand, deck, and discard
-hard code Smithy card into the hand

2.  Carry out/execute the action(s) you want to test
-call playCardSmithy()

3.  Check/assert that the desired/expected result(s) were achieved

Test 1: TEST OF ACTION WHEN PLAYER'S HAND, DECK, DISCARD ARE NOT EMPTY, SMITHY CARD IS IN HAND POSITION 0-2 IN A HAND OF 3 CARDS
	-check if hand increased by +3 cards, -1 smithy card.
	-check if deck decreased by -3 cards.
	-check if discard increased by +1 smithy card.
	-check if 3 hand cards is equal to 3 deck cards drawn from the top.
	-check if discarded smithy card is at top of discard pile.

******************************/

/*
 * Include the following lines in your makefile:
 *
 * cardtest1: cardtest1.c dominion.o rngs.o
 *      gcc -o cardtest1 -g  cardtest1.c dominion.o rngs.o $(CFLAGS)
 */


#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "smithy"

void assertTrue(int truthCondition, int returnVal, int* numOfFails, int unitTestNum);
void cardNumToName(int card, char *name);
void printCards(struct gameState *game);

int main() {
/*
    int newCards = 0;
    int discarded = 1;
    int xtraCoins = 0;
    int shuffledCards = 0;

    int i, j, m;
    
    int remove1, remove2;
*/	
	int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
    int seed = 1000;
    int numPlayers = 2;
    //int thisPlayer = 0;
	struct gameState G, testG;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};

	int preHandCount;
	int preDeckCount;
	int preDiscardCount;
	int numOfFails = 0;
	
	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);

	printf("----------------- Testing Card: %s ----------------\n", TESTCARD);
	
	/*********
	-initialize game using initializeGame() function
	-hard code number of cards in player's hand, deck, and discard
	-hard code Smithy card into the hand
	*********/
	
	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	testG.handCount[0] = 3; //Player 0 has 3 cards in hand.
	testG.hand[0][0] = smithy;
	testG.hand[0][1] = copper;
	testG.hand[0][2] = estate;

	
	testG.deckCount[0] = 3; //Player 0 has 3 cards in deck.
	testG.deck[0][0] = gold;
	testG.deck[0][1] = mine;
	testG.deck[0][2] = curse;

	
	testG.discardCount[0] = 3; //Player 0 has 3 cards in discard.
	testG.discard[0][0] = feast;
	testG.discard[0][1] = village;
	testG.discard[0][2] = gardens;
	
	preHandCount = testG.handCount[0];
	preDeckCount = testG.deckCount[0];
	preDiscardCount = testG.discardCount[0];
	
	printf("\nBefore playing Smithy card.\n");
	printCards(&testG); //PRE SMITHY CARD PLAYED
	
	//call playCardSmithy indirectly through the cardEffect function
	//Smithy has no choices to pass in.
	cardEffect(smithy, choice1, choice2, choice3, &testG, handpos, &bonus);
	
	/***********
	-check if hand increased by +3 cards, -1 smithy card.
	-check if deck decreased by -3 cards.
	-check if discard increased by +1 smithy card.
	-check if 3 hand cards is equal to 3 deck cards drawn from the top.
	-check if discarded smithy card is at top of discard pile.
	
	***********/
	printf("\n");
	assertTrue(preHandCount + 3 - 1, testG.handCount[0], &numOfFails, 1); //test if correct number of cards in hand
	assertTrue(preDeckCount - 3, testG.deckCount[0], &numOfFails, 1); //test if correct number of cards in deck
	assertTrue(preDiscardCount + 1, testG.deckCount[0], &numOfFails, 1); //test if correct number of cards in discard

	printf("\nAfter playing Smithy card.\n\n");
	printCards(&testG); //POST SMITHY CARD PLAYED
	
	printf("\n*** TESTS FAILED = %d ***\n\n", numOfFails);
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
	if(game->handCount[0] > 0)
	{
		for(i = 0; i < game->handCount[0]; i++)
		{
			hand = game->hand[0][i];
			//h = hand;
			cardNumToName(hand, cardName);
			printf("Hand card %d = %s\n", i, cardName);


		}
	}
	else
	{
		printf("Hand is empty.\n");
	}
	printf("\n");
	
	if(game->deckCount[0] > 0)
	{
		for(i = 0; i < game->deckCount[0]; i++)
		{
			deck = game->deck[0][i];
			//d = deck;
			cardNumToName(deck, cardName);

			printf("Deck card %d = %s\n", i, cardName);
		}
	}
	else
	{
		printf("Deck is empty.\n");
	}
	printf("\n");
	
	if(game->discardCount[0] > 0)
	{
		for(i = 0; i < game->discardCount[0]; i++)
		{

			discard = game->discard[0][i];
			//d = deck;
			cardNumToName(discard, cardName);

			printf("Discard card %d = %s\n", i, cardName);
		}
	}
	else
	{
		printf("Discard is empty.\n");
	}
}



