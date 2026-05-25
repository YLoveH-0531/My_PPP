/**
 * @file exer_17_1_2.cpp
 * @author KaKaRot
 * @brief  Chapter 17 Exercise 2: sizeof about pointers and built-in types
 * @version 0.1
 * @date 2026-05-25
 * 
 * @copyright Copyright (c) 2026
 */

#include <iostream>
#include <cassert>

int main()
{
    int*p = new int[10];
    std::cout << p << std::endl;
    delete[] p;

    assert(sizeof(p) == 8); 
    assert(sizeof(bool) == 1); 
    assert(sizeof(int) == 4); 
    assert(sizeof(double) == 8); 

    return 0;
}