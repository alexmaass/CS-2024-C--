//
// CS2024 -- Assignment 4
// Exercise 8.19, Deitel & Deitel's "Visual C++ 2008: How to Program"
// Cpp file for Simpletron class
//

#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <string>
#include "SimpletronH.h"

using namespace std;
/**
 **Constructor used to initialize an instance of Simpletron.
 **/
Simpletron::Simpletron() {
	memory = new int[100];
	for (int i=0; i<100; i++){
		memory[i]=0;
	} 
	accumulator = 0;
	instructionCounter = 0;
	operationCode = 0;
	operand = 0;
	instructionRegister = 0;
}

/** The method that will begin taking input from the user and storing it in memory.
 ** It will stop taking input if the user inputs -99999 or if the memory is full. 
 **/
void Simpletron::enterSML() {
	int	instructionNum = 0;			// index in memory
	int input = 0;

	// Display the introduction
	cout << "*** Welcome to Simpletron! ***" << endl;
	cout << "*** Please enter your program one instruction ***" << endl;
	cout << "*** (or data word) at a time. I will type the ***" << endl;
	cout << "*** location number and a question mark (?). ***" << endl;
	cout << "*** You then type the word for that location. ***" << endl;
	cout << "*** Type the sentinel -99999 to stop entering ***" << endl;
	cout << "*** your program. ***" << endl;

	// The input loop to take in instructions and store them in memory. 
	while(instructionNum < 100) {
		// Reading of the input. 
		if (instructionNum < 10){
			cout << 0;
		}
		cout << instructionNum << " ? ";
		cin >> input;		
		// Input is invalid. Prompt user again. 
		while ((input < -9999 && input != -99999) || input > 9999){
			cout << "User input invalid. Please try again." << endl;
			if (instructionNum < 10){
				cout << 0;
			}
			cout << instructionNum << " ? ";
			cin >> input;		
		}
		if (input == -99999){
			break;
		}
		else{
			memory[instructionNum] = input;
			instructionNum++;
		}
	}

	// Display status messages.
	cout << "*** Program loading completed ***" << endl;
	cout << "*** Program execution begins ***" << endl;
}

/**
 ** The function starts executing the instructions stored in the memory
 ** by copying the instruction in memory to the instruction register. 
 **/
void Simpletron::execute() {
	while (instructionCounter < 100) {
		instructionRegister = memory[instructionCounter];
		SML();
	}
}

/**
 ** The main execution function. This function takes the instruction and 
 ** splits it into operand and opcode. The Simpletron will then execute the 
 ** instruction specified by the opcode on the operand. 
 **/
void Simpletron::SML() {
	int tmp;
	// Split the instruction into the opcode and operand.
	operationCode = instructionRegister/100;
	operand = instructionRegister%100;
	++instructionCounter;

	// The switch statement that executes commands based on the opcode. 
	switch (operationCode)
	{
		case READ:
			cout << "Input number (-9999 to +9999): ";
			cin >> tmp;
			while (tmp < -9999 || tmp > 9999){
				error("overflow");
				cin >> tmp;
			}
			memory[operand] = tmp;
			break; 
		case WRITE:
			cout << memory[operand] << endl;
			break;
		case LOAD:
			accumulator = memory[operand];
			break;
		case STORE:
			memory[operand] = accumulator;
			break;
		case ADD:
			if (accumulator + memory[operand] > 9999 || accumulator + memory[operand] < -9999){
				error("overflow");
			}
			else{
				accumulator += memory[operand];
			}
			break;
		case SUBTRACT:
			if (accumulator - memory[operand] > 9999 || accumulator - memory[operand] < -9999){
				error("overflow");
			}
			else{
				accumulator -= memory[operand];
			}
			break;
		case DIVIDE:
			if (memory[operand] == 0){
				error("divide0");
			}
			else{
				accumulator /= memory[operand];
			}
			break;
		case MULTIPLY:
			// check for overflow
			if (accumulator * memory[operand] > 9999 || accumulator * memory[operand] < -9999){
				error("overflow");
			}
			else{
				accumulator *= memory[operand];
			}
			break;
		case BRANCH:
			instructionCounter = operand;
			break;
		case BRANCHNEG:
			if (accumulator < 0)
				instructionCounter = operand;
			break;
		case BRANCHZERO:
			if (accumulator == 0)
				instructionCounter = operand;
			break;
		case HALT:
			cout << "*** Simpletron execution terminated ***" << endl;
			printmem();
			instructionCounter=100;		// set to end of the array to stop executing
			break;
		default:
			cout << "Invalid input";
	}

}

// Simpletron's print memory function to print out everything in the registers and memory. 
void Simpletron::printmem(){
	cout << "REGISTERS:" << endl;
	cout << "accumulator:             " << showpos << setw(5) << setfill('0') << internal << accumulator << endl;
	cout << "instructionCounter:        " << showpos << setw(2) << setfill('0') << internal << instructionCounter << endl;
	cout << "instructionRegister:     " << showpos << setw(5) << setfill('0') << internal << instructionRegister << endl;
	cout << "operationCode:             " << showpos << setw(2) << setfill('0') << internal << operationCode << endl;
	cout << "operand:                   " << showpos << setw(2) << setfill('0') << internal << operand << endl;
	cout << "" << endl;
	cout << "MEMORY:" << endl;
	cout << "   0     1     2     3     4     5     6     7     8     9" << endl;
	// Print out the memory block.
	cout << "00 ";
	for (int i=0; i<10 ; i++){
		cout << showpos << setw(5) << setfill('0') << internal  << memory[i] << " ";
	}
	cout << endl;
	for (int s=10; s<100; s=s+10){
		cout << noshowpos << s << " ";
		for (int i=0; i<10; i++){
			cout << showpos << setw(5) << setfill('0') << internal  << memory[s+i] << " ";
		}
		cout << endl;
	}
}


// The error method. The type of error is passed into the error function as a string to 
// identify what type of error has occured. 
void Simpletron::error(string s){
	if (s.compare("overflow") == 0){
		cout << "An overflow error has occured. The program will now exit..." << endl;
		instructionCounter = 100;
		printmem();
	}
	if (s.compare("divide0") == 0){
		cout << "A divide by zero error has occured. The program will now exit..." << endl;
		instructionCounter = 100;
		printmem();
	}



}