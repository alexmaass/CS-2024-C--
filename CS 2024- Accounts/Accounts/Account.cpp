#include "AccountH.h"
#include <iostream>

using namespace std;

/**
 *	The constructor for the Account class. If the initial value provided
 *	is less than zero, an error message will return. 
 */
Account::Account(double n){
	if (n<0){
		balance = 0;
		cout << "Initial balance invalid. Balance set to 0." << endl;
	}
	else{
		balance = n;
	}
}

/**
 *	The credit function adds the specified amount of money to the 
 *	balance of the account. 
 */
void Account::credit(double n){
	balance = balance + n;
}

/**
 *	The debit function subtracts the specified amount of money from 
 *	the balance of the account. If the function tries to withdraw too
 *	much, an error message will return and no money will be withdrawn.
 */
bool Account::debit(double n){
	if (n > balance){
		cout << "Debit amount exceeded account balance." << endl;
		return false;
	} 
	else{
		balance = balance - n;
		return true;
	}
}

/**
 *	The getBalance function returns the current balance in the account.
 */
double Account::getBalance(){
	return balance;
}