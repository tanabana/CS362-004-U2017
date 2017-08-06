/*
 * cardtest2.c
 */

/******************************
This is a unit test for the playCardAdventurer function in dominion.c.

Description:
Reveal cards from your deck until you reveal 2 treasure cards. 
Put those treasure cards into your hand and discard the other revealed cards.


Input:
● struct gameState state – holds a pointer to a gameState variable
● int currentPlayer
● int temphand[MAX_HAND] - keeps track of the cards drawn when playing the adveturer card
● int drawntreasure - keeps track of the number of treasure cards drawn

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
-hard code Adventurer card into the hand

2.  Carry out/execute the action(s) you want to test
-call playCardAdventurer()

3.  Check/assert that the desired/expected result(s) were achieved

Test 1: TEST OF ACTION WHEN PLAYER'S HAND, DECK, DISCARD ARE NOT EMPTY, DRAWS 2 TREASURE CARDS BEFORE DECK RUNS OUT
	-check if hand increased by 2 treasure cards.
	-check if deck decreased by -2 treasure cards.
	-check if discard increased by number of drawn cards - 2.
	-check if cards drawn that are to be discarded are all non-treasure cards.
	-check if played adventurer card is in played card pile.

Test 2: TEST OF ACTION WHEN PLAYER'S HAND, DECK, DISCARD ARE NOT EMPTY, DRAWS 2 TREASURE CARDS WITH DECK RUNNING OUT OF CARDS
	-check if hand increased by 2 treasure cards.
	-check if deck decreased by -2 treasure cards.
	-check if discard increased by number of drawn cards - 2.
	-check if cards drawn that are to be discarded are all non-treasure cards.
	-check if played adventurer card is in played card pile.
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

#define TESTCARD "adventurer"

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
	int i;
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
	int prePlayedCardCount;
	int numOfCardsDrawn;
	int numOfFails = 0;
	int preNumOfTreasureInDiscard = 0;
	int postNumOfTreasureInDiscard = 0;
	int treasureCardCount = 0;
	
	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);

	printf("----------------- Testing Card: %s ----------------\n", TESTCARD);
	
	/*********
	Test 1: TEST OF ACTION WHEN PLAYER'S HAND, DECK, DISCARD ARE NOT EMPTY, DRAWS 2 TREASURE CARDS BEFORE DECK RUNS OUT
	-initialize game using initializeGame() function
	-hard code number of cards in player's hand, deck, and discard
	-hard code Adventure card into the hand
	*********/
	
	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	testG.handCount[0] = 3; //Player 0 has 3 cards in hand.
	testG.hand[0][0] = adventurer;
	testG.hand[0][1] = copper;
	testG.hand[0][2] = estate;

	
	testG.deckCount[0] = 4; //Player 0 has 4 cards in deck.
	testG.deck[0][0] = mine;
	testG.deck[0][1] = gold;
	testG.deck[0][2] = silver;	
	testG.deck[0][3] = curse;

	
	testG.discardCount[0] = 3; //Player 0 has 3 cards in discard.
	testG.discard[0][0] = feast;
	testG.discard[0][1] = village;
	testG.discard[0][2] = gardens;
	
	preHandCount = testG.handCount[0];
	preDeckCount = testG.deckCount[0];
	preDiscardCount = testG.discardCount[0];
	prePlayedCardCount = testG.playedCardCount;
	
	numOfCardsDrawn = 3; //First curse, then silver, then gold should be drawn before stopping
	
	//Count number of treasure cards in discard BEFORE playing adventure.
	for(i = 0; i < testG.discardCount[0]; i++)
	{
		if(testG.discard[0][i] == copper || testG.discard[0][i] == silver || testG.discard[0][i] == gold )
		{
			preNumOfTreasureInDiscard++;
		}
	}
/*	char name[32];
	cardNumToName(testG.deck[0][3], name);
	printf("\n\ntestG.deck[0][3] = %s\n", name);
	cardNumToName(testG.deck[0][4], name);
	printf("testG.deck[0][4] = %s\n", name);
	cardNumToName(testG.deck[0][5], name);
	printf("testG.deck[0][5] = %s\n", name);
	cardNumToName(testG.deck[0][6], name);
	printf("testG.deck[0][6] = %s\n", name);
	cardNumToName(testG.deck[0][7], name);
	printf("testG.deck[0][7] = %s\n", name);
	cardNumToName(testG.deck[0][8], name);
	printf("testG.deck[0][8] = %s\n\n", name);*/
	
	printf("\nBefore playing Adventurer card.\n");
	printCards(&testG); //PRE ADVENTURER CARD PLAYED
	
	//call playCardAdventurer indirectly through the cardEffect function
	//Adventurer has no choices to pass in.
	cardEffect(adventurer, choice1, choice2, choice3, &testG, handpos, &bonus);
	
	/***********
	-check if hand increased by 2 treasure cards.
	-check if deck decreased by -2 treasure cards.
	-check if discard increased by number of drawn cards minus 2.
	-check if cards drawn that are to be discarded are all non-treasure cards.
	-check if played adventurer card is in played card pile.
	***********/
	printf("\n");
	printf("Correct number of cards in hand:\t");
	assertTrue(preHandCount + 2, testG.handCount[0], &numOfFails, 1); //test if correct number of cards in hand
	
	printf("Two treasure cards added to hand:\t");
	//Check top two cards added to hand
	for(i=testG.handCount[0]; i > testG.handCount[0] - 2; i--)
	{
		if(testG.hand[0][testG.handCount[0]-1] == copper || testG.hand[0][testG.handCount[0]-1] == silver || testG.hand[0][testG.handCount[0]-1] == gold)
		{
			treasureCardCount++;
		}
	}
	assertTrue(2, treasureCardCount, &numOfFails, 1);
	
	printf("Correct number of cards in deck:\t");
	assertTrue(preDeckCount - numOfCardsDrawn, testG.deckCount[0], &numOfFails, 1); //test if correct number of cards in deck
	
	printf("\n\nNum of card drawn is %d\n\n", numOfCardsDrawn);
	
	//Count number of treasure cards in discard after playing adventure.
	for(i = 0; i < testG.discardCount[0]; i++)
	{
		if(testG.discard[0][i] == copper || testG.discard[0][i] == silver || testG.discard[0][i] == gold )
		{
			postNumOfTreasureInDiscard++;
		}
	}
	printf("No treasure cards discarded:\t");
	assertTrue(preNumOfTreasureInDiscard, postNumOfTreasureInDiscard, &numOfFails, 1); //no treasure cards should be discarded

	printf("\nAfter playing Adventurer card.\n\n");
	printCards(&testG); //POST ADVENTURER CARD PLAYED
	
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



