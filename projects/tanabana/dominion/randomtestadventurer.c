/*
 * randomtestadventurer.c
 */

/******************************
This is a random test for the playCardAdventurer function in dominion.c.

Description:
Reveal cards from your deck until you reveal 2 treasure cards. 
Put those treasure cards into your hand and discard the other revealed cards.

Input:
● struct gameState state – holds a pointer to a gameState variable
● int currentPlayer
● int temphand[MAX_HAND] - keeps track of the cards drawn when playing the Adventurer card
● int drawntreasure - keeps track of the number of treasure cards drawn

Return:
Returns 0 on success

Dependencies:
● shuffle()
● drawCard
	○ shuffle()




Basic Steps in Random Tester:
1.  Set up the “initial conditions” for the test
-initialize game using initializeGame() function
-randomize number of cards in player's deck and discard
-choose cards in player's deck and discard randomly using rand().
-hard code Adventurer card into the hand.

2.  Carry out/execute the action(s) you want to test
-call playCardAdventurer()

3.  Check/assert that the desired/expected result(s) were achieved

Test Cases: TEST OF ACTION WHEN PLAYER'S HAND, DECK, DISCARD ARE NOT EMPTY, DRAWS 2 TREASURE CARDS WHILE STILL DRAWING CARDS
			(GUARANTEED TO FIND 2 TREASURE CARDS)
	-check if hand increased by 2 treasure cards. DONE
	-check if deck + discard decreased by -2 treasure cards. DONE
	-check if discard increased by number of drawn cards - 2. DON'T HAVE ACCESS TO NUM OF DRAWN CARDS
	-check if cards drawn that are to be discarded are all non-treasure cards. DON'T HAVE ACCESS TO NUM OF DRAWN CARDS
	-check if playedCardCount increases by one. DONE
	-check if played adventurer card is in played card pile. DONE
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
#include <time.h>
#include <math.h>

#define TESTCARD "Adventurer"

// set PRINT_CARDS to 0 to remove printfs of hand, deck, and discard from output
#define PRINT_CARDS 0

void assertTrue(int truthCondition, int returnVal, int* numOfFails, int unitTestNum, char *test_type, int test_count);
void cardNumToName(int card, char *name);
void printCards(struct gameState *game);
int chooseRandomCard();
int chooseRandomTreasureCard();
int shuffleDiscard(int player, struct gameState *state);
//int shuffleCards(*cardPile);

int compar(const void* a, const void* b) {
  if (*(int*)a > *(int*)b)  //CASTING VOID POINTER TO TYPE INT AND THEN DEREFERENCING IT TO COMPARE
    return 1;
  if (*(int*)a < *(int*)b)
    return -1;
  return 0;
}

int main(int argc, char **argv) {
	srand(time(NULL));
	
	//set up random number generator - this is the one used in shuffle()
	SelectStream(1);
	PutSeed(-1);
	
	int i, j, m;
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
	int treasureCount;
	int treasureCountHand, treasureCountDeck, treasureCountDiscard;
	int postTreasureCountHand, postTreasureCountDeck, postTreasureCountDiscard;
	//int treasureCards[50];
	
	int treasure_total; //These are variables for building a deck and discard with at least 3 treasure cards.
	int treasure_deck;
	int treasure_discard;
	
	
	
	
	if(argc != 2)
	{
		printf("\nInput the number of times to run the random test like this ./randomtest1 <number>\n\n");
		
		return 0;
	}
	testruns =atoi(argv[1]);
	
	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);

	printf("\n----------------- Testing Card: %s ----------------\n", TESTCARD);
	
	
	for(j = 1; j <= testruns; j++) {
		
		totalCount = 0;
		treasureCount = 0;
		treasureCountHand = 0;
		treasureCountDeck = 0;
		treasureCountDiscard = 0;
		postTreasureCountHand =0;
		postTreasureCountDeck = 0;
		postTreasureCountDiscard = 0;
		
		// copy the game state to a test case
		memcpy(&testG, &G, sizeof(struct gameState));
	
		//hard code adventurer card. hardcode number of cards in hand. randomize the rest.
		testG.handCount[0] = 3; //Player 0 has 3 cards in hand.
		testG.hand[0][0] = adventurer;
		for(i = 1; i < testG.handCount[0]; i++)
		{
			testG.hand[0][i] = chooseRandomCard();
			if(testG.hand[0][i] == copper || testG.hand[0][i] == silver || testG.hand[0][i] == gold)
			{
				treasureCountHand++;
			}
		}
		
		testG.playedCardCount = 0;
		
		//Randomize combined number of cards in deck and discard piles from 3 to 10. We choose not to test case where //there are less than 2 treasure cards combined in deck and discard.

		totalCount = rand()%8 + 3; //Choose a number from 3 to 10.
		testG.deckCount[0] = rand()%(totalCount + 1);
		testG.discardCount[0] = totalCount - testG.deckCount[0];
		
		//Randomize number of treasure cards in deck and discard
		//treasure_total = rand()%(totalCount + 1 - 3) + 3; //has to be at least three, but not over the totalCount
		do 
		{
			treasure_deck = rand()%(testG.deckCount[0] + 1);
			treasure_discard = rand()%(testG.discardCount[0] + 1);
			treasure_total = treasure_deck + treasure_discard;
			
		}while (treasure_total < 3);
		
		printf("\ndeckCount = %d\n", testG.deckCount[0]);
		printf("discardCount = %d\n", testG.discardCount[0]);
		printf("\ntreasure in deck = %d\n", treasure_deck);
		printf("treasure in discard = %d\n", treasure_discard);		

		//Build the deck.
		for(i = 0; i < treasure_deck; i++)
		{
			testG.deck[0][i] = chooseRandomTreasureCard();
		}
		for(i = treasure_deck; i < testG.deckCount[0]; i++)
		{
			testG.deck[0][i] = chooseRandomCard();
		}
		
		//Shuffle the deck.
		shuffle(0, &testG);
		
		
		//Build the discard.
		for(i = 0; i < treasure_discard; i++)
		{
			testG.discard[0][i] = chooseRandomTreasureCard();
		}
		for(i = treasure_discard; i < testG.discardCount[0]; i++)
		{
			testG.discard[0][i] = chooseRandomCard();
		}
		//Shuffle the discard.
		shuffleDiscard(0, &testG);
		
		
		preHandCount = testG.handCount[0];
		preDeckCount = testG.deckCount[0];
		preDiscardCount = testG.discardCount[0];
		prePlayedCardCount = testG.playedCardCount;

#if (PRINT_CARDS == 1)
		printf("\nBefore playing Adventurer card.\n");
		printCards(&testG); //BEFORE ADVENTURER CARD PLAYED
#endif

		//call playCardAdventurer indirectly through the cardEffect function
		//Adventurer has no choices to pass in.
		cardEffect(adventurer, choice1, choice2, choice3, &testG, handpos, &bonus);

		/***********
		Test Case 1: TEST OF ACTION WHEN PLAYER'S HAND, DECK, DISCARD ARE NOT EMPTY, DRAWS 2 TREASURE CARDS BEFORE DECK RUNS OUT
		-DONE check if hand increased by 2 treasure cards.
		-DONE check if deck + discard decreased by -2 treasure cards.
		-CAN'T MEASURE # OF DRAWN CARDScheck if discard increased by number of drawn cards - 2 treasure cards that are kept in the hand.
		-check if cards drawn that are to be discarded are all non-treasure cards (discard added no treasure cards)
		-check if playedCardCount increases by one.
		-check if played adventurer card is in played card pile.

		***********/

		strcpy(test_descript, "Hand increased by 2 cards?");
		printf("\n");
		assertTrue(preHandCount + 2, testG.handCount[0], &numOfFails, 1, test_descript, j); //test if correct number of cards in hand
		
		
		strcpy(test_descript, "The 2 cards are treasures?");
		printf("\n");
		for(m = 0; m < testG.handCount[0]; m++)
		{
			if(testG.hand[0][m] == copper || testG.hand[0][m] == silver || testG.hand[0][m] == gold)
			{
				postTreasureCountHand++;
			}
		}
		assertTrue(treasureCountHand + 2, postTreasureCountHand, &numOfFails, 1, test_descript, j); //test if correct number of cards in hand

		//Count up the number of treasure cards in the deck and in the discard after playing Adventurer card.
		for(i = 0; i < testG.deckCount[0]; i++)
		{
			if(testG.deck[0][i] == copper || testG.deck[0][i] == silver || testG.deck[0][i] == gold)
			{
				postTreasureCountDeck++;
			}
		}
		for(i = 0; i < testG.discardCount[0]; i++)
		{
			if(testG.discard[0][i] == copper || testG.discard[0][i] == silver || testG.discard[0][i] == gold)
			{
				postTreasureCountDiscard++;
			}
		}
		

		strcpy(test_descript, "Deck+Discard decreased by 2 cards?");
		assertTrue(preDeckCount + preDiscardCount - 2, testG.deckCount[0] + testG.discardCount[0], &numOfFails, 1, test_descript, j); //in the case where deck runs out of cards and discard has to be shufflied and placed in deck pile.

		strcpy(test_descript, "The 2 cards are treasures?");
		assertTrue(treasure_deck + treasure_discard - 2, postTreasureCountDeck + postTreasureCountDiscard, &numOfFails, 1, test_descript, j);

		strcpy(test_descript, "Correct num of cards in played?");
		assertTrue(prePlayedCardCount + 1, testG.playedCardCount, &numOfFails, 1, test_descript, j); //test if correct number of cards in "played card" pile

		strcpy(test_descript, "Used Adventurer card in played?");
		assertTrue(adventurer, testG.playedCards[0], &numOfFails, 1, test_descript, j);


#if (PRINT_CARDS == 1)	
		printf("\nAfter playing Adventurer card.\n\n");
		printCards(&testG); //POST ADVENTURER CARD PLAYED
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

int chooseRandomCard() //choose any card except treasure cards
{
	int card;
	do
	{
		card = rand()%27;
		
	}while (card == copper || card == silver || card == gold);
	
	
	return card;
}

int chooseRandomTreasureCard()
{
	int card;
	card = rand()%3 + 4; //Choose num from 4 to 6, the numbers that represent copper, gold, silver.
	return card;
}

//int shuffleCards(*cardPile, cardPileCount)
//{
//	//int cardPile[] = { 100, 56, 80, 2, 25 };
//	//int cardPileCount = 5;
//	int newDeck[MAX_DECK];
//	int newDeckPos = 0;
//	int card;
//	int i;
//	//set up random number generator
//	SelectStream(1);
//	PutSeed(-1);
//	
//	int n;

   //printf("Before sorting the list is: \n");
//   for( n = 0 ; n < 5; n++ ) 
//   {
//      printf("%d ", [n]);
//   }

//	qsort(cardPile, cardPileCount, sizeof(int), compar);
	/* SORT CARDS IN DECK TO ENSURE DETERMINISM! */
   //printf("\nAfter sorting the list is: \n");
//   for( n = 0 ; n < 5; n++ ) 
//   {   
//      printf("%d ", cardPile[n]);
//   }
//
//
//	while (cardPileCount > 0)
//	{
//		card = floor(Random()*cardPileCount);
//		//printf("card = %d\n", card);
//		newDeck[newDeckPos] = cardPile[card];
//		newDeckPos++;
//		for (i = card; i < cardPileCount-1; i++) 
//		{
//			cardPile[i] = cardPile[i+1];
//			//printf("cardPile[%d] = %d\n", i, cardPile[i]);
//		}
//		cardPileCount--;	
//	}
//	for (i = 0; i < newDeckPos; i++) 
//	{
//		cardPile[i] = newDeck[i];
//		cardPileCount++;
//		printf("cardPile[%d] = %d\n", i, cardPile[i]);
//	}
//	return 0;
//}

//Used for the purpose of setting up initial random state
int shuffleDiscard(int player, struct gameState *state) {
 

  int newDiscard[MAX_DECK];
  int newDiscardPos = 0;
  int card;
  int i;

  if (state->discardCount[player] < 1)
    return -1;
  qsort ((void*)(state->discard[player]), state->discardCount[player], sizeof(int), compar); 
  /* SORT CARDS IN DISCARD TO ENSURE DETERMINISM! */

  while (state->discardCount[player] > 0) {
    card = floor(Random() * state->discardCount[player]);
    newDiscard[newDiscardPos] = state->discard[player][card];
    newDiscardPos++;
    for (i = card; i < state->discardCount[player]-1; i++) {
      state->discard[player][i] = state->discard[player][i+1];
    }
    state->discardCount[player]--;
  }
  for (i = 0; i < newDiscardPos; i++) {
    state->discard[player][i] = newDiscard[i];
    state->discardCount[player]++;
  }

  return 0;
}