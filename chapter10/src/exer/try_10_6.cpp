/**
 * @file     ch10_6.cpp
 * @author   KaKaRot
 * @date     11/5/2026
 * @brief    practice on contents.
 * @details  rewrite some examples from chapter.
 */

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <vector>


#ifndef DATA_DIR
#error "DATA_DIR must be defined by CMake. Use cmake to build this project."
#endif

const std::string DATA = DATA_DIR;


void fill_to_vector(std::vector<int>& v, std::ifstream& ifs, char teminator)
{
    for(int i; ifs >> i;){
        v.push_back(i);
    }

    if (ifs.eof()) { return; }

    ifs.clear();
    char c;
    ifs >> c;
    if (c != teminator){
        ifs.unget();
        ifs.setstate(std::ios::failbit);
    }
}

int main()
{
    const std::string& filename{"exer_10_1.txt"};
    std::ifstream ifs(DATA + filename);
    std::vector<int> vec;

    ifs.exceptions(ifs.exceptions() | std::ios::badbit);
    fill_to_vector(vec, ifs, '|');

    return 0;
}