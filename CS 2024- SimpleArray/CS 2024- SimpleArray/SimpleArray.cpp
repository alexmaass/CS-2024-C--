#include <exception>
#include <iostream>
#define ArrayOutofBoundsError 1

/**
 *	The definition and declaration of the SimpleArray class. The SimpleArray class
 *	is an array class that can take be used for any type.
 */
template<typename StorageType, int size>
class SimpleArray{
// Private variables
private:
	// The pointer that will be used to point to the actual array of values. 
	StorageType *store;
// Public variables
public:
	/**
	 *	The constructor for the SimpleArray class. The size of the array is predetermined by the template. 
	 */
	SimpleArray(){
		store = new StorageType[size];
	}
	/**
	 *	The [] operator for the SimpleArray class. It functions exactly the same as the normal operator,
	 *	with the exception that is checks to see if the requested index is valid and throws an exception if
	 *	it is not. 
	 */
	StorageType& operator[](int i){
		if ( i > size-1 || i < 0){
			throw ArrayOutofBoundsError;
		}
		else{
			return store[i];
		}
	}
};


