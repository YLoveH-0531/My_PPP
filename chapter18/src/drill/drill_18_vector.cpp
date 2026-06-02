/**
 * @file drill_18_vector.cpp
 * @author KaKaRot
 * @brief 
 * @version 0.1
 * @date 2026-06-02
 * 
 * @copyright Copyright (c) 2026
 */

#include <iostream>
#include <vector>

std::vector<int> gv{1, 2, 4, 8, 16, 32, 64, 128, 256, 512};

void f(const std::vector<int>& v)
{
    std::vector<int> lv;
    lv = v;
    std::cout << "lv = {";
    std::size_t count = 0;
    for (const auto& i : lv){
        std::cout << i;
        if (++count < lv.size()){ std::cout << ", "; }
        
    }
    std::cout << "}\n";

    std::vector<int> lv2;
    lv2 = v;
    std::cout << "lv2 = {";
    count = 0;
    for (const auto& i : lv2){
        std::cout << i;
        if (++count < lv2.size()){ std::cout << ", "; }
        
    }
    std::cout << "}\n";
}

int main()
{
    f(gv);
    std::vector<int> vv{1, 2, 6, 24, 120, 720, 720*7, 720*7*8, 720*7*8*9, 720*7*8*9*10};
    f(vv);
    return 0;
}