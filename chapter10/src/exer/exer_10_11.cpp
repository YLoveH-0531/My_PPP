/**
 * @file     exer_10_11.cpp
 * @author   KaKaRot
 * @date     11/5/2026
 * @brief    sum of integers in files.
 * @details  Write a program that produces the sum of all 
             the whitespace-separated integers in a text file
 */

#ifndef DATA_DIR
#error "DATA_DIR must be defined by CMake. Use cmake to build this project."
#endif

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

const std::string DATA = DATA_DIR;

void sum(const std::string& file)
{
    std::cout << "sum start...\n";
    std::ifstream ifs(DATA + file);
    if (!ifs) { throw std::runtime_error("can not open file."); }

    std::string word;
    int sum = 0;
    while(ifs >> word){ 
        try{
            int n = std::stoi(word);
            sum += n;
        }catch (const std::invalid_argument&){
            continue;
        }catch (...){
            continue;
        }
    }

    std::cout << "sum is " << sum << '\n';
    std::cout << "sum finish...\n";
}

int main()
{
    std::string file{"exer_10_11_file.txt"};
    sum(file);
    return 0;
}