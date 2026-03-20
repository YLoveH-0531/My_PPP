/*
    @copyright 2026 KaKaRot
    drills from chapter 4
    drill 4 and 5 while lope, read two doubles and output the smaller and larger until '|' is read, 
    writing out The numbers are almost equal if the two numbers differ by less than 1.0/100.
*/

#include <iostream>

int main()
{
    double a, b;
    std::cout << "Please input two doubles(Press | to stop):";
    while(std::cin >> a >> b)
    {
        double smaller = (a < b) ? a : b;
        double larger = (a > b) ? a : b;
        if(smaller == larger)
        {
            std::cout << "The two doubles are equal: " << smaller << std::endl;
        }
        else {
            std::cout << "The smaller double is " << smaller << " and the larger double is " << larger << std::endl;
            if ((larger-smaller) < 1.0/100 )
            {   
                std::cout << "The numbers are almost equal" << std::endl;
            }
        }
        std::cout << "Please re-input two doubles(Press | to stop):";
    }
    return 0;
}