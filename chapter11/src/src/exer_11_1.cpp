/**
 * @file     exer_11_1.cpp
 * @author   KaKaRot
 * @date     14/5/2026
 * @brief    reads a text file and converts its input to all lower case, producing a new file.
 * @details  reads a text file and converts its input to all lower case, producing a new file.
 */

#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cctype>

#ifndef DATA_DIR
#error "DATA_DIR must be defined by CMake. Use cmake to build this project."
#endif

const std::string DATA = DATA_DIR;

void convert(const std::string& in, const std::string& out)
{
    std::cout << "start converting..." << '\n';
    std::ifstream ifs(DATA + in);
    std::ofstream ofs(DATA + out);
    if(!ifs || !ofs) {
        throw std::runtime_error("open/create file failed!");
    }

    std::string line;
    while(std::getline(ifs, line)){
        for(auto& ch : line){
            ch = std::tolower(static_cast<unsigned char>(ch));
        }
        ofs << line << '\n';
    }

    std::cout << ofs.rdbuf();
    std::cout << "finish converting..." << '\n';
}

int main()
{
    std::string file1{"exer_11_1_input.txt"}, file2{"exer_11_1_output.txt"};
    convert(file1, file2);
    return 0;
}