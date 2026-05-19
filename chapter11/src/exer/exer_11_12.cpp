/**
 * @file exer_11_12.cpp
 * @author KaKaRot
 * @brief 
 * @version 0.1
 * @date 2026-05-19
 * 
 * @copyright Copyright (c) 2026
 */

#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>

#ifndef DATA_DIR
#error "DATA_DIR must be defined by CMake. Use cmake to build this project."
#endif

const std::string DATA = DATA_DIR;

void reverse_file(const std::string& filename)
{
    std::cout << "Start Reversing file: "<< "\n";
    std::ifstream ifile(filename);
    if (!ifile) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::string temp_filename = DATA + "exer_11_12_output.txt";
    std::ofstream ofile(temp_filename);
    if (!ofile) {
        std::cerr << "Error opening output file." << std::endl;
        return;
    }

    std::ostringstream buffer;
    buffer << ifile.rdbuf();
    std::string content = buffer.str();
    std::reverse(content.begin(), content.end());
    ofile << content;
    std::cout << "File reversed and saved to: " << temp_filename << "\n";
}

int main()
{   
    std::string filename = DATA + "exer_11_12_input.txt";
    reverse_file(filename);
    return 0;
}