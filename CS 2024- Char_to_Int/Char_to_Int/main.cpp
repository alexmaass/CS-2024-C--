#include <iostream>

using namespace std;

int main(int argc, char *argv[]){
	char l;
	cout << "Please input a character to convert to an integer." << endl;
	cin >> l;
	cout << "The integer value of " << l << " is: " << endl;
	cout << static_cast<int>(l) << endl;
	system("PAUSE");	
}