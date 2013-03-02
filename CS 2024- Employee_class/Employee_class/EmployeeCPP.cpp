#include "EmployeeH.h"							// Including the employeeH.h header file 

// The constructor for the employee class. If the salary is given as negative, it should be rounded up to 0. 
Employee::Employee(string f, string l, int s){	
	fname= f;									
	lname= l;
	if (s < 0){
		salary= 0;
	}
	else{
		salary= s;
	}
}

// Getter method for the employee's first name.
string Employee::getFirstName(){				
	return fname;
}

// Setter method for the employee's first name. 
void Employee::setFirstName(string fn){			
	fname= fn;
}

// Getter method for the employee's last name. 
string Employee::getLastName(){					
	return lname;
}

// Setter method for the employee's last name.
void Employee::setLastName(string ln){			 
	lname= ln;
}

// Getter method for the employee's salary.
int Employee::getSalary(){
	return salary;
}

// Setter method for the employee's salary. If the salary given is less than 0, round it up to 0. 
void Employee::setSalary(int sn){
	if (sn < 0){
		salary= 0;
	}
	else{
		salary= sn;
	}
}
