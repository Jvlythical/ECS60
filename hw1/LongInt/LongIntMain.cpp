#include <iostream>

#include "LongInt.h"

using namespace std;

int main() {
	LongInt int1, int2, int3;

	cout << "Please enter first long integer >> ";
	cin >> int1;
	cout << "Please enter second long integer >> ";
	cin >> int2;

	int3 = int1 + int2;

	cout << int3 << endl;
        
    int test = "432598741390874309587243098744681944435448443143663211" == "432598741390874309587243098744681944435448443143663211";    
        
        
	return 0;
}
