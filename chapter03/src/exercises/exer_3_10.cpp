/*
    @copyright 2026 KaKaRot
    exercises from chapter 3
    exercise 10 calculate the values
*/

#include "std_lib_facilities.h"

int main()
{
    std::string operator_;
    double opt1, opt2;
    
    std::cout << "Please input the operation and the numbers: ";
    std::cin >> operator_ >> opt1 >> opt2;
    std::cout << std::endl;

    if (operator_ == "+") {
    std::cout << opt1 << operator_ << opt2 << "=" << opt1 + opt2 << std::endl;
    }else if (operator_ == "-") {
    std::cout << opt1 << operator_ << opt2 << "=" << opt1 - opt2 << std::endl;
    }else if (operator_ == "*") {
    std::cout << opt1 << operator_ << opt2 << "=" << opt1 * opt2 << std::endl;
    }else if (operator_ == "/") {
    std::cout << opt1 << operator_ << opt2 << "=" << opt1 / opt2 << std::endl;
    }else {
    std::cout << "Not a operation I know" << std::endl;
    }

    return 0;    
}