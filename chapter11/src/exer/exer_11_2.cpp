/**
 * @file exer_11_2.cpp
 * @author KaKaRot
 * @brief given a file name and a word outputs each line that contains 
 *        that word together with the line number.
 * @version 0.1
 * @date 2026-05-15
 * @copyright Copyright (c) 2026
 * 
 */

#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include <stdexcept>

#ifndef DATA_DIR
#error "DATA_DIR must be defined by CMake. Use cmake to build this project."
#endif

const std::string DATA = DATA_DIR;

/**
 * @brief Checks a file for lines containing a specific word and prints them with line numbers.
 * 
 * @param filename 
 */
void check_file(const std::string& filename)
{
    std::cout << "start checking..." << '\n';

    std::string word = "in";
    std::ifstream ifs(filename);
    if (!ifs) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    std::string line;
    int line_number = 0;
    while (std::getline(ifs, line)) {
        ++line_number;
        auto pos = line.find(word);
        while (pos != std::string::npos) {
            // Check if the found word is a standalone word
            bool is_standalone = (pos == 0 || !std::isalnum(static_cast<unsigned char>(line[pos - 1]))) &&
                                 (pos + word.size() >= line.size() || !std::isalnum(static_cast<unsigned char>(line[pos + word.size()])));
            if (is_standalone) {
                std::cout << "Line " << line_number << ": " << line << '\n';
                break; // Move to the next line after finding the first occurrence
            }
            pos = line.find(word, pos + word.size());
        }
    }
    std::cout << "checking finished..." << '\n';
}

int main()
{
    try {
        check_file(DATA + "exer_11_2_input.txt");
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
    return 0;
}