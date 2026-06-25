/**
 * @file drill_20.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2026-06-21
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "drill_20.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include <list>


int main()
{
    int array[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<int> v_int{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::list<int> l_int{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    // copy constructor
    // int array_c[10];
    // for (int i = 0; i < 10; i++) {
    //     array_c[i] = array[i];
    // }
    // std::vector<int> v_int_c{v_int};
    // std::list<int> l_int_c{v_int};

    std::cout << "array: ";
    KaKaLot::print(array, array + 10);
    std::cout << "v_int: ";
    KaKaLot::print(v_int.begin(), v_int.end());
    std::cout << "l_int: ";
    KaKaLot::print(l_int.begin(), l_int.end());

    // increment
    for (int i = 0; i < 10; i++) {
        array[i] = array[i] + 2;
    }

    for (auto& e : v_int) {
        e += 3;
    }

    for (auto& e : l_int) {
        e += 5;
    }

    std::cout << "***After increment***" << std::endl;
    std::cout << "array: ";
    KaKaLot::print(array, array + 10);
    std::cout << "v_int: ";
    KaKaLot::print(v_int.begin(), v_int.end());
    std::cout << "l_int: ";
    KaKaLot::print(l_int.begin(), l_int.end());

    // copy value
    KaKaLot::copy(array, array + 9, v_int.begin());
    KaKaLot::copy(l_int.cbegin(), l_int.cend(), array);

    std::cout << "***After copy***" << std::endl;
    std::cout << "array: ";
    KaKaLot::print(array, array + 10);
    std::cout << "v_int: ";
    KaKaLot::print(v_int.begin(), v_int.end());
    std::cout << "l_int: ";
    KaKaLot::print(l_int.begin(), l_int.end());

    auto it = std::find(v_int.begin(), v_int.end(), 3);
    if (it != v_int.end()) {
        std::cout << "Found 3 at position " << std::distance(v_int.begin(), it) << std::endl;
    }else {
        std::cout << "3 not found" << std::endl;
    }

    auto it2 = std::find(l_int.begin(), l_int.end(), 27);
    if (it2 != l_int.end()) {
        std::cout << "Found 27 at position " << std::distance(l_int.begin(), it2) << std::endl;
    }else {
        std::cout << "27 not found" << std::endl;
    }
    return 0;
}