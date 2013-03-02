/**
 *	The main method file for CS 2024: Assignment 9. The assignment is to create
 *	and test a Point class and modify its stream insertion and extraction operators.
 */
#include "Point.h"
#include <fstream>
#include <string>

// The main method for the assignment. The main method reads input from the input.dat
// file and stores each coordinate point in an instance of the Point class. The values
// of the Point instance are then printed out. 
int main(){
	Point p;
	string s;
	// Create the input file stream. 
	ifstream in("input.dat");
	// Check to see if the stream is opened. 
	if (!in.is_open()){
		cout << "Couldn't open input file" << endl;
		return -1;
	}
	// For each space delimited point, store the values in a Point
	// and print out the values. 
	while(in){
		if (in.failbit !=0){
			in >> p;
			cout << p << endl;
		}
	}
	system("Pause");

}