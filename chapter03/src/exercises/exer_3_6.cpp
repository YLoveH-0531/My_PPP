/*
    @copyright 2026 KaKaRot
    exercises from chapter 3
    exercise 6 output three user's integer numbers in numerical
*/
#include "std_lib_facilities.h"

int main()
{
    int a, b, c;
    int temp = 0;

    std::cout << "Please input three integer numbers:";
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