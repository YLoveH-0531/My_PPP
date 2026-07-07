/**
 * @file exer_20_12_13.cpp
 * @author KaKaRot
 * @brief 
 * @version 0.1
 * @date 2026-07-07
 * 
 * @copyright Copyright (c) 2026
 */

#include "exer_20_12_13.h"

template<typename Iter> 
// requires Input_Iter<Iter>() (§19.3.3)
Iterator high(Iter first, Iter last)
{ 
    Iterator high = first;
    for (Iterator p = first; p!=last; ++p)
    if (*high<*p) 
        high = p;
    return high;
}

int main()
{
    return 0;
}