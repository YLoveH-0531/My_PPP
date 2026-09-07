#ifndef ORDER_H
#define ORDER_H

#include <ostream>
#include <vector>
#include <string>
#include "date.h"

class Purchase;
class Order{
    private:
    std::string Customer;
    std::string Address;
    date::year_month_day order_date;
    std::vector<Purchase> BuyList;
    
    friend std::ostream& operator<<(std::ostream &os, const Order &order);
};

class Purchase{
    private:
    std::string Product;
    int count;
    double unit_price;
    
    friend std::ostream& operator<<(std::ostream &os, const Purchase &purchase);
};

std::ostream& operator<<(std::ostream &os, const Order &order)
{
    os << "Customer: " << order.Customer << "\n"
                << "Address: " << order.Address << "\n"
                << "Order Date: " << order.order_date << "\n"
                << "Buy List: \n";
    for (const auto& item : order.BuyList) {
        os << item << "\n";
    }
}

std::ostream& operator<<(std::ostream &os, const Purchase &purchase)
{
    os << "Product: " << purchase.Product << "\n"
                << "Count: " << purchase.count << "\n"
                << "Unit Price: " << purchase.unit_price << "\n";
}
#endif // ORDER_H