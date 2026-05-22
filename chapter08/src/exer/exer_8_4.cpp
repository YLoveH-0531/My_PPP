/**
 * @file     exer_8_4.cpp
 * @author   KaKaRot
 * @date     20/4/2026
 * @brief    To find max value of int type using Fibonacci numbers
 * @details  Find an approximation of that maximum number by using fibonacci().
 */
#include "exer_8_2.h"
#include <limits>

void fibonacci(int x, int y, std::vector<int>& fac_vec, std::vector<int>::size_type size)
{
    long long max_int = std::numeric_limits<int>::max();
    long long lx = x;
    long long ly = y;
    for (std::vector<int>::size_type i = 0; i < size ; i++) {
        if (lx > max_int) {
            break;
        }
        fac_vec.push_back(static_cast<int>(lx));
        long long z = lx + ly;
        lx = ly;
        ly = z;
    }
    print("fac_vec", fac_vec, std::cout);
    std::cout << "Approximation of int maximum number is " << fac_vec[fac_vec.size()-1] << "\n";
}

int main()
{
    long long max_int = std::numeric_limits<int>::max();
    std::vector<int> vec;
    fibonacci(1, 2, vec, max_int);
    return 0;
}