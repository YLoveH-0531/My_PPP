/**
 * @file exer_11_10_11.cpp
 * @author KaKaRot
 * @brief  Write a program that reads a file and splits each line into words, 
 *         using the space character as a delimiter.
 * @version 0.1
 * @date 2026-05-19
 * 
 * @copyright Copyright (c) 2026
 */

#include <fstream>
#include <iostream>
#include <vector>
#include <string>

#ifndef DATA_DIR
#error "DATA_DIR must be defined by CMake. Use cmake to build this project."
#endif

const std::string DATA = DATA_DIR;

/**
 * @brief   Write a program that reads a file and splits each line into words, 
 *          using the space character as a delimiter.    
 * @param s 
 * @param w 
 * @return std::vector<std::string> 
 */
std::vector<std::string> split(const std::string& s, const std::string& w)
{
    std::cout << "Splitting string by delimiter: " << w << "\n";
    std::vector<std::string> result;
    std::string delimiter = " " + w;
    size_t pos = 0;
    size_t found = s.find(delimiter);
    while (found != std::string::npos) {
        result.push_back(s.substr(pos, found - pos));
        pos = found + delimiter.length();
        found = s.find(delimiter, pos);
    }
    result.push_back(s.substr(pos));
    std::cout << "Splitting down\n";
    return result;
}

int main()
{
    std::ifstream file(DATA + "exer_11_10_11_input.txt");
    if (!file) {
        std::cerr << "Error opening file: " << DATA + "exer_11_10_11_input.txt" << std::endl;
        return 1;
    }
    std::string line;
    while(std::getline(file, line)){
        const std::vector<std::string> words = split(line, "the");
        for (const auto& word : words)
        {
            std::cout << word << " ";
        }
        std::cout << '\n';
    }
    return 0;
}