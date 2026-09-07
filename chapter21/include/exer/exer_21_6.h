#ifndef EXER_21_6_H
#define EXER_21_6_H

#include <string>

struct Fruit {
    std::string name;
    int count;
    double unit_price;
    //...
};

struct Fruit_order {
    bool operator()(const Fruit *a, const Fruit *b) const {
        return a->name < b->name;
    }
};

#endif // EXER_21_6_H