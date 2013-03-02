/**
 *	The header file for the Account class. This class declares all of 
 *	the variables and functions found in the Account class. 
 */
#ifndef _AccountH_
#define _AccountH_

class Account{
// All of the public methods and variables available in the class. Includes the constructor.
public:
	Account(double);
	void credit(double);
	bool debit(double);
	double getBalance();

// All of the private methods and variables available in the class. 
private:
	double balance;
};

#endif