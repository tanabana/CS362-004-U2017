/*
 * randomtestcard2.c
 */

/******************************
This is a random tester for the playCardMine function in dominion.c.

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


Basic Steps in Random Tester:
1.  Set up the “initial conditions” for the test
-initialize game using initializeGame() function
-hard code discard and deck
-hard code Mine card into the hand
-randomly choose a treasure card to hard code into hand
-set up generation of random numbers of cards in the hand, with non-treasure cards


2.  Carry out/execute the action(s) you want to test
-call playCardMine()

3.  Check/assert that the desired/expected result(s) were achieved

	Randomly choose a card from the hand. 
	-If the card is a treasure card, assert conditions delineated in Test 1, 3, 4, and 5.
	-If card is a non-treasure card, assert conditions delineated in Test 2.
	

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
 * randomtestcard2: randomtestcard2.c dominion.o rngs.o
 *      gcc -o randomtestcard2 -g  randomtestcard2.c dominion.o rngs.o $(CFLAGS)
 */


#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "Mine"
// set PRINT_CARDS to 0 to remove printfs of hand, deck, and discard from output
#define PRINT_CARDS 0

int chooseRandomCard();
void assertTrue(int truthCondition, int returnVal, int* numOfFails, int unitTestNum, char *test_type, int test_count);
void cardNumToName(int card, char *name);
void printCards(struct gameState *game);
int chooseRandomCard();
int chooseRandomTreasureCard();

int main(int argc, char **argv) 
{
	int r; //return value of playCardMine();
	int choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
    int seed = 1000;
    int numPlayers = 2;
    //int thisPlayer = 0;
	struct gameState G, testG;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};

	int numOfFails = 0;
	int i, j;
	int testruns;
	int cardToTrade;
	
	
	if(argc != 2)
	{
		printf("\nInput the number of times to run the random test like this ./randomtest1 <number>\n\n");
		
		return 0;
	}
	testruns =atoi(argv[1]);
	
	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);

	printf("----------------- Testing Card: %s ----------------\n", TESTCARD);
	

	for(j = 1; j <= testruns; j++ )
	{
		// copy the game state to a test case
		memcpy(&testG, &G, sizeof(struct gameState));

		testG.handCount[0] = rand()%8 + 3; //Generate random number of cards in the hand from 3 to 10.
		testG.hand[0][0] = mine;
		testG.hand[0][1] = chooseRandomTreasureCard(); //Guarantee at least one treasure card.

		for(i = 2; i < testG.handCount[0] - 1; i++)
		{
			testG.hand[0][i] = chooseRandomCard();
		}

		//Choose a card randomly to play with the mine card.
		choice1 = rand()%(testG.handCount[0] - 1) + 1; //Don't choose mine card.
		choice2 = rand()%(35) - 5; //Choose any card to trade it with. Include out of range (nonexistent cards)
		
		cardToTrade = testG.hand[0][choice1];
		
		char card[32];
		cardNumToName(testG.hand[0][choice1], card);
		printf("choice1 = %s\n", card);
		
		cardNumToName(choice2, card);
		printf("choice2 = %s\n", card);

#if (PRINT_CARDS == 1)	
		printf("\nBefore playing Mine card.\n\n");
		printCards(&testG);
#endif
		
		//call playCardMine indirectly through the cardEffect function
		r = cardEffect(mine, choice1, choice2, choice3, &testG, 0, &bonus);

#if (PRINT_CARDS == 1)	
		printf("\nAfter playing Mine card.\n\n");
		printCards(&testG);
#endif		
		if(cardToTrade == copper || cardToTrade == silver || cardToTrade == gold)
		{
			if(cardToTrade == copper)
			{
				//printf("IN COPPER\n");
				switch(choice2){
				case copper: assertTrue(r, 1, &numOfFails, 1, "copper for copper", j);
					break;
				case silver: assertTrue(r, 1, &numOfFails, 1, "copper for silver", j);
					break;
				case gold: assertTrue(r, -1, &numOfFails, 1, "copper for gold", j);
					break;

				default: assertTrue(r, -1, &numOfFails, 1, "copper for non-treasure", j);
				}
			}
		
			if(cardToTrade == silver)
			{
				//printf("IN SILVER\n");
				switch(choice2)
				{
				case copper: assertTrue(r, 1, &numOfFails, 1, "silver for copper", j);
					break;
				case silver: assertTrue(r, 1, &numOfFails, 1, "silver for silver", j);
					break;
				case gold: assertTrue(r, 1, &numOfFails, 1, "silver for gold", j);
					break;

				default: assertTrue(r, -1, &numOfFails, 1, "silver for non-treasure", j);
				}			
			}

			if(cardToTrade == gold)
			{
				//printf("IN GOLD\n");
				switch(choice2)
				{
				case copper: assertTrue(r, 1, &numOfFails, 1, "gold for copper", j);
					break;
				case silver: assertTrue(r, 1, &numOfFails, 1, "gold for silver", j);
					break;
				case gold: assertTrue(r, 1, &numOfFails, 1, "gold for gold", j);
					break;

				default: assertTrue(r, -1, &numOfFails, 1, "gold for non-treasure", j);
				}			
			}
		}
		else
		{
			//printf("IN OTHER\n");
			assertTrue(r, -1, &numOfFails, 1, "non-treasure for any card", j);
		}
		
		if(choice2 < curse || choice2 > treasure_map)
		{
			assertTrue(r, -1, &numOfFails, 1, "non-existent card", j);
		}
	}
	
	printf("\n----------------- TESTS FAILED = %d -----------------\n\n", numOfFails);
	return 0;
}


//Counts number of failed tests.
void assertTrue(int truthCondition, int returnVal, int* numOfFails, int unitTestNum, char *test_type, int test_count)
{
	printf("%s\t", test_type);
	printf("TEST %d:", test_count);
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
//void printCards(struct gameState *game)
//{
//	int i, hand;
//	char cardName[32];
//	printf("\n");
//	if(game->handCount[0] > 0)
//	{
//		for(i = 0; i < game->handCount[0]; i++)
//		{
//			hand = game->hand[0][i];
//			//h = hand;
//			cardNumToName(hand, cardName);
//			printf("Hand card %d = %s\n", i, cardName);
//
//
//		}
//	}
//	else
//	{
//		printf("Hand is empty.\n");
//	}
//	printf("\n");
//}

//Prints out the cards for hand, deck and discard in order.
void printCards(struct gameState *game)
{
	int i, deck, hand, discard, played_card;
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

	if(game->playedCardCount > 0)
	{
		for(i = 0; i < game->playedCardCount; i++)
		{
			played_card = game->playedCards[i];
			cardNumToName(played_card, cardName);

			printf("\nPlayed card %d = %s\n", i, cardName);
		}
	}
	else
	{
		printf("\nPlayed card pile is empty.\n");
	}
	
}

int chooseRandomTreasureCard()
{
	int card;
	card = rand()%3 + 4; //Choose num from 4 to 6, the numbers that represent copper, gold, silver.
	return card;
}

int chooseRandomCard() //This includes randomly choosing both non-treasure and treasure card.
{
	int card;
	card = rand()%27;
	return card;
}