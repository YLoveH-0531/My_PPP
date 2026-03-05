/*
    @copyright 2026 KaKaRot
    drills from chapter 4
    drill 1 while lope, read two ints and output them until '|' is read
*/

#include "std_lib_facilities.h"

int main()
{
    int a, b;
    std::cout << "Please input two integers(Press | to stop):";
    while(std::cin >> a >> b)
    {
        std::cout << "The two integers are " << a << " and " << b << std::endl;
        std::cout << "Please re-input two integers(Press | to stop):";
    }
    return 0;
}