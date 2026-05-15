/**
 * @file exer_11_3.cpp
 * @author KaKaRot
 * @brief removes all vowels from a file
 * @version 0.1
 * @date 2026-05-15
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <cstdio>

#ifndef DATA_DIR
#error "DATA_DIR must be defined by CMake. Use cmake to build this project."
#endif

const std::string DATA = DATA_DIR;

/**
 * @brief Checks if a character is a vowel
 * 
 * @param ch The character to check
 * @return true if the character is a vowel, false otherwise
 */
bool is_vowel(char ch)
{
    switch(ch){
        case 'a': case 'e': case 'i': case 'o': case 'u':
        case 'A': case 'E': case 'I': case 'O': case 'U':
            return true;
        default:
            return false;
    }
}

/**
 * @brief Removes vowels from a text file
 * 
 * @param filename The name of the file to process
 */
void remove_vowels(const std::string& filename)
{
    std::cout << "start removing vowels..." << '\n';
    std::ifstream ifs(filename);
    std::string temp_filename = filename + ".tmp";
    std::ofstream ofs(temp_filename);

    if (!ifs) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    if (!ofs)
    {
        throw std::runtime_error("Could not create file: " + temp_filename);
    }

    char ch;
    while(ifs.get(ch)){
        if(!is_vowel(ch)){
            ofs << ch;
        }
    }

    ifs.close();
    ofs.close();

    if (std::remove(filename.c_str()) != 0)
    {
        throw std::runtime_error("Could not remove file: " + filename);
    }

    if (std::rename(temp_filename.c_str(), filename.c_str()) != 0)
    {
        throw std::runtime_error("Could not rename file: " + temp_filename + " to " + filename);
    }
    
    std::cout << "finished removing vowels..." << '\n';
}

int main()
{
    try {
        remove_vowels(DATA + "exer_11_3_input.txt");
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }  
}