/**
 * @file learn_and_practice.cpp
 * @author KaKaRot
 * @brief 
 * @version 0.1
 * @date 2026-09-02
 * 
 * @copyright Copyright (c) 2026
 */

#include <map>
#include <iostream>
#include <numeric>
#include <string>
#include <utility>

double weight_value(const std::pair<std::string, double> p1,
                    const std::pair<std::string, double> p2) {
    return p1.second * p2.second;
}

int main()
{
    std::map<std::string, double> dow_price = {
        // Dow Jones Industrial index (symbol,price);
        // for up-to-date quotes see
        // www.djindexes.com
        {"MMM", 81.86},
        {"AA", 34.69},
        {"MO", 54.45},
        // . . .
    };

    std::map<std::string, double> dow_weight = {
        // Dow (symbol,weight)
        {"MMM", 5.8549},
        {"AA", 2.4808},
        {"MO", 3.8940},
        // . . .
    };

    std::map<std::string,std::string> dow_name = {
        // Dow (symbol,name)
        {"MMM","3M Co."},
        {"AA","Alcoa Inc."},
        {"MO", "Altria Group Inc."},
        // . . .
    };
    
    for (const auto& p : dow_price) {
       const std::string& symbol = p.first;
       std::cout << symbol
                 << p.second 
                 << dow_name[symbol] << std::endl;
    }

    double price = std::inner_product(dow_price.begin(), dow_price.end(), dow_weight.begin(), 0.0,
                                      std::plus<double>(), weight_value);
    std::cout << "Price = " << price << '\n';
}