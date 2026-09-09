/**
 * @file exer_21_10.cpp
 * @author KaKaRot
 * @brief
 * @version 0.1
 * @date 2026-09-08
 *
 * @copyright Copyright (c) 2026
 */

#include "Order.h"
#include "OrderQuery.h"
#include <iostream>
#include <string>
#include <vector>

#ifndef DATA_DIR
#error "DATA_DIR must be defined by CMake. Use cmake to build this project."
#endif

const std::string DATA = DATA_DIR;

int main()
{
    const std::string file_a = DATA + "exer_21_9_orders_a.txt";
    const std::string file_b = DATA + "exer_21_9_orders_b.txt";

    std::vector<Order> orders_a, orders_b;
    read_orders(file_a, orders_a);
    read_orders(file_b, orders_b);

    double total_a = total_value(orders_a);
    double total_b = total_value(orders_b);

    std::cout << "Total value of " << file_a << ": " << total_a << "\n"
              << "Total value of " << file_b << ": " << total_b << "\n"
              << "Grand total: " << (total_a + total_b) << "\n";

    return 0;
}
