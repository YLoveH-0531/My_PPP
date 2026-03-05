/*
    @copyright 2026 KaKaRot
    exercises from chapter 3
    exercise 5 calculate the two intergers
*/
#include "std_lib_facilities.h"

int main()
{
    double val1, val2;
    std::cout << "Please input two float numbers: ";
    std::cout << endl;
    std::cin >> val1 >> val2;

    std::cout << "The max is: " << ((val1 >= val2) ? val1 : val2)
              << "\nThe min is: " << ((val1 < val2) ? val1 : val2)
              << "\nThe sum is: " << val1 + val2
              << "\nThe difference is: " << val1 - val2
              << "\nThe product is: " << val1 * val2
              << "\nThe ratio is: " << val1 / val2
              << std::endl;
    return 0;
}