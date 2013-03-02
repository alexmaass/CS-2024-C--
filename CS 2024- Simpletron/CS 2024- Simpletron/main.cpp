//
// CS2024 -- Assignment 4
// Exercise 8.19, Deitel & Deitel's "Visual C++ 2008: How to Program"
// main function that creates a Simpletron
//

#include <iostream>
#include <string>
#include "SimpletronH.h"

using namespace std;

// Main method to begin inputing and executing code. 
int main (int argc, char *argv[]) 
{
	Simpletron comp = Simpletron();
	
	comp.enterSML();
	comp.execute();
	
	system("Pause");
}
