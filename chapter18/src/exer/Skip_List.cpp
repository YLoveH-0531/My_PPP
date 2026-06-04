/**
 * @file Skip_List.cpp
 * @author KaKaRot
 * @brief 
 * @version 0.1
 * @date 2026-06-04
 * 
 * @copyright Copyright (c) 2026
 */
 
#include "Skip_List.h"

Skip_list::Skip_list(const Skip_list& lhs)
{

}

Skip_list& Skip_list::operator=(const Skip_list& lhs)
{

}

Skip_list::~Skip_list() 
{
    hight = 0;
    node* p = head->succ[hight];
    while (!p) {
        node* p = 
    }
    delete head; 
}
