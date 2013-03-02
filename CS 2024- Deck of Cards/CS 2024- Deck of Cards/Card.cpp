/* 
 *	CPP file for the cards class. This file contains the various
 *	constants and class variable and method definitions. The class
 *	is declared in CardsH.h. 
 *
 */
#include "CardH.h"
#include <iostream>
#include <cstdio>
using namespace std;

string Card::suits[4] = {"DIAMONDS","CLUBS","HEARTS","SPADES"};
string Card::faces[13] = {"ACE", "TWO", "THREE", "FOUR", "FIVE", "SIX", "SEVEN", "EIGHT", "NINE", "TEN", "JACK", "QUEEN", "KING"};


/*
 *	Constructor used to initialize an instance of Cards.
 */
Card::Card(int type, int num){
	suit = suits[type];
	face = faces[num];
}

/*
 *	The toString function prints out the card's face and suit. 
 */ 
void Card::toString(){
	string printout = face + " of " + suit;
	cout << printout << endl;
//	cout << face << " of " << suit;
}

/*
 *	The getFace function prints out the card's face.
 */
string Card::getFace(){
	return face;
}

/*
 *	The getSuit function prints out the card's suit.
 */
string Card::getSuit(){
	return suit;
}