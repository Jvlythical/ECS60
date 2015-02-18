#include "weiss/StackLi.h"

class LongInt {
 public:
    LongInt();
    LongInt operator+(LongInt &n);
    StackLi<int>& getData();

    friend istream& operator>>(std::istream &strm, LongInt &num);
    friend ostream& operator<<(std::ostream &strm, LongInt &num);
 
 private:
    StackLi<int> d;
 
};