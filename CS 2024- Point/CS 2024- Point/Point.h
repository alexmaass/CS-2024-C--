/**
 *	The header file of the Point class for CS 2024: Assignment 9. The Point
 *	class stores the X and Y coordinate values and has overwritten >> and <<
 *	operators to allow easy insertion and extraction of values. 
 */

#include <iostream>
#include <sstream>

using namespace std;

/**
 *	The class declaration and definition of the Point class. 
 */
class Point{
// Public methods and variables. 
public:
	// Default Point constructor. X & Y are set to 0. 
	Point(){
		xCoordinate = 0;
		yCoordinate = 0;
	}
	// Point constructor with specified X and Y coordinates. 
	Point(int x, int y){
		xCoordinate = x;
		yCoordinate = y;
	}
	// Overwritten stream extraction operator. It prints out the point
	// in the (X,Y) format. 
	friend ostream &operator<<(ostream &os, Point &p){
		os << "(" << p.xCoordinate << "," << p.yCoordinate << ")";
		return os;
	}
	// Overwritten stream insertion operator. It stores the X and Y
	// components of the string coordinate point in the xCoordinate and
	// yCoordinate variables. 
	friend istream &operator>>(istream &is, Point &p){
		string str;
		// Convert istream to string.
		is >> str;
		// Convert string to istringstream to allow easy splitting.
		istringstream iss(str);
		int x,y;
		char c1,c2,c3;
		iss >> c1 >> x >> c2 >> y >> c3;
		p.xCoordinate = x;
		p.yCoordinate = y;
		return is;

	}
// Private variables and methods.
private:
	// The X coordinate of the coordinate point
	int xCoordinate;
	// The Y coordinate of the coordinate point
	int yCoordinate;
};

