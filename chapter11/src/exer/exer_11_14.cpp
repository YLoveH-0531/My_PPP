/**
 * @file exer_11_14.cpp
 * @author KaKaRot
 * @brief 
 * @version 0.1
 * @date 2026-05-19
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <cctype>

#ifndef DATA_DIR
#error "DATA_DIR must be defined by CMake. Use cmake to build this project."
#endif

const std::string DATA = DATA_DIR;

void count_character(const std::string& file)
{
    std::ifstream ifs(file);
    if (!ifs) {
        std::cerr << "open " << file << " file failed!\n";
        return ;
    }

    char ch;
    std::map<std::string, unsigned int> ch_map{{"isalpha", 0}, {"isdigit",0}, {"isspace", 0}, {"ispunct",0}, {"isupper",0}, {"islower", 0}, {"isprint", 0}, {"iscntrl", 0}, {"isgraph",0 }};
    while (ifs.get(ch)) {
        unsigned char uch= static_cast<unsigned char>(ch);
        if(std::isalpha(uch)) { ch_map["isalpha"]++; }
        if(std::isdigit(uch)) { ch_map["isdigit"]++; }
        if(std::isspace(uch)) { ch_map["isspace"]++; }
        if(std::ispunct(uch)) { ch_map["ispunct"]++; }
        if(std::isupper(uch)) { ch_map["isupper"]++; }
        if(std::islower(uch)) { ch_map["islower"]++; }
        if(std::isprint(uch)) { ch_map["isprint"]++; }
        if(std::iscntrl(uch)) { ch_map["iscntrl"]++; }
        if(std::isgraph(uch)) { ch_map["isgraph"]++; }
    }

    for (const auto& ele : ch_map) {
        std::cout << ele.first << " occurs " << ele.second << (ele.second > 1 ? " counts" : " count") << '\n';
    }
}

int main()
{
    std::string filename = DATA + "exer_11_14_input.txt";
    count_character(filename);
    return 0;
}