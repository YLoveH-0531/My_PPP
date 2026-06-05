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
#include <random>
#include <utility>
#include <iostream>

Skip_list::Skip_list()
: rng(std::random_device{}()), head(new node{0, MAX_LEVEL , new node*[MAX_LEVEL]{nullptr, nullptr, nullptr, nullptr, nullptr}}), height(0)
{ 
}

Skip_list::Skip_list(Skip_list&& lhs) noexcept : rng(lhs.rng), head(lhs.head), height(lhs.height)
{
    lhs.head = nullptr;
    lhs.height = 0;
}

Skip_list& Skip_list::operator=(Skip_list&& lhs) noexcept
{
    if (this == &lhs) {
        return *this;
    }
    destory();
    rng = lhs.rng;
    head = lhs.head;
    height = lhs.height;
    lhs.head = nullptr;
    lhs.height = 0;
    return *this;
}

Skip_list::~Skip_list() 
{
    destory();
}

void Skip_list::destory()
{
    if(!head) { return; }
    height = 0;
    node* p = head->succ[height];
    node* q = p;
    while (p) {
        q = p;
        p = p->succ[height];
        delete[] q->succ;
        delete q;
    }
    delete[] head->succ;
    delete head; 
}

node* Skip_list::insert(int value)
{
    node* update[MAX_LEVEL];
    std::pair<bool, node*> match= find(value, update);
    if (match.first) {
        return match.second;     // if found, do nothing
    }

    int level = createLevel();
    node** add_succ = new node*[level];
    node* add = new node{value, level, add_succ};
    for (int i = level -1; i >= 0; i--){
            add->succ[i] = update[i]->succ[i];
            update[i]->succ[i] = add;
    }
    return add;
}

std::pair<bool, node*> Skip_list::find(int value, node** update)
{
    int current_level = MAX_LEVEL;
    node* pc = head;
    while (--current_level >= 0) {
        while (pc->succ[current_level] != nullptr && pc->succ[current_level]->data < value ) {
            pc = pc->succ[current_level];
        }
        update[current_level] = pc;
    }
    node* candidate= pc->succ[0];
    if (candidate != nullptr && candidate->data == value) {
        return std::make_pair(true, candidate);
    }
    return std::make_pair(false, candidate);
}

int Skip_list::createLevel()
{
    int level = 1;
    std::bernoulli_distribution dis(0.5);
    while (level < MAX_LEVEL && dis(rng)) {
        level++;
    }
    return level;
}

node* Skip_list::at(int value){
    node* update[MAX_LEVEL];
    std::pair<bool, node*> match= find(value, update);
    if (match.first) {
        return match.second;
    }else {
        return nullptr;
    }
}

node* Skip_list::erase(int value)
{
    node* update[MAX_LEVEL];
    std::pair<bool, node*> match= find(value, update);
    if (!match.first) {
        return  nullptr;
    }
    for (int i = match.second->height - 1; i >= 0; i--) {
            update[i]->succ[i] = match.second->succ[i];
    }
    delete[] match.second->succ;
    delete match.second;
    return update[0];
}

void Skip_list::print()
{
    int level = MAX_LEVEL;
    std::cout << "Skip list: \n";
    while (--level >= 0) {
        node* p = head;
        std::cout << " level " << level + 1 << ": ";
        while (p->succ[level]) {
            std::cout << p->succ[level]->data << " ";
            p = p->succ[level];
        }
        std::cout << '\n';
    }
}