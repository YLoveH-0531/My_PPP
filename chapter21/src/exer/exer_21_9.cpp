/**
 * @file exer_21_9.cpp
 * @author KaKaRot
 * @brief
 * @version 0.1
 * @date 2026-09-08
 *
 * @copyright Copyright (c) 2026
 */

#include "Order.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <list>
#include <string>
#include <vector>

#ifndef DATA_DIR
#error "DATA_DIR must be defined by CMake. Use cmake to build this project."
#endif

const std::string DATA = DATA_DIR;

namespace {

std::vector<Order> make_sample_orders_a()
{
    using date::year;
    return {
        {"Alice Smith",  "12 Elm St",    year{2026}/9/1,  {{"Widget", 2.50, 4}, {"Gadget", 9.99, 1}}},
        {"Bob Johnson",  "34 Oak Ave",   year{2026}/9/2,  {{"Gizmo", 3.00, 10}}},
        {"Carol Davis",  "56 Pine Rd",   year{2026}/9/3,  {{"Widget", 2.50, 2}}},
        {"David Lee",    "78 Maple Dr",  year{2026}/9/4,  {{"Sprocket", 1.25, 6}}},
        {"Eve Turner",   "90 Cedar Ln",  year{2026}/9/5,  {{"Gadget", 9.99, 3}}},
        {"Frank Wright", "11 Birch Ct",  year{2026}/9/6,  {{"Widget", 2.50, 5}, {"Gizmo", 3.00, 2}}},
        {"Grace Hall",   "22 Aspen Way", year{2026}/9/7,  {{"Sprocket", 1.25, 8}}},
        {"Henry Young",  "33 Willow Pl", year{2026}/9/8,  {{"Gadget", 9.99, 2}}},
        {"Ivy King",     "44 Spruce Sq", year{2026}/9/9,  {{"Widget", 2.50, 1}}},
        {"Jack Scott",   "55 Fir Blvd",  year{2026}/9/10, {{"Gizmo", 3.00, 4}}},
        {"Karen Adams",  "66 Palm St",   year{2026}/9/11, {{"Sprocket", 1.25, 3}}},
    };
}

std::vector<Order> make_sample_orders_b()
{
    using date::year;
    auto a = make_sample_orders_a();
    // About a third of file B is identical to file A, so the merge step visibly keeps duplicates.
    std::vector<Order> b{a[2], a[5], a[8], a[9]};
    std::vector<Order> extra{
        {"Liam Baker",    "77 Ash Ct",     year{2026}/9/12, {{"Widget", 2.50, 3}}},
        {"Mia Clark",     "88 Cherry Ln",  year{2026}/9/13, {{"Gadget", 9.99, 1}, {"Sprocket", 1.25, 2}}},
        {"Noah Evans",    "99 Walnut Dr",  year{2026}/9/14, {{"Gizmo", 3.00, 6}}},
        {"Olivia Foster", "10 Chestnut St",year{2026}/9/15, {{"Widget", 2.50, 2}}},
        {"Paul Green",    "21 Hazel Ave",  year{2026}/9/16, {{"Sprocket", 1.25, 5}}},
        {"Quinn Harris",  "32 Poplar Rd",  year{2026}/9/17, {{"Gadget", 9.99, 4}}},
        {"Ruby Irwin",    "43 Sycamore Sq",year{2026}/9/18, {{"Gizmo", 3.00, 1}}},
    };
    b.insert(b.end(), extra.begin(), extra.end());
    return b;
}

}  // namespace

int main()
{
    const std::string file_a        = DATA + "exer_21_9_orders_a.txt";
    const std::string file_a_sorted = DATA + "exer_21_9_orders_a_by_name.txt";
    const std::string file_b        = DATA + "exer_21_9_orders_b.txt";
    const std::string file_b_sorted = DATA + "exer_21_9_orders_b_by_address.txt";
    const std::string file_merged   = DATA + "exer_21_9_orders_merged.txt";

    write_orders(file_a, make_sample_orders_a());
    write_orders(file_b, make_sample_orders_b());

    std::vector<Order> orders_a;
    read_orders(file_a, orders_a);
    std::sort(orders_a.begin(), orders_a.end(), by_name);
    write_orders(file_a_sorted, orders_a);

    std::list<Order> orders_b;
    read_orders(file_b, orders_b);
    orders_b.sort(by_address);
    write_orders(file_b_sorted, orders_b);

    // std::merge requires both ranges sorted by the same key; orders_a is already sorted
    // by name, so only orders_b (currently sorted by address) needs to be re-sorted.
    orders_b.sort(by_name);

    std::vector<Order> merged;
    std::merge(orders_a.begin(), orders_a.end(),
               orders_b.begin(), orders_b.end(),
               std::back_inserter(merged), by_name);
    write_orders(file_merged, merged);

    std::cout << "orders_a (by name):    " << orders_a.size() << " -> " << file_a_sorted << "\n"
              << "orders_b (by address): " << orders_b.size() << " -> " << file_b_sorted << "\n"
              << "merged (by name):      " << merged.size()   << " -> " << file_merged   << "\n\n"
              << "=== Merged Orders ===\n";
    for (const auto& o : merged) print(std::cout, o);

    return 0;
}
