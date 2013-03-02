/* 
 *	Header file for the Card class. This file contains the various
 *	constants and class variable and method declarations. The class
 *	is defined in Card.cpp. 
 */ 

#include <string>
#include <vector>

using namespace std;

// The main class header for the Card class.
class Card{
public:
	// declares the public functions
	Card(int, int);
	void toString(); 
	static string suits[4];
	static string faces[13];
	string getFace();
	string getSuit();
private:
	// declares the private fucntions. 
	string suit;
	string face;
};