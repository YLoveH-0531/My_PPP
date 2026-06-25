/**
 * @file exer_20_2.cpp
 * @author KaKaRot
 * @brief 
 * @version 0.1
 * @date 2026-06-25
 * 
 * @copyright Copyright (c) 2026
 */

#ifndef DATA_DIR
#error "DATA_DIR must be defined by CMake. Use cmake to build this project."
#endif

#include <iostream>
#include <vector>
#include <fstream>
#include <string>

const std::string DATA = DATA_DIR;

double* high(const double* first, const double* last)
{
    double* result = nullptr;
    
    for (int i = 0; i < last - first; i++)
    {
        if (result == nullptr || first[i] > *result)
        {
            result = const_cast<double*>(&first[i]);
        }
    }
    return result;
}

void Jack_and_Jill()
{
    std::string filename1 = DATA + "exer_20_2.txt";
    std::ifstream input1(filename1);
    if (!input1)
    {
        std::cerr << "Failed to open file: " << filename1 << std::endl;
        return;
    }

    std::vector<double> numbers;
    double num;
    while (input1 >> num)
    {
        numbers.push_back(num);
    }

    if (!numbers.empty())
    {
        double* max_ptr = high(&numbers[0], &numbers[0] + numbers.size());
        std::cout << "The highest value is: " << *max_ptr << std::endl;
    }

    double array[10];
    std::string filename2 = DATA + "exer_20_2_array.txt";
    std::ifstream input2(filename2);
    if (!input2)
    {
        std::cerr << "Failed to open file: " << filename2 << std::endl;
        return;
    }

    for (int i = 0; i < 10 && input2 >> array[i]; i++){ }

    double* max_ptr = high(array, array + 10);
    std::cout << "The highest value in the array is: " << *max_ptr << std::endl;
}

int main()
{
    Jack_and_Jill();
    return 0;
}