/*
    @copyright 2026 KaKaRot
    drills from chapter 4
    drill 2 and 3 while lope, read two ints and output the smaller and larger until '|' is read
*/

#include <iostream>

int main()
{
    int a, b;
    std::cout << "Please input two integers(Press | to stop):";
    while(std::cin >> a >> b)
    {
        int smaller = (a < b) ? a : b;
        int larger = (a > b) ? a : b;
        if(smaller == larger)
        {
            std::cout << "The two integers are equal: " << smaller << std::endl;
        }
        else {
            std::cout << "The smaller integer is " << smaller << " and the larger integer is " << larger << std::endl;
        }
        std::cout << "Please re-input two integers(Press | to stop):";
    }
    return 0;
}