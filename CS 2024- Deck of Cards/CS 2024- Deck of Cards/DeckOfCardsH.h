/* 
 *	Header file for the DeckOfCards class. This file contains the various
 *	constants and class variable and method declarations. The class
 *	is defined in DeckOfCards.cpp. 
 */ 

#include <vector>
#include "CardH.h"


// The main class header for the DeckOfCards class.
class DeckOfCards{
public:
	// Declares the public function
	DeckOfCards();
	void shuffle();
	Card dealCard();
	bool moreCards();
private:
	vector<Card> deck;
	int currentCard;
	void swap(int, int);
};