/**
 * @file     exer_8_test.cpp
 * @author   KaKaRot
 * @date     21/4/2026
 * @brief    Test exercises.
 * @details  This file used to test the exercises in chapter 8
**/

#include "exer_8_2.h"
#include "exer_8_5_6.h"

int main()
{
    std::vector<int> vec_int = {1, 2, 3, 4, 5};
    std::vector<std::string> vec_str = {"Hello", "World", "C++", "Programming"};
    print("Original vector of integers: ", vec_int, std::cout);
    reverse_vector_int(vec_int);
    print("Reversed vector of integers: ", vec_int, std::cout); 
    reverse_vector_int_not_inplace(vec_int);
    print("Reversed vector of integersnot_inplace: ", vec_int, std::cout);

    print("Original vector of strings: ", vec_str, std::cout);
    reverse_vector_str(vec_str);
    print("Reversed vector of strings: ", vec_str, std::cout);
    reverse_vector_str_not_inplace(vec_str);
    print("Reversed vector of strings not_inplace: ", vec_str, std::cout);

    return 0;
}
 