/**
 * @file Regex_stud.cpp
 * @author KaKaRot
 * @brief 
 * @version 0.1
 * @date 2026-09-17
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
    std::ifstream in(DATA + "Regex_stud.txt");
    if (!in) std::cerr << "Can not open file" << std::endl;

    std::regex header();
    std::regex header();

    return 0;
}
