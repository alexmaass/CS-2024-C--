//#ifndef S_H
//#define S_H

#include "AccountH.h"

/**
 *	The header file for the SavingsAccount class. This class declares all of 
 *	the variables and functions found in the SavingsAccount class. The SavingsAccount
 *	class inherits all variables and functions from the Account class.
 */

class SavingsAccount : public Account{
// All of the public methods and variables available in the class. Includes the constructor.
public:
	SavingsAccount(double, double);
	double calculateInterest();

// All of the private methods and variables available in the class. 
private:
	double interestRate;
}; 

//#endif