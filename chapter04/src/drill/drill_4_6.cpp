/*
    @copyright 2026 KaKaRot
    drills from chapter 4, drill 6 while lope, 
    read one double data once loop.
    If it’s the smallest so far, write thesmallest so far after the number. 
    If it is the largest so far, write the largest so far after the number
*/

#include <iostream>

int main()
{
    double input;
    double smaller = 0.0, larger = 0.0;

    std::cout << "Please input one double(Press | to stop):";
    while(std::cin >> input)
    {
        if (smaller >= input) {
            smaller = input;
            std::cout << input <<" Smallest so far: " << smaller << std::endl;

        }
        else if (larger <= input) {
            larger = input;
            std::cout << input <<" Largest so far: " << larger << std::endl;
        }
        std::cout << "Please re-input one double(Press | to stop):";
    }
    return 0;
}