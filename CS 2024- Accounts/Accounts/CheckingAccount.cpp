#include "CheckingAccountH.h"
#include <iostream>

using namespace std;

/**
 *	The constructor for the CheckingAccount class. If the initial balance provided
 *	is less than zero, an error message will return. 
 */
CheckingAccount::CheckingAccount(double bal, double fee) : Account(bal){
	transactionFee = fee;
}

/**
 *	The credit function adds the specified amount of money to the 
 *	balance of the account. It then subtracts the amount of money 
 *	specified by the transaction fee.
 */
void CheckingAccount::credit(double n){
	Account::credit(n);
	Account::debit(transactionFee);
}

/**
 *	The debit function subtracts the specified amount of money from 
 *	the balance of the account. If the function tries to withdraw too
 *	much, an error message will return and no money will be withdrawn.
 *	If money is successfully deducted, a transaction fee will then be
 *	charged. 
 */
bool CheckingAccount::debit(double n){
	if (Account::debit(n)){
		Account::debit(transactionFee);
		return true;
	}
	else{
		return false;
	}
}