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
#include <vector>

std::string Str_map(std::string word)
{
    static const std::vector<std::pair<std::string, std::string>> word_map = {
        {"don't", "do not"},
        {"can't", "cannot"},
        {"won't", "will not"}
    };

    for (const auto& pair : word_map)
    {
        std::string::size_type pos = 0;
        while ((pos = word.find(pair.first, pos)) != std::string::npos)
        {
            word.replace(pos, pair.first.length(), pair.second);
            pos += pair.second.length();
        }
    }

    return word;
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
                if (std::ispunct(static_cast<unsigned char>(word[i]))) {
                    if (word[i] == '-' && i > 0 && i < word.size() - 1) {
                        if (!std::isalpha(word[i-1]) || !std::isalpha(word[i+1])) {
                            word[i] = ' ';
                        }
                    }else if (word[i] != '"' && word[i] != '\'') {
                        word[i] = ' ';
                    }
                }
            }
            std::string mapped_word = Str_map(word);
            word = (mapped_word == word) ? word : mapped_word;
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