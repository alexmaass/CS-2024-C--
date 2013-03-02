#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

/*
 *	The main function for CS 2024: Assignment 10. The purpose of this assignment is 
 *	to use the sizeof function and the stream operators to find out the sizes of various
 *	types and write them to a file called datasize.dat. Then the file should be read and
 *	everything written to the file should be printed out. 
 */
int main(){
	// Declaring the output file stream.
	ofstream outStream("datasize.dat");
	// Setting the max number of characters per line.
	int lineoffset = 30;
	// Check to see if the file successfully opened.
	if (!outStream.is_open()){
		cout << "File failed to open. Nothing was written." << endl;
		return -1;
	}
	// Begin writing the type and the size of the type to the output file.
	outStream << "char" << setw(lineoffset-sizeof("char")) << sizeof(char) << endl;
	outStream << "unsigned char" << setw(lineoffset-sizeof("unsigned char")) << sizeof(unsigned char) << endl;
	outStream << "short int" << setw(lineoffset-sizeof("short int"))<< sizeof(short int) << endl;
	outStream << "unsigned short int" << setw(lineoffset-sizeof("unsigned short int"))<< sizeof(unsigned short int) << endl;
	outStream << "int" << setw(lineoffset-sizeof("int"))<< sizeof(int) << endl;
	outStream << "unsigned int" << setw(lineoffset-sizeof("unsigned int"))<< sizeof(unsigned int) << endl;
	outStream << "long int" << setw(lineoffset-sizeof("long int"))<< sizeof(long int) << endl;
	outStream << "unsigned long int" << setw(lineoffset-sizeof("unsigned long int"))<< sizeof(unsigned long int) << endl;
	outStream << "float" << setw(lineoffset-sizeof("float"))<< sizeof(float) << endl;
	outStream << "double" << setw(lineoffset-sizeof("double"))<< sizeof(double) << endl;
	outStream << "long double" << setw(lineoffset-sizeof("long double"))<< sizeof(long double) << endl;
	// Clearing the input file stream to read the written file.
	ifstream inStream("datasize.dat");
	// Check to see if the file successfully opened.
	if (!inStream.is_open()){
		cout << "File failed to open. Nothing was read." << endl;
		return -1;
	}
	// Print out what is stored in the file by way of a buffer.
	cout << inStream.rdbuf();
	system("Pause");
}
