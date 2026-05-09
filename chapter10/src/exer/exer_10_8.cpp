/**
 * @file     exer_10_8.cpp
 * @author   KaKaRot
 * @date     9/5/2026
 * @brief    concatenates the two files.
 * @details   accepts two file names and produces a new file 
              that is the contents of the first file followed 
              by the contents of the second
 */

#ifndef DATA_DIR
#error "DATA_DIR must be defined by CMake. Use cmake to build this project."
#endif

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

void concate(const std::string& file1, const std::string& file2)
{
    std::ifstream ifs1(file1);
    std::ifstream ifs2(file2);

    if (!ifs1 || !ifs2) { throw std::runtime_error("can not open file"); }

}


int main()
{   
    std::string file1, file2;
    concate(file1, file2);
    return 0;
}