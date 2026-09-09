#ifndef ORDER_H
#define ORDER_H

#include <fstream>
#include <istream>
#include <ostream>
#include <vector>
#include <string>
#include "date.h"

class Purchase{
    private:
    std::string Product;
    double unit_price = 0.0;
    int count = 0;

    public:
    Purchase() = default;
    Purchase(std::string product, double price, int cnt)
        : Product(std::move(product)), unit_price(price), count(cnt) {}

    const std::string& product_name() const { return Product; }
    double value() const { return unit_price * count; }

    // File I/O mechanism: compact "Product:unit_price:count" format, must stay parseable by operator>>.
    friend std::ostream& operator<<(std::ostream &os, const Purchase &purchase);
    friend std::istream& operator>>(std::istream &is, Purchase &purchase);
    // Printing mechanism: human-readable, not required to round-trip through operator>>.
    friend void print(std::ostream &os, const Purchase &purchase);
};

class Order{
    private:
    std::string Customer;
    std::string Address;
    date::year_month_day order_date;
    std::vector<Purchase> BuyList;

    public:
    Order() = default;
    Order(std::string customer, std::string address,
          date::year_month_day date, std::vector<Purchase> purchases)
        : Customer(std::move(customer)), Address(std::move(address)),
          order_date(date), BuyList(std::move(purchases)) {}

    const std::string& customer_name() const { return Customer; }
    const std::string& customer_address() const { return Address; }
    double value() const;

    // File I/O mechanism: one Order per line, fields separated by '|'.
    friend std::ostream& operator<<(std::ostream &os, const Order &order);
    friend std::istream& operator>>(std::istream &is, Order &order);
    // Printing mechanism: human-readable, separate from the file format above.
    friend void print(std::ostream &os, const Order &order);
};

// Sort keys differ per use (vector<Order> by name, list<Order> by address), so these are
// free comparators handed to std::sort/list::sort rather than a single Order::operator<.
bool by_name(const Order &a, const Order &b);
bool by_address(const Order &a, const Order &b);

// Shared with exer_21_10/11/12; header-only because they're templates over arbitrary Container.
template <class Container>
void write_orders(const std::string &filename, const Container &orders)
{
    std::ofstream ofs(filename);
    for (const auto &o : orders) ofs << o;
}

template <class Container>
void read_orders(const std::string &filename, Container &orders)
{
    std::ifstream ifs(filename);
    Order o;
    while (ifs >> o) orders.push_back(o);
}

void append_order(const std::string &filename, const Order &order);

#endif // ORDER_H