/**
 * @file drill_19_1.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2026-06-08
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "drill_19_1.h"
#include <iostream>
#include <vector>
#include <string>

int main()
{
    S<int> s_int(3);
    S<char> s_char('a');
    S<double> s_double(3.3);
    S<std::string> s_str("hello world!");
    std::vector<int> vec{5,6,7};
    S<std::vector<int>> s_vec_int(vec);

    std::cout << s_int.get() << '\n'; 
    std::cout << s_char.get() << '\n'; 
    std::cout << s_double.get() << '\n'; 
    std::cout << s_str.get() << '\n'; 
    for (const auto& i : s_vec_int.get()) {
        std::cout << i << " ";
    }
}