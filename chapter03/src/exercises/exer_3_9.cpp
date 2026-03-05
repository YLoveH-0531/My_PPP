/*
    @copyright 2026 KaKaRot
    exercises from chapter 3
    exercise 9 converts spelled-out numbers such into digits
*/

#include "std_lib_facilities.h"

int main()
{
    std::string num;
    std::cout << "Please input the number: ";
    std::cin >> num;
    std::cout << std::endl;

    if (num == "zero") {
        std::cout << "0" << std::endl;
    }else if (num == "one") {
        std::cout << "1" << std::endl;
    }else if (num == "two") {
        std::cout << "2" << std::endl;
    }else if (num == "three") {
        std::cout << "3" << std::endl;
    }else if (num == "four") {
        std::cout << "4" << std::endl;
    }else if (num == "five") {
        std::cout << "5" << std::endl;
    }else {
        std::cout << "Not a number I know" << std::endl;
    }

    return 0;
}