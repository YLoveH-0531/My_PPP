/**
 * @file     Test_output.cpp
 * @author   KaKaRot
 * @date     14/5/2026
 * @brief    practice output format 
 * @details  Practice formatting output in C++
 */

#include <iostream>
#include <iomanip>

int main()
{
    int birth_year = 1994;
    int age = 2026 - birth_year;
    std::cout << "Birth year: " << '\n';
    std::cout << std::showbase;
    std::cout << birth_year << '\t';
    std::cout << std::hex << birth_year << '\t';
    std::cout << std::oct << birth_year << '\n';

    std::cout << "Age: " << '\n';
    std::cout << std::dec << age << '\t';
    std::cout << std::hex << age << '\t';
    std::cout << std::oct << age << '\n';
    return 0;
}