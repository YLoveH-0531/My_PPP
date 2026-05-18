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
#include <map>

std::string Str_map(const std::string& word)
{
    std::map<std::string, std::string> word_map = {
        {"don't", "do not"},
        {"can't", "cannot"},
        {"won't", "will not"}
    };

    return word_map.count(word) ? word_map[word] : word;
}

void tolower_and_convert()
{
    std::cout << "Input a sentence...\n";
    std::string line;
    while(std::getline(std::cin, line)){
        std::istringstream is(line);
        std::string word;
        while (is >> word) {
            for (std::string::size_type i = 0; i < word.size(); ++i) {
                word[i] = std::tolower(static_cast<unsigned char>(word[i]));
                if (std::ispunct(word[i])) {
                    if (word[i] != '-' && (i > 0 || i < word.size() - 1)) {
                        if (!std::isalpha(word[i-1]) || !std::isalpha(word[i+1])) {
                            word[i] = ' ';
                        }
                    }
                    if (word[i] != '"' && word[i] != '\'') {
                        word[i] = ' ';
                    }
                }
            }
            word = (Str_map(word) == word) ? word : Str_map(word);
            std::cout << word << " ";
        }
        std::cout << '\n';
    }
}

int main()
{
    tolower_and_convert();
    return 0;
}