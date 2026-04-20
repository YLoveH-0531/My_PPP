/**
 * @file     exer_8_4.cpp
 * @author   KaKaRot
 * @date     20/4/2026
 * @brief    To find max value of int type using Fibonacci numbers
 * @details  Fibonacci numbers
 */
#include "exer_8_2.h"

void fibonacci(int x, int y, std::vector<int>& fac_vec, std::vector<int>::size_type size)
{
    fac_vec.push_back(x);
    fac_vec.push_back(y);

    for (std::vector<int>::size_type i = 0; i < size - 2; i++) {
        fac_vec.push_back(fac_vec[i+1] + fac_vec[i]);
    }

    print("fac_vec", fac_vec, std::cout);
}

int main()
{
    std::vector<int> vec;
    fibonacci(1, 2, vec, 8);
    return 0;
}