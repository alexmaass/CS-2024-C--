#include "SimpleArray.cpp"
using namespace std;

/**
 *	Main method for testing the SimpleArray template class.
 */
int main(){
	// Define a constant size for all arrays for easy testing.
	const int size = 5;

	// Create all of test arrays using different types.
	SimpleArray<int,size> intArray;
	SimpleArray<char,size> charArray;
	SimpleArray<double,size> doubleArray;

	// Store instances of the various types in each of the arrays
	for (int i = 0; i<size; i++){
		try{
			intArray[i] = i;
			charArray[i] = (char) (i+97);	// Use asci encoding and casting
			doubleArray[i] = (double) (i + .25);
		}
		catch(int e){
			cout << "An improper index was detected. An ArrayOutofBoundsError has occurred." << endl;
		}
	}
	// Print out everything that was stored in arrays.
	for (int j = 0; j<size; j++){
		try{
			cout << intArray[j] << endl; 
			cout << doubleArray[j] << endl;
			cout << charArray[j] << endl;
		}
		catch(int e){
			cout << "An improper index was detected. An ArrayOutofBoundsError has occurred." << endl;
		}
	}
	// Test the exception handling
	try{
		intArray[-1];
	}
	catch(int e){
			cout << "An improper index was detected. An ArrayOutofBoundsError has occurred." << endl;
	}
	system("Pause");
}