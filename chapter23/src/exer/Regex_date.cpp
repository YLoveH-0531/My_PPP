/**
 * @file Regex_date.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2026-09-22
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include <string>
#include <iostream>
#include <fstream>
#include <regex>

#ifndef DATA_DIR
#error "DATA_DIR must be defined by CMake. Use cmake to build this project."
#endif

const std::string DATA = DATA_DIR;

int main()
{
    std::ifstream io(DATA + "Regex_date.txt");
    if (!io) {
        std::cerr << "can not open file!" << std::endl;
        return 1;
    }
    
    std::string line;
    int linenum = 0;
    std::regex re_date(
        R"((\d{1,2}[./-]\d{1,2}[./-]\d{2,4})"
        R"(|(\d{4}[./-]\d{1,2}[./-]\d{1,2})"
        R"(|((Jan|Feb|Mar|Apr|May|Jun|Jul|Aug|Sep|Oct|Nov|Dec)[a-z]*\.?\s+\d{1,2},?\s+\d{2,4})"
        R"(|(\d{1,2}\s+(Jan|Feb|Mar|Apr|May|Jun|Jul|Aug|Sep|Oct|Nov|Dec)[a-z]*\.?\s+\d{2,4}))",
        std::regex::icase);
    std::smatch result;

    while (std::getline(io, line)) {
        ++linenum;
        if (std::regex_search(line, result, re_date)) {
            std::cout << linenum << ": " << line << " -- " << result[0].str();
        }
    }
    return 0;
}