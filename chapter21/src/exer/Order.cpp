/**
 * @file Order.cpp
 * @author KaKaRot
 * @brief 
 * @version 0.1
 * @date 2026-09-08
 * 
 * @copyright Copyright (c) 2026
 */

#include "Order.h"
#include <fstream>
#include <iomanip>
#include <sstream>

namespace {

// Formats the price independently of the caller's stream so operator<< never mutates
// the passed-in ostream's flags, and so the written value round-trips through std::stod.
std::string format_price(double price)
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << price;
    return oss.str();
}

}  // namespace

std::ostream& operator<<(std::ostream &os, const Purchase &purchase)
{
    return os << purchase.Product << ':' << format_price(purchase.unit_price)
               << ':' << purchase.count;
}

std::istream& operator>>(std::istream &is, Purchase &purchase)
{
    std::string name, price_str;
    if (!std::getline(is, name, ':') || !std::getline(is, price_str, ':')) {
        is.setstate(std::ios::failbit);
        return is;
    }
    int cnt = 0;
    if (!(is >> cnt)) return is;

    purchase.Product = std::move(name);
    purchase.unit_price = std::stod(price_str);
    purchase.count = cnt;
    return is;
}

void print(std::ostream &os, const Purchase &purchase)
{
    os << "Product: " << purchase.Product << "\n"
       << "Count: " << purchase.count << "\n"
       << "Unit Price: " << purchase.unit_price << "\n";
}

std::ostream& operator<<(std::ostream &os, const Order &order)
{
    os << order.Customer << '|' << order.Address << '|' << order.order_date << '|';
    for (std::size_t i = 0; i < order.BuyList.size(); ++i) {
        if (i) os << ';';
        os << order.BuyList[i];
    }
    return os << '\n';
}

std::istream& operator>>(std::istream &is, Order &order)
{
    std::string line;
    if (!std::getline(is, line)) return is;

    std::istringstream fields(line);
    std::string customer, address, date_str, purchases_str;
    if (!std::getline(fields, customer, '|') ||
        !std::getline(fields, address, '|') ||
        !std::getline(fields, date_str, '|') ||
        !std::getline(fields, purchases_str, '|')) {
        is.setstate(std::ios::failbit);
        return is;
    }

    date::year_month_day parsed_date;
    std::istringstream date_stream(date_str);
    date_stream >> date::parse("%F", parsed_date);
    if (!date_stream) {
        is.setstate(std::ios::failbit);
        return is;
    }

    std::vector<Purchase> purchases;
    std::istringstream purchase_list(purchases_str);
    std::string one;
    while (std::getline(purchase_list, one, ';')) {
        std::istringstream item(one);
        Purchase p;
        if (!(item >> p)) {
            is.setstate(std::ios::failbit);
            return is;
        }
        purchases.push_back(std::move(p));
    }

    order.Customer = std::move(customer);
    order.Address = std::move(address);
    order.order_date = parsed_date;
    order.BuyList = std::move(purchases);
    return is;
}

void print(std::ostream &os, const Order &order)
{
    os << "Customer: " << order.Customer << "\n"
                << "Address: " << order.Address << "\n"
                << "Order Date: " << order.order_date << "\n"
                << "Buy List: \n";
    for (const auto& item : order.BuyList) {
        print(os, item);
    }
}

bool by_name(const Order &a, const Order &b)
{
    return a.customer_name() < b.customer_name();
}

bool by_address(const Order &a, const Order &b)
{
    return a.customer_address() < b.customer_address();
}

double Order::value() const
{
    double total = 0.0;
    for (const auto &p : BuyList) total += p.value();
    return total;
}

void append_order(const std::string &filename, const Order &order)
{
    std::ofstream ofs(filename, std::ios::app);
    ofs << order;
}