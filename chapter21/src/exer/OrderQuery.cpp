/**
 * @file OrderQuery.cpp
 * @author KaKaRot
 * @brief
 * @version 0.1
 * @date 2026-09-08
 *
 * @copyright Copyright (c) 2026
 */

#include "OrderQuery.h"

std::vector<Order> find_by_customer(const std::vector<Order> &orders, const std::string &customer)
{
    std::vector<Order> result;
    for (const auto &o : orders) {
        if (o.customer_name() == customer) result.push_back(o);
    }
    return result;
}

double total_value(const std::vector<Order> &orders)
{
    double total = 0.0;
    for (const auto &o : orders) total += o.value();
    return total;
}
