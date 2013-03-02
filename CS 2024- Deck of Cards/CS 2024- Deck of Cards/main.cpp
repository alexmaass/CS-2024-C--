#include <cstdlib>
#include <iostream>
#include <algorithm>
#include "DeckOfCardsH.h"


using namespace std;

// Main method to begin inputing and executing code. 
int main (int argc, char *argv[]){
	// Set up the deck and deal the 5 cards.
	DeckOfCards d = DeckOfCards();
	vector<Card> hand;
	cout << "The cards you have been dealt are: " << endl;
	for (int i=0; i<5; i++){
		hand.push_back(d.dealCard());
		hand[i].toString();
	}

	// Check to see for pair(s). 
	int flag = 0;
	for (int i=0; i<5; i++){
		for (int j = i+1; j<5; j++){
			if ((hand[i].getFace()).compare(hand[j].getFace()) == 0 && i != j){
				flag++;
			} 
		}
	}
	if (flag == 1){
		cout << "A pair has been found!" << endl;
	}
	if (flag > 1){
		cout << "Two pairs have been found!" << endl;
	}
	flag = 0;

	// Check to see for triple. 
	for (int i=0; i<5; i++){
		for (int j = i+1; j<5; j++){
			for (int k= j+1; k<5; k++){
				if ((hand[i].getFace()).compare(hand[j].getFace()) == 0 && (hand[i].getFace()).compare(hand[k].getFace()) == 0 && i != j != k){
					flag++;
				}
			} 
		}
	}
	if (flag){
		cout << "A triple has been found!" << endl;
		flag = 0;
	}

	// Check to see for quadruples. 
	for (int i=0; i<5; i++){
		for (int j = i+1; j<5; j++){
			for (int k= j+1; k<5; k++){
				for (int l = k+1; l<5; l++){
					if ((hand[i].getFace()).compare(hand[j].getFace()) == 0 && (hand[i].getFace()).compare(hand[k].getFace()) == 0 && (hand[i].getFace()).compare(hand[l].getFace()) == 0 && i != j != k != l){
						flag++;
					}
				}
			} 
		}
	}
	if (flag){
		cout << "A quadruple has been found!" << endl;
		flag = 0;
	}

	// Check to see for flush.
	if ((hand[0].getSuit()).compare(hand[1].getSuit()) == 0 && (hand[0].getSuit()).compare(hand[2].getSuit()) == 0 && (hand[0].getSuit()).compare(hand[3].getSuit()) == 0 && (hand[0].getSuit()).compare(hand[4].getSuit()) == 0){
		cout<< "A flush has been found!" << endl;
	}

	// Check for straight. Convert the face values to numbers, store them in a vector, sort the vector, and then check to see if the numbers are consecutive. 
	vector<int> v;
	for (int i=0; i<5; i++){
		for (int j=0; j<13; j++){
			if ((hand[i].getFace()).compare(hand[i].faces[j]) == 0){
				v.push_back(j+1);
			}
		}
	}
	sort(v.begin(),v.end());
	if (v[0] == v[1]-1 && v[0] == v[2]-2 && v[0] == v[3]-3 && v[0] == v[4]-4){
		cout << "A straight has been found!" << endl;
	}

	system("Pause");

}
