//#ifndef C_H
//#define C_H

#include "AccountH.h"

/**
 *	The header file for the CheckingAccount class. This class declares all of 
 *	the variables and functions found in the CheckingAccount class. The CheckingAccount
 *	class inherits all variables and functions from the Account class.
 */
class CheckingAccount : public Account{
// All of the public methods and variables available in the class. Includes the constructor.
public:
	CheckingAccount(double, double);
	void credit(double);
	bool debit(double);

// All of the private methods and variables available in the class. 
private:
	double transactionFee;
}; 

//#endif