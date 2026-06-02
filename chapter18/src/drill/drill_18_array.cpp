/**
 * @file drill_18_array.cpp
 * @author KaKaRot
 * @brief 
 * @version 0.1
 * @date 2026-06-02
 * 
 * @copyright Copyright (c) 2026
 */

#include <iostream>

int ga[10] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512};

void f(int* p, int count)
{
    int la[10];
    std::cout << "La = {";
    for (int i = 0; i < count; ++i){
        la[i] = p[i];
        std::cout << la[i];
        if (i != count -1)
        {
             std::cout << ",";
        }
    }
    std::cout << "}\n";

    int* lq = new int[10];
    std::cout << "lq = {";
    for (int i = 0; i < count; ++i){
        lq[i] = p[i];
        std::cout << la[i];
        if (i != count -1)
        {
             std::cout << ",";
        }
    }
    std::cout << "}\n";

    delete[] lq;
}

int main()
{
    f(ga, 10);
    int aa[10]{1, 2, 6, 24, 120, 720, 720*7, 720*7*8, 720*7*8*9, 720*7*8*9*10};
    f(aa, 10);
    return 0;
}