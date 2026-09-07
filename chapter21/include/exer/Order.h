#ifndef ORDER_H
#define ORDER_H

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
};

#endif // ORDER_H