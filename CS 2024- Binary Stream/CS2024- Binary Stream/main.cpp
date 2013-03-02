/* The main.cpp file for the bonus assignment. This files contains a custom "sticky" stream manipulator
which converts all following ints into a binary format. 
Name: Alex Maass
NetID: am838
*/

#include <bitset>
#include <iostream>
#include <limits>
#include <locale>

// The index location used to store the iword state. 
int index = std::ios_base::xalloc();

// The overriden num_put class from the inherited num_put found in the standard libraries. 
// If the state is turned on, it changes the output stream into the binary format. 
class num_put : public std::num_put<char>{
    iter_type do_put(iter_type next, std::ios_base& iosbase, char_type fill, long val){
		// If the state is 0 (off), do the normal num_put call to the superclass. 
        if (iosbase.iword(index) == 0) {
            return std::num_put<char>::do_put(next, iosbase, fill, val);
        }
		// If the state is 1 (on), change to binary.
        else {
			// Cast the long value into an int for easier manipulation.
			int num = (int) val;
			// Cast the int value into an iterable bitset array.
            std::bitset<std::numeric_limits<int>::digits> bits(num);
			int i = bits.size();
			// Print the bit in string format
            for (; 0 < i; --i, ++next) {
                *next = bits[i - 1]? '1': '0';
            }
            return next;
        }
    }
};

// Turn on the binary stream manipulator by setting the state in the iword
std::ostream& binary(std::ostream& stream) {
    stream.iword(index) = 1;
    return stream;
}

// Turn off the binary stream manipulator by desetting the state in the iword
std::ostream& nobinary(std::ostream& stream) {
    stream.iword(index) = 0;
    return stream;
}

int main()
{
	// Set up the locale and imbue the locale into the cout stream. 
    std::locale loc(std::locale(), new num_put);
    std::cout.imbue(loc);
	// Demonstrate the default print out.
	std::cout << "This is the standard bring out before turning on binary or nobinary. (10,16)" << std::endl;
	std::cout << 10 << std::endl;
	std::cout << 16 << std::endl;
	// Demonstrate the binary stream manipulator
	std::cout << "This is showing when binary is turned on. (10,16)" << std::endl;
    std::cout << binary << 10  << std::endl;
	std::cout << 16 << std::endl;
	// Demonstrate the nobinary stream manipulator
	std::cout << "This is showing when binary is turned off. (10,16)" << std::endl;
    std::cout << nobinary << 10 << std::endl;
	std::cout << 16 << std::endl;
	system("Pause");
}