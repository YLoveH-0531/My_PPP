/*
    @copyright 2026 KaKaRot
    exercises from chapter 3
    exercise 8 judge a int value is odd or even
*/

#include "std_lib_facilities.h"

int main()
{
    int val;
    std::cout << "Please input a number: " ;
    std::cin >> val;
    std::cout << std::endl;

    std::cout << "The value " << val << " is " << ((val%2 == 0) ? "even" : "odd.") << std::endl;

    return 0;
}