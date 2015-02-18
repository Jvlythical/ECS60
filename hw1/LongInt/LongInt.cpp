#include <iostream>
#include <cstdio>

#include "weiss/StackLi.h"
#include "LongInt.h"

using namespace std;

LongInt::LongInt() {}

istream& operator>>(istream &s, LongInt &n) {
	char c;

	while(s.get(c)) {
		if(isdigit(c)) n.getData().push(c - '0');
		if(!isdigit(s.peek())) break;
	}

	return s;
}

ostream& operator<<(std::ostream &os, LongInt &n) {
	StackLi<int> d = n.getData();
	int c;

	while(!d.isEmpty()) {
		c = d.topAndPop();
		cout << c;
	}

	return os;
}

LongInt LongInt::operator+(LongInt &n) {
	bool carry = 0; 
	int tmp_sum = 0;
	LongInt s;
	StackLi<int> n1 = this->getData(), n2 = n.getData(), & n3 = s.getData();

	while(!n1.isEmpty() && !n2.isEmpty()) {
		tmp_sum = n1.topAndPop() + n2.topAndPop();
			
		if(carry) {
		  ++tmp_sum;
		  carry = 0;
		}
		
		if(tmp_sum > 9) {
			tmp_sum = tmp_sum - 10;
			carry = 1;
		}
		
		n3.push(tmp_sum);
	}

// Changed start
	if(n1.isEmpty()) n1 = n2;
	
	while(!n1.isEmpty()) {
        tmp_sum = n1.topAndPop();
        
        if(carry) {
            ++tmp_sum;
            carry = 0;
            
            if(tmp_sum > 9) {
                tmp_sum = tmp_sum - 10;
                carry = 1;
            }
        }
        
        n3.push(tmp_sum);
    }

    if(carry) n3.push(1);
// Changed end	    
	    
	return s;
}

StackLi<int>& LongInt::getData() {
	return d;
}
