/*
    @copyright 2026 KaKaRot
    exercises from chapter 3
    exercise 2 converts from miles to kilometers
*/
#include "std_lib_facilities.h"

int main()
{
    constexpr float m_to_km = 1.609;
    int miles = 0;

    std::cout << "Please enter a number to convert: " << std::endl;

    while (std::cin >> miles) {
        std::cout << miles << " miles equal to " << miles * m_to_km << " kilometers." << std::endl;
        std::cout << "Again! \n";
    }
}