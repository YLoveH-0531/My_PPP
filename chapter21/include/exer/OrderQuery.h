#ifndef ORDER_QUERY_H
#define ORDER_QUERY_H

#include <string>
#include <vector>
#include "Order.h"

std::vector<Order> find_by_customer(const std::vector<Order> &orders, const std::string &customer);
double total_value(const std::vector<Order> &orders);

#endif // ORDER_QUERY_H
