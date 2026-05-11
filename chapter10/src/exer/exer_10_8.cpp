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

const std::string DATA = DATA_DIR;

void concate(const std::string& file1, const std::string& file2, const std::string& out)
{
    std::cout << "Concate  start...\n";

    std::ifstream ifs1(DATA + file1);
    std::ifstream ifs2(DATA + file2);
    std::ofstream ofs(DATA + out);

    if (!ifs1 || !ifs2) { throw std::runtime_error("can not open file"); }
    if (!ofs) { throw std::runtime_error("can not create file"); }

    ofs << ifs1.rdbuf();
    ofs << ifs2.rdbuf();
    if (!ofs) throw std::runtime_error("Write failed: " + out);
    ofs.close();

    std::ifstream ifs3(DATA + out);
    std::cout << ifs3.rdbuf();
    std::cout << "Concate  finish...\n";
}


int main()
{   
    std::string file1{"exer_10_1.txt"}, file2{"MyData.txt"}, file3{"exer_10_8.txt"};
    concate(file1, file2, file3);
    return 0;
}