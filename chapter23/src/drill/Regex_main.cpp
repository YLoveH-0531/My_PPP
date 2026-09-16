/**
 * @file Regex_main.cpp
 * @author KaKaRot
 * @brief 
 * @version 0.1
 * @date 2026-09-16
 * 
 * @copyright Copyright (c) 2026
 */

#include <regex> 
#include <fstream> 
#include <iostream>
#include <string>

#ifndef DATA_DIR
#error "DATA_DIR must be defined by CMake. Use cmake to build this project."
#endif

const std::string DATA = DATA_DIR;

int main()
{
    std::ifstream ifs(DATA + "Regex_data.txt");
    if(!ifs) {
        std::cerr << "Could not open the file - '"
                  << DATA + "Regex_data.txt" << "'" << std::endl;
        return EXIT_FAILURE;
    }

    std::regex r(R"(\w{2}\s*\d{5}(-\d{4})?)");
    int lineno = 0;
    for (std::string line; getline(ifs, line);) {
        ++lineno;
        std::smatch matches;
        if (std::regex_search(line, matches, r)) {
            std::cout << lineno << ": " << matches[0] << std::endl;
        }
    }
    
    return 0;
}