#include "SavingsAccountH.h"

/**
 *	The constructor for the SavingsAccount class. If the initial balance provided
 *	is less than zero, an error message will return. 
 */
SavingsAccount::SavingsAccount(double bal, double per) : Account(bal){
	interestRate = per;
}

/**
 *	The calculateInterest function returns the amount of interest the account
 *	has earned. It is calculated by multiplying the interest rate by the balance
 *	of the account. 
 */
double SavingsAccount::calculateInterest(){
	return interestRate*getBalance();
}

