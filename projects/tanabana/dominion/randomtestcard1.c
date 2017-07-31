/*
 * randomtestcard1.c
 */

/******************************
This is a random test for the playCardSmithy function in dominion.c.

Description of Function:
Adds 3 cards from the deck to the player’s hand, and puts the Smithy card from hand into the "played card" pile. 

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



Basic Steps in Random Tester:
1.  Set up the “initial conditions” for the test
-initialize game using initializeGame() function
-randomize number of cards in player's deck and discard
-choose cards in player's deck and discard randomly using rand().
-hard code Smithy card into the hand.

2.  Carry out/execute the action(s) you want to test
-call playCardSmithy()

3.  Check/assert that the desired/expected result(s) were achieved

Test 1: TEST OF ACTION WHEN PLAYER'S HAND, DECK, DISCARD ARE NOT EMPTY, SMITHY CARD IS IN HAND POSITION 0-2 IN A HAND OF 3 CARDS
	-check if hand increased by +3 cards, -1 smithy card.
	-check if deck+discard decreased by -3 cards.
	-check if played card pile increased by 1.
	-check if 3 hand cards is equal to 3 deck cards drawn from the top. (visual check only by printing out all the cards in each pile using printCards() function)
	-check if used smithy card is in the "played cards" pile.
******************************/

/*
 * Include the following lines in your makefile:
 *
 * randomtestcard1: randomtestcard.c dominion.o rngs.o
 *      gcc -o randomtestcard1 -g  randomtestcard1.c dominion.o rngs.o $(CFLAGS)
 */


#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>
#include<time.h>

#define TESTCARD "Smithy"

// set PRINT_CARDS to 0 to remove printfs of hand, deck, and discard from output
#define PRINT_CARDS 0

void assertTrue(int truthCondition, int returnVal, int* numOfFails, int unitTestNum, char *test_type, int test_count);
void cardNumToName(int card, char *name);
void printCards(struct gameState *game);
int chooseRandomCard();

int main(int argc, char **argv) {
	srand(time(NULL));
	
/*
    int newCards = 0;
    int discarded = 1;
    int xtraCoins = 0;
    int shuffledCards = 0;

    int i, j, m;
    
    int remove1, remove2;
*/	
	int i, j;
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
	int numOfFails = 0;
	char test_descript[50];
	int totalCount; //num of cards in hand + discard
	int testruns;
	
	
	if(argc != 2)
	{
		printf("\nInput the number of times to run the random test like this ./randomtest1 <number>\n\n");
		
		return 0;
	}
	testruns =atoi(argv[1]);
	
	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);

	printf("\n----------------- Testing Card: %s ----------------\n", TESTCARD);
	
	/*********
	
	*********/
	
	for(j = 1; j <= testruns; j++) {
		
		// copy the game state to a test case
		memcpy(&testG, &G, sizeof(struct gameState));
	
		//hard code smithy card. hardcode number of cards in hand. randomize the rest.
		testG.handCount[0] = 3; //Player 0 has 3 cards in hand.
		
		testG.hand[0][0] = smithy;
		testG.hand[0][1] = chooseRandomCard();
		testG.hand[0][2] = chooseRandomCard();

		testG.playedCardCount = 0;
		
		//Randomize combined number of cards in deck and discard piles from 3 to 10. We choose not to test case where there are less
		//than 3 cards combined, as that is an unlikely case in a real game situation. In other words, we only test situations in which
		//player will always be able to draw 3 cards after playing Smithy.

		totalCount = rand()%8 + 3; //Choose a number from 3 to 10.
		testG.deckCount[0] = rand()%(totalCount + 1);
		testG.discardCount[0] = totalCount - testG.deckCount[0];

		printf("\ndeckCount = %d\n", testG.deckCount[0]);
		printf("discardCount = %d\n", testG.discardCount[0]);

		for(i = 0; i < testG.deckCount[0]; i++)
		{
			testG.deck[0][i] = chooseRandomCard();
		}

		for(i = 0; i < testG.discardCount[0]; i++)
		{
			testG.discard[0][i] = chooseRandomCard();
		}

		preHandCount = testG.handCount[0];
		preDeckCount = testG.deckCount[0];
		preDiscardCount = testG.discardCount[0];
		prePlayedCardCount = testG.playedCardCount;

#if (PRINT_CARDS == 1)
		printf("\nBefore playing Smithy card.\n");
		printCards(&testG); //PRE SMITHY CARD PLAYED
#endif

		//call playCardSmithy indirectly through the cardEffect function
		//Smithy has no choices to pass in.
		cardEffect(smithy, choice1, choice2, choice3, &testG, handpos, &bonus);

		/***********
		-check if hand increased by +3 cards, -1 smithy card.
		-check if deck decreased by -3 cards.
		-check if played card pile increased by 1.
		-check if 3 hand cards is equal to 3 deck cards drawn from the top.
		-check if used smithy card is in the "played cards" pile.

		***********/

		strcpy(test_descript, "Correct num of cards in hand?");
		printf("\n");
		assertTrue(preHandCount + 3 - 1, testG.handCount[0], &numOfFails, 1, test_descript, j); //test if correct number of cards in hand

		strcpy(test_descript, "Correct num of cards in deck?");
		if(preDeckCount >= 3)
		{
			assertTrue(preDeckCount - 3, testG.deckCount[0], &numOfFails, 1, test_descript, j); //test if correct number of cards in deck in case where deck has 3 or more cards
		}
		else
		{
			assertTrue(preDeckCount + preDiscardCount - 3, testG.deckCount[0], &numOfFails, 1, test_descript, j); //in the case where deck runs out of cards and discard has to be shufflied and placed in deck pile.
		}

		strcpy(test_descript, "Correct num of cards in played?");
		assertTrue(prePlayedCardCount + 1, testG.playedCardCount, &numOfFails, 1, test_descript, j); //test if correct number of cards in "played card" pile

		strcpy(test_descript, "Used Smithy card in played?");
		assertTrue(smithy, testG.playedCards[0], &numOfFails, 1, test_descript, j);

#if (PRINT_CARDS == 1)	
		printf("\nAfter playing Smithy card.\n\n");
		printCards(&testG); //POST SMITHY CARD PLAYED
#endif
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

int chooseRandomCard()
{
	int card;
	card = rand()%27;
	return card;
}

