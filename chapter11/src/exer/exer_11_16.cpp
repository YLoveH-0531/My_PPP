/**
 * @file exer_11_16.cpp
 * @author KaKaRot
 * @brief 
 * @version 0.1
 * @date 2026-05-20
 * 
 * @copyright Copyright (c) 2026
 */

#include <fstream>
#include <iostream>
#include <string>
#include <map>

#ifndef DATA_DIR
#error "DATA_DIR must be defined by CMake. Use cmake to build this project."
#endif

const std::string DATA = DATA_DIR;

void read_int_to_map(const std::string& in_file)
{
    std::map<int, int> int_map;
    std::cout << "start read int to map...\n";
    std::ifstream ifs(in_file);
    if (!ifs) {
        std::cerr << "open " << in_file << " file failed!\n";
        return ;
    }

    int num;
    while (ifs >> num) {
        ++int_map[num];
    }

    for(const auto& pair : int_map) {
        std::cout << pair.first;
        if (pair.second > 1)
        {
            std::cout << " " << pair.second << " times)";
        }
        std::cout << '\n';
    }
    
    std::cout << "finish read int to map...\n";
}

int main()
{
    std::string in_file = DATA + "exer_11_16_input.txt";
    read_int_to_map(in_file);
    return 0;
}