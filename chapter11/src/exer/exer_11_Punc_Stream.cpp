/**
 * @file exer_11_Punc_Stream.cpp
 * @author KaKaRot
 * @brief 
 * @version 0.1
 * @date 2026-05-20
 * 
 * @copyright Copyright (c) 2026
 */

#include <fstream>
#include <string>
#include <set>
#include "exer_11_Punc_Stream.h"

#ifndef DATA_DIR
#error "DATA_DIR must be defined by CMake. Use cmake to build this project."
#endif 

const std::string DATA = DATA_DIR;

void read_use_Punc_Stream()
{
    std::ifstream ifs(DATA + "exer_11_Punc_Stream.txt");
    if(!ifs){
        std::cerr << "Could not open file: " << DATA + "exer_11_Punc_Stream.txt" << std::endl;
        return;
    }

    Punc_Stream ps(ifs);
    ps.add_punct(",.!?;:\"()[]{}<>");
    ps.setSensitive(false);

    std::set<std::string> unique_words;
    std::string word;
    while(ps >> word){
        unique_words.insert(word);
    }

    std::cout << "Unique words in the file:" << std::endl;
    for(const auto& w : unique_words){
        std::cout << w << std::endl;
    }
}

int main()
{
    read_use_Punc_Stream();
    return 0;
}
