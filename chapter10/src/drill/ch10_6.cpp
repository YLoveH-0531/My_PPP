/**
 * @file     ch10_6.cpp
 * @author   KaKaRot
 * @date     11/5/2026
 * @brief    practice on contents.
 * @details  rewrite some examples from chapter.
 */

#ifndef DATA_DIR
#error "DATA_DIR must be defined by CMake. Use cmake to build this project."
#endif

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <vector>

const std::string DATA = DATA_DIR;

void fill_from_vector(const std::vector<int>& v, const std::string& str, char teminator)
{
    for(const auto&)
}