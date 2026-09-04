/**
 * @file vector_vd.cpp
 * @author KaKaRot
 * @brief 
 * @version 0.1
 * @date 2026-09-04
 * 
 * @copyright Copyright (c) 2026
 */

#ifndef DATA_DIR
#error "DATA_DIR must be defined by CMake. Use cmake to build this project."
#endif

#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <iterator>

const std::string DATA = DATA_DIR;

int main()
{
    std::vector<double> vd;
    std::vector<int> vi;

    std::ifstream ifs1(DATA + "exer_21_drill_vd.txt");
//    std::ofstream ofs(DATA + out);

    std::istream_iterator<double> ind(ifs1), endd;
    
    std::copy(ind, endd, std::back_inserter(vd));
    std::copy(vd.begin(), vd.end(), std::back_inserter(vi));

    for (auto& item : vi) {
        std::cout << item << std::endl;
    }
    return 0;
}
