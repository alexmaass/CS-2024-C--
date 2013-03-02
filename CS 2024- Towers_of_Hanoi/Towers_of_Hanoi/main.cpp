#include <iostream>
#include <cstdlib>
using namespace std;

void Towers_of_Hanoi(int discs, int pega, int pegb, int pegc){
	if (discs == 1){
		cout << "Move: " << pega << " -> " << pegb << endl;
	}
	else {
		Towers_of_Hanoi(discs-1, pega, pegc, pegb);
		cout << "Move " << pega << " -> " << pegb << endl;
		Towers_of_Hanoi(discs-1, pegc, pegb, pega);
	}
}

int main(int argc, char *argv[]){
	int num;
	int pa;
	int pb;
	int pc;
	cout << "Input the number of discs you would like to move: " << endl;
	cin >> num;
	cout << "Input the peg number which starts off with the discs: " << endl;
	cin >> pa;
	cout << "Input the peg number that will holds the discs at the end: " << endl;
	cin >> pb;
	cout << "Input the temporary peg number: " << endl;
	cin >> pc;
	Towers_of_Hanoi(num, pa, pb, pc);
	system("Pause");
}