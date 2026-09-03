#ifndef ITEM_H
#define ITEM_H
#include <string>

struct Item { 
    std::string name; 
    int iid;
    double value;
    /* . . . */
    Item(std::string n, int i, double v) // 显式构造函数，emplace_back 需要它来做"原地构造"
        : name{std::move(n)}, iid{i}, value{v} {}
};

#endif // ITEM_H