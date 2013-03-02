#include "EmployeeH.h"								// Includes the header file to link to the EmployeeCPP file. 
#include <iostream>
#include <cstdlib>
using namespace std;

int main(int argc, char *argv[]){
	// Sets up the two instances of the employee class using the constructor. 
	Employee e1 = Employee( "Barack", "Obama", 100000);
	Employee e2 = Employee( "Mitt", "Romney", 500000);
	// Prints out the names and salaries of the employees.
	cout << "The first employee is: " << e1.getFirstName() << " " << e1.getLastName() << ". His yearly salary is: "<< e1.getSalary() << endl;
	cout << "The second employee is: " << e2.getFirstName() << " " << e2.getLastName() << ". His yearly salary is: " << e2.getSalary() << endl;
	cout << "Both employees are getting a 10% raise!" << endl;
	// Raises the salary of each employee by 10%.
	e1.setSalary(1.1*e1.getSalary());
	e2.setSalary(1.1*e2.getSalary());
	// Prints out the new salary and name for each employee.
	cout << e1.getFirstName() << " " << e1.getLastName() << "'s new yearly salary is: " << e1.getSalary() << endl;
	cout << e2.getFirstName() << " " << e2.getLastName() << "'s new yearly salary is: " << e2.getSalary() << endl;
	// Waits until a key is pressed to close.
	system("Pause");
	return 0;
}
