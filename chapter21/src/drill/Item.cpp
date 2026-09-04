/**
 * @file Item.cpp
 * @author KaKaRot
 * @brief 
 * @version 0.1
 * @date 2026-09-04
 * 
 * @copyright Copyright (c) 2026
 */

#include "Item.h" 
#include <iostream>

std::ostream& operator<<(std::ostream& os, const Item& it)
{
    os << it.name << ' ' << it.iid << ' ' << it.value;
    return os;
}