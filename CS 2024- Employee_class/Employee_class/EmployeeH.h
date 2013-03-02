// EmployeeH.h - - Header file for Employee Class
 
#include <string>							// including the string package to allow usage of the string type.
using std::string;							// setting up the string package to allow direct usage of the type

class Employee{

public:										// All the public methods available in the class. Include setters, getters and constructors
	Employee(string, string, int);
	void setFirstName(string);
	string getFirstName();
	void setLastName(string);
	string getLastName();
	void setSalary(int);
	int getSalary();
	
private:									// All the private variables in the class. These variables will hold the data of the employee.
	string fname;
	string lname;
	int salary;

};