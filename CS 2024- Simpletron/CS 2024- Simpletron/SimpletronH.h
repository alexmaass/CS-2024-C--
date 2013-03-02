//
// CS2024 -- Assignment 4
// Exercise 8.19, Deitel & Deitel's "Visual C++ 2008: How to Program"
// Header file for Simpletron class
//

#include <iostream>

using namespace std;

const int READ = 10;
const int WRITE = 11;
const int LOAD = 20;
const int STORE = 21;
const int ADD = 30;
const int SUBTRACT = 31;
const int DIVIDE = 32;
const int MULTIPLY = 33;
const int BRANCH = 40;
const int BRANCHNEG = 41;
const int BRANCHZERO = 42;
const int HALT = 43;

// The main class header for the Simpletron class. 
class Simpletron
{
public:
	// declares the public functions
	Simpletron();
	void enterSML();
	void execute();
	void SML();

private:
	// declares the member variables
	void error(string);
	void printmem();
	int* memory;
	int accumulator;
	int instructionCounter;
	int operationCode;
	int operand;
	int instructionRegister;
};