/**
 * @file exer_11_15.cpp
 * @author KaKaRot
 * @brief  Write a program that reads a series of floating-point numbers from a file.
 * @version 0.1
 * @date 2026-05-20
 * 
 * @copyright Copyright (c) 2026
 */

#include <iomanip>
#include <fstream>
#include <iostream>
#include <string>

#ifndef DATA_DIR
#error "DATA_DIR must be defined by CMake. Use cmake to build this project."
#endif

const std::string DATA = DATA_DIR;

void read_write_numbers(const std::string& in_file, const std::string& out_file)
{
    std::cout << "start read and write numbers...\n";
    std::ifstream ifs(in_file);
    if (!ifs) {
        std::cerr << "open " << in_file << " file failed!\n";
        return ;
    }

    std::ofstream ofs(out_file);
    if (!ofs) {
        std::cerr << "create " << out_file << " file failed!\n";
        return ;
    }

    double num;
    int count = 0;
    ofs << std::scientific << std::setprecision(8) << std::right;
    while (ifs >> num) {
        ofs << std::setw(20) << num;
        if (++count % 4 == 0) {
            ofs << '\n';
        }
    }

    if (count > 0 && count % 4 != 0) {
        ofs << '\n';
    }
    
    std::cout << "finish read and write numbers...\n";
}

int main()
{
    std::string in_file = DATA + "exer_11_15_input.txt";
    std::string out_file = DATA + "exer_11_15_output.txt";
    read_write_numbers(in_file, out_file);
    return 0;
}