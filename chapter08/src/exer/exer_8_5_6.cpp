/**
 * @file     exer_8_5_6.cpp
 * @author   KaKaRot
 * @date     21/4/2026
 * @brief    Reverse the elements of a vector in-place.
 * @details  Does not allocate any additional vector; Swaps elements from both ends toward the center.
 */

#include "exer_8_5_6.h"

void reverse_vector_int(std::vector<int>& vec)
{
    for(int_vec_sz i = 0; i < vec.size() / 2; ++i)
    {       
        int temp = 0;
        temp = vec[i];
        vec[i] = vec[vec.size() - 1 - i];
        vec[vec.size() - 1 - i] = temp;
    }
}


std::vector<int> reverse_vector_int_not_inplace(std::vector<int>& vec)
{
    std::vector<int> reversed_vec;
    for(const auto& i : vec)
    {
        reversed_vec.push_back(i);
    }
    return reversed_vec;
}

void reverse_vector_str(std::vector<std::string>& vec)
{
    for(str_vec_sz i = 0; i < vec.size() / 2; ++i)
    {       
        std::string temp;
        temp = vec[i];
        vec[i] = vec[vec.size() - 1 - i];
        vec[vec.size() - 1 - i] = temp;
    }
}

std::vector<std::string> reverse_vector_str_not_inplace(std::vector<std::string>& vec)
{
    std::vector<std::string> reversed_vec;
    for(const auto& s : vec)
    {
        reversed_vec.push_back(s);
    }
    return reversed_vec;
}
 
