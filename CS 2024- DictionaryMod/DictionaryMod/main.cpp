#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <sstream>

using namespace std;
static vector<string> dictionary;
static float size;

int main(){
	time_t start, end;
	double dif;
	string line;
	long fsize;
	char* buffer;
	size_t result;
	stringstream trimmer;

	cout << "Conversion is beginning." << endl;

	// Open the input file stream for conversion.
	ifstream in("enable.txt");
	if (!in.is_open()){
		cout << "Unable to open dictionary source file. Program will now exit." << endl;
		system("Pause");
		exit(1);
	}
	
	// Open the output file stream for conversion. 
	ofstream out ("dictionary.txt");
	if(!out.is_open()){
		cout << "Unable to open destination file. Program will now exit." << endl;
		system("Pause");
		exit(1);
	}

	// Start the timer. 
	time(&start);
	
	// Main loop to convert words to fixed length. 
	while(in.good()){
		getline(in,line);
		while (line.length() < 31){
			line = line + " ";
		}
		line = line + '\n';
		out << line;
	}
	
	// Close the files. 
	in.close();
	out.close();

	// End the timer and print out the time. 
	time(&end);
	dif = difftime(end,start);
	cout << "It took " << dif << " seconds to convert the file." << endl;


	/*
	Start of file reading.
	*/
	
	/*
	// Open the converted dictionary file. 
	FILE * dictionary;
	dictionary = fopen("dictionary.txt", "r");
	if (dictionary == NULL){
		cout << "Unable to open dictionary file. Program will now exit." << endl;
		system("Pause");
		exit(1);
	}
	// Find the size of the file. 
	fseek(dictionary,0,SEEK_END);
	fsize = ftell(dictionary);
	rewind(dictionary);

	// Allocate memory for the buffer.
	buffer = (char*) malloc(sizeof(char)*28);
	if(buffer == NULL){
		cout << "Unable to allocate memory. Program will now exit." << endl;
		system("Pause");
		exit(1);
	}
	
	for(int i = 0; i<10; i++){
		// Read the file at the specified spot. 
		fseek(dictionary,(i*56) + 1,SEEK_SET);
		result = fread(buffer,1,56,dictionary);

		// Try to remove the weird spaces.
		trimmer << buffer;
		((string)buffer).clear();
		trimmer >> buffer;

		// Print out the buffer. 
		//if(i%2 == 0){
			cout << "Finished reading from the file. Result is printed below." << endl;
			cout << buffer << endl;
		//}
	}

	fclose(dictionary);
	free(buffer);

	system("Pause");

	*/
	// Open the converted file stream.
	ifstream inn("enable.txt");
	if (!inn.is_open()){
		cout << "Unable to open dictionary source file. Program will now exit." << endl;
		system("Pause");
		exit(1);
	}
	for(int i=0;i<10;i++){
		getline(inn,line);
		cout << line << "." << endl;
	}
	cout << ' ' << endl;
	for(int i=0;i<10;i++){
		inn.seekg(i*32);
		getline(inn,line);
		cout << line << "." << endl;
	}

	system("Pause");








}