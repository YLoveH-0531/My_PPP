/**
 * @file Regex_error.cpp
 * @author KaKaRot
 * @brief 
 * @version 0.1
 * @date 2026-09-17
 * 
 * @copyright Copyright (c) 2026
 */

#include <cstddef>
#include <exception>
#include <regex>
#include <string>
#include <iostream>

void Regex_error()
{
    std::regex reg;
    std::string expre;

    std::cout << "Input regex expression..." << '\n';
    std::cin >> expre;

    try {
        reg = expre;
    } catch (const std::bad_exception&) {
        std::cerr << "The regex expression does not meet the requirement..." << std::endl;
        exit(1);
    }
    
    int lineno = 0;
    for (std::string line; std::getline(std::cin, line);) {
        std::smatch matches;
        ++lineno;

        if (std::regex_search(line, matches, reg )) {
            std::cout << "line: " << lineno << line << std::endl;
            for (std::size_t i = 0; i < matches.size(); ++i){
                std::cout << "\tmatches[ " << i << "]: " << matches[i] << std::endl;
            }
        }else {
            std::cout << "Does not match!" << std::endl;
        }
    
    }

}