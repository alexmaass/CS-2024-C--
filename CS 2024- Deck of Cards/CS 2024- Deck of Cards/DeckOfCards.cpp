/* 
 *	CPP file for the DeckOfCards class. This file contains the various
 *	constants and class variable and method declarations. The class
 *	is defined in DeckOfCards.h. 
 */ 

#include "DeckOfCardsH.h"
#include <iostream>
#include <cstdlib>
#include <time.h>

using namespace std;
/*
 *	Constructor used to initialize an instance of DeckOfCards.
 */
DeckOfCards::DeckOfCards() {
	currentCard=0;
	// Iterate through the different suits
	for (int i=0; i<4; i++){
		// Iterate through the different face values
		for (int j=0; j<13; j++){
			Card c = Card(i,j);
			deck.push_back(c);
		}
	}
	// Shuffle the cards.
	shuffle();
}

/*
 *	Shuffle function to shuffle all of the cards in the deck.
 */
void DeckOfCards::shuffle(){
	srand(time(NULL));
	for (int i=0; i<52; i++){
		int s = rand() % 52;
		swap(i,s);
	}
}

/*
 *	The swap function swaps two cards in the deck. The card at position a
 *	is moved to position b and the card at position b is moves to position a. 
 */ 
void DeckOfCards::swap(int a, int b){
	Card tmp = deck[b];
	deck[b] = deck[a];
	deck[a] = tmp;
}

/*
 *	The dealCard function deals the next Card from the deck.
 */
Card DeckOfCards::dealCard(){
	currentCard++;
	if (moreCards()){
		return deck[currentCard - 1];
	}
	else{
		cout << "The deck is empty." << endl;
	}
}

/*
 *	The moreCards function checks to see if there are more Cards left to deal.
 */
bool DeckOfCards::moreCards(){
	if (currentCard < 52){
		return true;
	}
	else{
		return false;
	}

}