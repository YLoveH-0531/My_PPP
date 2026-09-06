/**
 * @file vector_vd.cpp
 * @author KaKaRot
 * @brief 
 * @version 0.1
 * @date 2026-09-04
 * 
 * @copyright Copyright (c) 2026
 */

#include <numeric>
#include <sstream>
#ifndef DATA_DIR
#error "DATA_DIR must be defined by CMake. Use cmake to build this project."
#endif

#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <iterator>
#include <string>

const std::string DATA = DATA_DIR;

int main()
{
    std::vector<double> vd;
    std::vector<int> vi;

    std::ifstream ifs1(DATA + "exer_21_drill_vd.txt");
    std::istream_iterator<double> ind(ifs1), endd;
    std::ostream_iterator<double> os_it(std::cout, "\n");
    
    std::copy(ind, endd, std::back_inserter(vd));
    std::copy(vd.begin(), vd.end(), std::back_inserter(vi));

    for (auto& item : vi) {
        std::cout << item << std::endl;
    }

    std::transform(vd.begin(), vd.end(), vi.begin(),
                   std::ostream_iterator<std::string>(std::cout, "\n"),
                   [](double d, int i) {
                     std::ostringstream oss;
                     oss << "(" << d << "," << i << ")";
                     return oss.str();
                   });

    std::cout << "The sum is: " << std::accumulate(vd.cbegin(), vd.cend(), 0.0) << std::endl;

    std::cout << "The diffrence is: "
              << std::accumulate(vd.cbegin(), vd.cend(), 0.0) -
                     std::accumulate(vi.cbegin(), vi.cend(), 0.0)
              << std::endl;

    std::cout << "Reverse algorithm..." << "\n";
    std::reverse(vd.begin(), vd.end());
    std::copy(vd.cbegin(), vd.cend(), os_it);
    
    std::cout << "Mean value..." << "\n";
    double mean = std::accumulate(vd.cbegin(), vd.cend(), 0.0) / vd.size(); 
    std::cout << mean << std::endl;

    std::cout << "vd2..." << "\n";
    std::vector<double> vd2;
    std::copy_if(vd.cbegin(), vd.cend(), std::back_inserter(vd2), [&mean](double d) { return d < mean; });
    std::copy(vd2.cbegin(), vd2.cend(), os_it);
    
    std::cout << "sorted..." << "\n";
    std::sort(vd.begin(), vd.end());
    std::copy(vd.cbegin(), vd.cend(), os_it);
    return 0;
}