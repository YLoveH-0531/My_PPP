/*
    @copyright 2026 KaKaRot
    exercises from chapter 3
    exercise 7 output three user's string input in ascending order
*/
#include "std_lib_facilities.h"

int main()
{
    std::string a, b, c;
    std::string temp;

    std::cout << "Please input three string values:";
    std::cin >> a >> b >> c;
    std::cout << std::endl;

    // output the value in order a,b,c, a is always min, and c is always max
    if (a > b) {
        temp = b;
        b = a;
        a = temp;
    }else if(a > c){
        temp = c;
        c = a;
        a = temp;    
    }

    if (b > c) {
        temp = c;
        c= b;
        b = temp;
    }

    std::cout << "The order is " << a << ", " << b << ", " << c << std::endl;
    return 0;
}