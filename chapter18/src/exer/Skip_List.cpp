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

Skip_list::Skip_list(const Skip_list& lhs) : head(lhs.head), hight(lhs.hight)
{
}

Skip_list& Skip_list::operator=(const Skip_list& lhs)
{
    if (this == &lhs) {
        return *this;
    }
    destory();
    head = lhs.head;
    hight = lhs.hight;
    return *this;
}

Skip_list::Skip_list(Skip_list&& lhs) noexcept : head(lhs.head), hight(lhs.hight)
{
    lhs.head = nullptr;
    lhs.hight = 0;
}

Skip_list& Skip_list::operator=(Skip_list&& lhs) noexcept
{
    if (this == &lhs) {
        return *this;
    }
    destory();
    head = lhs.head;
    hight = lhs.hight;
    lhs.head = nullptr;
    lhs.hight = 0;
    return *this;
}

Skip_list::~Skip_list() 
{
    hight = 0;
    node* p = head->succ[hight];
    node* q = p;
    while (!p) {
        q = p;
        p = p->succ[hight];
        delete q;
    }
    delete head; 
}