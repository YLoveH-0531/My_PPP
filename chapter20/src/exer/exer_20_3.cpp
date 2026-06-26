/**
 * @file exer_20_3.cpp
 * @author KaKaRot
 * @brief another version of exer_20_2.cpp, using size instead of last pointer
 * @version 0.1
 * @date 2026-06-26
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
#include <iterator>
#include <algorithm>

const std::string DATA = DATA_DIR;


void Jack_and_Jill()
{
    std::string filename1 = DATA + "exer_20_2.txt";
    std::string filename2 = DATA + "exer_20_2_array.txt";
    std::ifstream input1(filename1);
    std::ifstream input2(filename2);
    if (!input1 || !input2)
    {
        std::cerr << "Failed to open file: " << filename1 << " or " << filename2 << std::endl;
        return;
    }

    std::vector<double> words1(20);
    std::vector<double> words2;

    std::copy(std::istream_iterator<double>(input1), std::istream_iterator<double>(), std::back_inserter(words2));
    std::copy_n(std::istream_iterator<double>(input2), words1.size(), words1.begin());

    auto it1 = std::max_element(words1.begin(), words1.end());
    auto it2 = std::max_element(words2.begin(), words2.end());

    std::cout << "The maximum value in " << filename1 << " is: " << *it1 << std::endl;
    std::cout << "The maximum value in " << filename2 << " is: " << *it2 << std::endl;

}

int main()
{
    Jack_and_Jill();
    return 0;
}