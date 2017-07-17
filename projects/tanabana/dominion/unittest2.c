#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

/******************************
This is a unit test for the fullDeckCount function in dominion.c.

Description:
Returns the count of a specific card in a specific player’s discard deck and hand.
Input:
● struct gameState state – holds a pointer to a gameState variable
● int card
● int player
Return:
Returns count
Data structures:
● int i
● int count
Dependencies:
● none


Basic Steps in Unit Test:
1.  Set up the “initial conditions” for the test
-use initializeGame() to set up game for 2 players.
-set both hand and deck count of player 1
-set entire deck and hand of player 1 of this game so that all the cards are known by the test
2.  Carry out/execute the action(s) you want to test
-call the function and compare to set (known) card count values
3.  Check/assert that the desired/expected result(s) were achieved 
-check/assert that the counts are the same

Test 1a: Check edge cases (first card in player's hand and last card in player's hand)
Test 1b: Check edge cases (first card in player's deck and last card in player's deck)

Test 2: Check non-edge cases

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



int main()
{
    int seed = 1000;
    int numPlayer = 2;
    int r;   
    int knownCount;
    int testCount;
    int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};
    struct gameState G;
    

   // memset(&G, 23, sizeof(struct gameState));   // clear the game state
    r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
    
    //TEST 1: EDGE CASE. Fixed player 0's set to compare to fullDeckCount return value
    G.handCount[0] = 5;
    G.hand[0][0] = curse;
    G.hand[0][1] = estate;
    G.hand[0][2] = duchy;
    G.hand[0][3] = province;
    G.hand[0][4] = curse;
    
    G.deckCount[0] = 5;
    G.deck[0][0] = curse;
    G.deck[0][1] = estate;
    G.deck[0][2] = duchy;
    G.deck[0][3] = province;
    G.deck[0][4] = curse;  
    
    G.discardCount[0] = 5;
    G.discard[0][0] = curse;
    G.discard[0][1] = estate;
    G.discard[0][2] = duchy;
    G.discard[0][3] = province;
    G.discard[0][4] = curse;  
    
    knownCount = 6; //total number of curse cards
    testCount = fullDeckCount(0, curse, &G);
    printf("knownCount = %d, \t testCount = %d\n", knownCount, testCount);
    
//TEST 2: EDGE CASE & NON-EDGE COMBINED.
    G.handCount[0] = 5;
    G.hand[0][0] = curse;
    G.hand[0][1] = estate;
    G.hand[0][2] = curse;
    G.hand[0][3] = province;
    G.hand[0][4] = curse;
    
    G.deckCount[0] = 5;
    G.deck[0][0] = curse;
    G.deck[0][1] = curse;
    G.deck[0][2] = duchy;
    G.deck[0][3] = province;
    G.deck[0][4] = curse;  
    
    G.discardCount[0] = 5;
    G.discard[0][0] = curse;
    G.discard[0][1] = estate;
    G.discard[0][2] = duchy;
    G.discard[0][3] = curse;
    G.discard[0][4] = curse;  
    
    knownCount = 9; //total number of curse cards
    testCount = fullDeckCount(0, curse, &G);
    printf("knownCount = %d, \t testCount = %d\n", knownCount, testCount);
    
//TEST 3: NON-EDGE ONLY.
    G.handCount[0] = 5;
    G.hand[0][0] = estate;
    G.hand[0][1] = estate;
    G.hand[0][2] = curse;
    G.hand[0][3] = province;
    G.hand[0][4] = province;
    
    G.deckCount[0] = 5;
    G.deck[0][0] = duchy;
    G.deck[0][1] = curse;
    G.deck[0][2] = duchy;
    G.deck[0][3] = province;
    G.deck[0][4] = duchy;  
    
    G.discardCount[0] = 5;
    G.discard[0][0] = province;
    G.discard[0][1] = estate;
    G.discard[0][2] = duchy;
    G.discard[0][3] = curse;
    G.discard[0][4] = estate;  
    
    knownCount = 3; //total number of curse cards
    testCount = fullDeckCount(0, curse, &G);
    printf("knownCount = %d, \t testCount = %d\n", knownCount, testCount);
    
    //TEST 4: ZERO CARDS.
    G.handCount[0] = 5;
    G.hand[0][0] = estate;
    G.hand[0][1] = estate;
    G.hand[0][2] = curse;
    G.hand[0][3] = province;
    G.hand[0][4] = province;
    
    G.deckCount[0] = 5;
    G.deck[0][0] = duchy;
    G.deck[0][1] = curse;
    G.deck[0][2] = duchy;
    G.deck[0][3] = province;
    G.deck[0][4] = duchy;  
    
    G.discardCount[0] = 5;
    G.discard[0][0] = province;
    G.discard[0][1] = estate;
    G.discard[0][2] = duchy;
    G.discard[0][3] = curse;
    G.discard[0][4] = estate;  
    
    knownCount = 0; //total number of mine cards
    testCount = fullDeckCount(0, mine, &G);
    printf("knownCount = %d, \t testCount = %d\n", knownCount, testCount);
    
/*    printf ("TESTING fullDeckCount:\n");
    for (p = 0; p < numPlayer; p++)
    {
        for (numOfCardsInHand = 1; numOfCardsInHand <= maxHandCount; numOfCardsInHand++)
        {
            for (numOfCardsInDeck = 1; numOfCardsInDeck <= maxDeckCount; numOfCardsInDeck++)
            {
            printf("Test player %d with %d num of card(s) in hand, %d num of cards in deck num of cards in discard.\n", p, numOfCardsInHand, numOfCardsInDeck, numOfCardsInDiscard);

            memset(&G, 23, sizeof(struct gameState));   // clear the game state
            r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
            G.handCount[p] = numOfCardsInHand;                 // set the number of cards on hand
            memcpy(G.hand[p], coppers, sizeof(int) * handCount); // set all the cards to copper 
            updateCoins(p, &G, bonus);

            printf("G.coins = %d, expected = %d\n", G.coins, handCount * 1 + bonus);

            assert(G.coins == handCount * 1 + bonus); // check if the number of coins is correct

            memcpy(G.hand[p], silvers, sizeof(int) * handCount); // set all the cards to silver
            updateCoins(p, &G, bonus);

            printf("G.coins = %d, expected = %d\n", G.coins, handCount * 2 + bonus);

            assert(G.coins == handCount * 2 + bonus); // check if the number of coins is correct

            memcpy(G.hand[p], golds, sizeof(int) * handCount); // set all the cards to gold
            updateCoins(p, &G, bonus);

            printf("G.coins = %d, expected = %d\n", G.coins, handCount * 3 + bonus);

            assert(G.coins == handCount * 3 + bonus); // check if the number of coins is correct
        }
    }
    return 0;
}*/




// set NOISY_TEST to 0 to remove printfs from output
#define NOISY_TEST 1

/*
int main() {
    int i;
    int seed = 1000;
    int numPlayer = 2;
    int maxBonus = 10;
    int p, r, handCount;
    int bonus;
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
    struct gameState G;
    int maxHandCount = 5;
    // arrays of all coppers, silvers, and golds
    int coppers[MAX_HAND];
    int silvers[MAX_HAND];
    int golds[MAX_HAND];
    for (i = 0; i < MAX_HAND; i++)
    {
        coppers[i] = copper;
        silvers[i] = silver;
        golds[i] = gold;
    }

    printf ("TESTING updateCoins():\n");
    for (p = 0; p < numPlayer; p++)
    {
        for (handCount = 1; handCount <= maxHandCount; handCount++)
        {
            for (bonus = 0; bonus <= maxBonus; bonus++)
            {
                printf("Test player %d with %d treasure card(s) and %d bonus.\n", p, handCount, bonus);

                memset(&G, 23, sizeof(struct gameState));   // clear the game state
                r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
                G.handCount[p] = handCount;                 // set the number of cards on hand
                memcpy(G.hand[p], coppers, sizeof(int) * handCount); // set all the cards to copper
                updateCoins(p, &G, bonus);

                printf("G.coins = %d, expected = %d\n", G.coins, handCount * 1 + bonus);

                assert(G.coins == handCount * 1 + bonus); // check if the number of coins is correct

                memcpy(G.hand[p], silvers, sizeof(int) * handCount); // set all the cards to silver
                updateCoins(p, &G, bonus);

                printf("G.coins = %d, expected = %d\n", G.coins, handCount * 2 + bonus);

                assert(G.coins == handCount * 2 + bonus); // check if the number of coins is correct

                memcpy(G.hand[p], golds, sizeof(int) * handCount); // set all the cards to gold
                updateCoins(p, &G, bonus);

                printf("G.coins = %d, expected = %d\n", G.coins, handCount * 3 + bonus);

                assert(G.coins == handCount * 3 + bonus); // check if the number of coins is correct
            }
        }
    }

    printf("All tests passed!\n");
*/
    return 0;
}

