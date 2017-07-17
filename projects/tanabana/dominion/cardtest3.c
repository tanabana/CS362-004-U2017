/*
 * cardtest3.c
 */

/******************************
This is a unit test for the playCardMine function in dominion.c.

Description:
YOU MAY TRASH A TREASURE CARD FROM YOUR HAND. GAIN A TREASURE TO YOUR HAND COSTING 3 COINS MORE THAN IT. (SO TRASH A COPPER, GAIN A SILVER. TRASH A SILVER, GAIN A GOLD.)


Input:
● struct gameState state – holds a pointer to a gameState variable
● int currentPlayer
● int handPos - card position of Mine in hand
● int choice1 - card position of treasure card to trash
● int choice2 - card number ("name") of treasure card to gain in hand

Return:
Returns 0 on success
Returns -1 for three cases:
● if value of chosen card (choice1) to trash is not a treasure card (copper, silver, or gold)
● if card to gain in hand is out of range (not an actual card)
● if value of the card to trash + 3 is !>= treasure card to gain

Dependencies:
● getCost
● gainCard
● discardCard


Basic Steps in Unit Test:
1.  Set up the “initial conditions” for the test
-initialize game using initializeGame() function
-hard code number of cards in player's hand
-hard code Mine card into the hand

2.  Carry out/execute the action(s) you want to test
-call playCardMine()

3.  Check/assert that the desired/expected result(s) were achieved

Test 1: ILLEGAL TRADE: COPPER FOR GOLD, COPPER FOR COPPER, SILVER FOR SILVER, ETC
	-check if choice1=copper, choice2=gold returns -1.
	-check if choice1=copper, choice2=cooper returns -1.
	-check if choice1=silver, choice2=copper returns -1.
	-check if choice1=silver, choice2=silver returns -1.
	-check if choice1=gold, choice2=gold returns -1.
	-check if choice1=gold, choice2=silver returns -1.
	-check if choice1=gold, choice2=copper returns -1.

Test 2: ILLEGAL TRADE: NON-TREASURE CARD FOR COPPER || SILVER || GOLD
	-check edge cases of choice1 = province and choice1 = adventurer
	-check general case of choice1 < copper and choice1 > gold (but within existing card ranges.
	
	
Test 3: ILLEGAL TRADE: COPPER || SILVER || GOLD FOR NON-EXISTENT CARD 
	-check edge cases of choice2 = curse - 1 and choice2 = treasure_map + 1 
	
Test 4: LEGAL CASE: COPPER FOR SILVER AND SILVER FOR GOLD
	-check choice1=copper and choice2 = silver
	-check choice1=silver and choice2 = gold

Test 5: TREASURE CARD FOR EXISTING NON-TREASURE CARD
	-check choice1=copper and choice2 = curse
	-check choice1=silver and choice2 = treasure_map
	-check choice3=gold and choice2 = smithy
******************************/

/*
 * Include the following lines in your makefile:
 *
 * cardtest3: cardtest3.c dominion.o rngs.o
 *      gcc -o cardtest3 -g  cardtest3.c dominion.o rngs.o $(CFLAGS)
 */


#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "Mine"

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
	int r;
	int choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
    int seed = 1000;
    int numPlayers = 2;
    //int thisPlayer = 0;
	struct gameState G, testG;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};

	int numOfFails = 0;
	
	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);

	printf("----------------- Testing Card: %s ----------------\n", TESTCARD);
	
/*********
	Test 1: ILLEGAL TRADE: COPPER FOR GOLD
	-check if choice1=copper, choice2=gold returns -1.
	-check if choice1=copper, choice2=cooper returns -1.
	-check if choice1=silver, choice2=copper returns -1.
	-check if choice1=silver, choice2=silver returns -1.
	-check if choice1=gold, choice2=gold returns -1.
	-check if choice1=gold, choice2=silver returns -1.
	-check if choice1=gold, choice2=copper returns -1.
*********/
	
	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	testG.handCount[0] = 4; //Player 0 has 4 cards in hand.
	testG.hand[0][0] = mine;
	testG.hand[0][1] = copper;
	testG.hand[0][2] = silver;
	testG.hand[0][3] = gold;
	
	printf("\nCheck if choice1=copper, choice2=gold returns -1\n");
	choice1 = 1; //copper card
	choice2 = gold;
	printf("\nBefore playing Mine card.\n");
	printCards(&testG); //PRE MINE CARD PLAYED
	
	//call playCardMine indirectly through the cardEffect function
	r = cardEffect(mine, choice1, choice2, choice3, &testG, 0, &bonus);
	printf("\nAfter playing Mine card.\n\n");
	printCards(&testG);
	assertTrue(r, -1, &numOfFails, 1);
	
	printf("______\n");
	
	
	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	testG.handCount[0] = 4; //Player 0 has 4 cards in hand.
	testG.hand[0][0] = mine;
	testG.hand[0][1] = copper;
	testG.hand[0][2] = silver;
	testG.hand[0][3] = gold;
	
	printf("\nCheck if choice1=copper, choice2=copper returns -1\n");
	choice1 = 1; //copper card
	choice2 = copper;
	printf("\nBefore playing Mine card.\n");
	printCards(&testG); //PRE MINE CARD PLAYED
	
	r = cardEffect(mine, choice1, choice2, choice3, &testG, 0, &bonus);
	printf("\nAfter playing Mine card.\n\n");
	printCards(&testG);
	assertTrue(r, -1, &numOfFails, 1);
	
	printf("______\n");
	
	
	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	testG.handCount[0] = 4; //Player 0 has 4 cards in hand.
	testG.hand[0][0] = mine;
	testG.hand[0][1] = copper;
	testG.hand[0][2] = silver;
	testG.hand[0][3] = gold;
	
	printf("\nCheck if choice1=silver, choice2=copper returns -1\n");
	choice1 = 2; //silver card
	choice2 = copper;
	printf("\nBefore playing Mine card.\n");
	printCards(&testG); //PRE MINE CARD PLAYED
	
	r = cardEffect(mine, choice1, choice2, choice3, &testG, 0, &bonus);
	printf("\nAfter playing Mine card.\n\n");
	printCards(&testG);
	assertTrue(r, -1, &numOfFails, 1);	
	

	printf("______\n");
	
	
	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	testG.handCount[0] = 4; //Player 0 has 4 cards in hand.
	testG.hand[0][0] = mine;
	testG.hand[0][1] = copper;
	testG.hand[0][2] = silver;
	testG.hand[0][3] = gold;
	
	printf("\nCheck if choice1=silver, choice2=silver returns -1\n");
	choice1 = 2; //silver card
	choice2 = silver;
	printf("\nBefore playing Mine card.\n");
	printCards(&testG); //PRE MINE CARD PLAYED
	
	r = cardEffect(mine, choice1, choice2, choice3, &testG, 0, &bonus);
	printf("\nAfter playing Mine card.\n\n");
	printCards(&testG);
	assertTrue(r, -1, &numOfFails, 1);	
	

	printf("______\n");
	
	
	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	testG.handCount[0] = 4; //Player 0 has 4 cards in hand.
	testG.hand[0][0] = mine;
	testG.hand[0][1] = copper;
	testG.hand[0][2] = silver;
	testG.hand[0][3] = gold;
	
	printf("\nCheck if choice1=gold, choice2=gold returns -1\n");
	choice1 = 3; //gold card
	choice2 = gold;
	printf("\nBefore playing Mine card.\n");
	printCards(&testG); //PRE MINE CARD PLAYED
	
	r = cardEffect(mine, choice1, choice2, choice3, &testG, 0, &bonus);
	printf("\nAfter playing Mine card.\n\n");
	printCards(&testG);
	assertTrue(r, -1, &numOfFails, 1);	

	
	printf("______\n");
	
	
	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	testG.handCount[0] = 4; //Player 0 has 4 cards in hand.
	testG.hand[0][0] = mine;
	testG.hand[0][1] = copper;
	testG.hand[0][2] = silver;
	testG.hand[0][3] = gold;
	
	printf("\nCheck if choice1=gold, choice2=silver returns -1\n");
	choice1 = 3; //gold card
	choice2 = silver;
	printf("\nBefore playing Mine card.\n");
	printCards(&testG); //PRE MINE CARD PLAYED
	
	r = cardEffect(mine, choice1, choice2, choice3, &testG, 0, &bonus);
	printf("\nAfter playing Mine card.\n\n");
	printCards(&testG);
	assertTrue(r, -1, &numOfFails, 1);	

	
	printf("______\n");
	
	
	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	testG.handCount[0] = 4; //Player 0 has 4 cards in hand.
	testG.hand[0][0] = mine;
	testG.hand[0][1] = copper;
	testG.hand[0][2] = silver;
	testG.hand[0][3] = gold;
	
	printf("\nCheck if choice1=gold, choice2=copper returns -1\n");
	choice1 = 3; //gold card
	choice2 = copper;
	printf("\nBefore playing Mine card.\n");
	printCards(&testG); //PRE MINE CARD PLAYED
	
	r = cardEffect(mine, choice1, choice2, choice3, &testG, 0, &bonus);
	printf("\nAfter playing Mine card.\n\n");
	printCards(&testG);
	assertTrue(r, -1, &numOfFails, 1);		
	
	
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
	int i, hand;
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
/*	
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
	*/
}



