/**
 * @file exer_11_7.cpp
 * @author KaKaRot
 * @brief  converts all characters to lower case 
 * @version 0.1
 * @date 2026-05-17
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#include <iostream>
#include <cctype>
#include <string>
#include <sstream>

void tolower_and_convert()
{
    std::cout << "Input a sentence...\n";
    std::string line;
    while(std::getline(std::cin, line)){
        std::istringstream is(line);
        std::string word;
        while (is >> word) {
            for (auto& ch : word) {
                ch = std::tolower(static_cast<unsigned char>(ch));
                if (std::ispunct(ch) && ch != '"' && ch != '-') {
                    ch = ' ';
                }
            }
        }

        if (word == std::string("don't")) {
            word = "do not";
        }else if(word == std::string("can't")){
            word = "cannot";
        }
        std::cout << word;
    }
}

int main()
{
    tolower_and_convert();
    return 0;
}