/**
 *	The main method for the testing demonstration. In this main method,
 *	various instances of the different accounts will be created and 
 *	initialized. Then different functions will be performed on the instances
 *	to prove that they work as intended.
 */

#include "CheckingAccountH.h"
#include "SavingsAccountH.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[]){
	// Create different accounts.
	Account acc = Account(100);
	CheckingAccount cacc = CheckingAccount(100, 1);
	SavingsAccount sacc = SavingsAccount(100, .1);

	// Try to create an account with a negative balance.
	Account negacc = Account(-111);

	// Test debit, credit, and get balance of Account class.
	cout<< "Account Acc has " << acc.getBalance() << " in account." << endl;
	acc.debit(50);
	cout << "Acc should have been debited 50. The resulting balance is " << acc.getBalance() << endl;
	acc.credit(50);
	cout << "Acc should have been credited 50. The resulting balance is " << acc.getBalance() << endl;
	// Test debit to deal with a number greater than the balance.
	acc.debit(111);
	cout << "Acc should have been over-debited 111. The resulting balance is " << acc.getBalance() << endl;

	// Test debit and credit of CheckingAccount class.
	cout << "Checking account Cacc has " << cacc.getBalance() << " in checking account." << endl;
	cacc.debit(50);
	cout << "Cacc should have been debited 50. The resulting balance is " << cacc.getBalance() << endl;
	cacc.credit(50);
	cout << "Cacc should have been credited 50. The resulting balance is " << cacc.getBalance() << endl;

	// Test calculateInterest of SavingsAccount class.
	cout << "Savings account Sacc has interest of .1 and a balance of " << sacc.getBalance() << endl;
	sacc.credit(sacc.calculateInterest());
	cout << "After crediting the interest, Sacc now has a balance of " << sacc.getBalance() << endl;

	system("Pause");
}