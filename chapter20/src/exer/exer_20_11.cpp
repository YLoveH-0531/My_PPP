/**
 * @file exer_20_11.cpp
 * @author KaKaRot
 * @brief 
 * @version 0.1
 * @date 2026-07-07
 * 
 * @copyright Copyright (c) 2026
 */

#include <iostream>
#include <iterator>
#include <list>
#include <vector>
#include <algorithm>

void fun(const std::list<int>& l_int)
{
    std::vector<double> vec;
    vec.reserve(l_int.size());
    std::copy(l_int.cbegin(), l_int.cend(), std::back_inserter(vec));
    std::sort(vec.begin(), vec.end());
    for (auto i : vec) {
        std::cout << i << ' ';
    }
    std::cout << '\n';
}

int main()
{
    std::cout << "空列表: ";
    fun({});

    std::cout << "单元素 {42}: ";
    fun({42});

    std::cout << "已排序 {1,2,3,4,5}: ";
    fun({1, 2, 3, 4, 5});

    std::cout << "逆序   {5,4,3,2,1}: ";
    fun({5, 4, 3, 2, 1});

    std::cout << "乱序   {3,1,4,1,5,9,2,6}: ";
    fun({3, 1, 4, 1, 5, 9, 2, 6});

    std::cout << "含负数 {-3,0,5,-1,2}: ";
    fun({-3, 0, 5, -1, 2});

    return 0;
}