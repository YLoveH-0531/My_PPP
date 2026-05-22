/**
 * @file exer_11_8.cpp
 * @author KaKaRot
 * @brief  Modify the program from the previous exercise to read a file and build the set of words from that file.
 * @version 0.1
 * @date 2026-05-19
 * 
 * @copyright Copyright (c) 2026
 */

#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
#include <sstream>
#include <vector>
#include <set>

#ifndef DATA_DIR
#error "DATA_DIR must be defined by CMake. Use cmake to build this project."
#endif

const std::string DATA = DATA_DIR;

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

/**
 * @brief  Convert the input sentence to lowercase and replace punctuation with spaces, 
 *         then insert the processed words into the provided set.
 * @param dic 
 * @param filename 
 */
void tolower_and_convert(std::set<std::string>& dic, const std::string& filename)
{
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }
    std::string line;
    while(std::getline(file, line)){
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
            dic.insert(word);
        }
    }
}

int main()
{
    std::set<std::string> dic;
    std::string filename{DATA + "exer_11_8_input.txt"};
    tolower_and_convert(dic, filename);
    for (const auto& word : dic){
        std::cout << word << "\n";
    }
    return 0;
}