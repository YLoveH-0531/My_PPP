/**
 * @file Implemention.cpp
 * @author KaKaRot
 * @brief 
 * @version 0.1
 * @date 2026-09-07
 * 
 * @copyright Copyright (c) 2026
 */

#include "Implemention.h"

#include <cassert>
#include <iostream>
#include <list>
#include <string>
#include <vector>

int main()
{
    // 1. vector<int>, count 基本用例
    std::vector<int> v{1, 2, 3, 2, 2, 5};
    auto r1 = count(v.begin(), v.end(), 2);
    std::cout << "count(v,2) = " << r1 << " (expect 3)\n";
    assert(r1 == 3);

    // 2. 没有匹配项
    auto r2 = count(v.begin(), v.end(), 100);
    std::cout << "count(v,100) = " << r2 << " (expect 0)\n";
    assert(r2 == 0);

    // 3. 空区间
    auto r3 = count(v.begin(), v.begin(), 2);
    std::cout << "count(empty) = " << r3 << " (expect 0)\n";
    assert(r3 == 0);

    // 4. 裸指针（验证 iterator_traits 对指针的偏特化）
    int arr[] = {4, 4, 4, 1, 4};
    auto r4 = count(arr, arr + 5, 4);
    std::cout << "count(raw ptr) = " << r4 << " (expect 4)\n";
    assert(r4 == 4);

    // 5. list<int>（双向迭代器，验证不依赖 operator+）
    std::list<int> lst{7, 8, 7, 7};
    auto r5 = count(lst.begin(), lst.end(), 7);
    std::cout << "count(list) = " << r5 << " (expect 3)\n";
    assert(r5 == 3);

    // 6. string 元素
    std::vector<std::string> sv{"a", "b", "a", "c"};
    auto r6 = count(sv.begin(), sv.end(), std::string("a"));
    std::cout << "count(strings) = " << r6 << " (expect 2)\n";
    assert(r6 == 2);

    // 7. count_if 直接使用
    auto r7 = count_if(v.begin(), v.end(), [](int x) { return x > 2; });
    std::cout << "count_if(v, >2) = " << r7 << " (expect 2)\n";
    assert(r7 == 2);

    // 8. 异构类型比较 (value 是 double，序列元素是 int)
    auto r8 = count(v.begin(), v.end(), 2.0);
    std::cout << "count(v, 2.0) = " << r8 << " (expect 3)\n";
    assert(r8 == 3);

    std::cout << "ALL TESTS PASSED\n";
    return 0;
}