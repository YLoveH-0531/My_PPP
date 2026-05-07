/**
 * @file     exer_10_1.cpp
 * @author   KaKaRot
 * @date     7/5/2026
 * @brief    read from file and sum the numbers in it
 * @details  produces the sum of all the numbers in a file of whitespace-separated integers
 */

#include <fstream>
#include <iostream>
#include <stdexcept>

#ifndef DATA_DIR
#error "DATA_DIR must be defined by CMake. Use cmake to build this project."
#endif

void function()
{
    std::ifstream infile(DATA_DIR"exer_10_1.txt");
    if (!infile) {
        throw std::runtime_error("No data.txt file exist!");
    }

    int number, sum = 0;
    while (infile >> number) {
        sum += number;
    }

    if (infile.fail() && !infile.eof()) {
        throw std::runtime_error("data format error");
    }

    if (infile.eof()) {
        std::cout << "Sum is " << sum << std::endl;
    }

}

int main()
{
    function();
    return 0;
}