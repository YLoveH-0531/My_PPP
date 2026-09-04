/**
 * @file vector_item.cpp
 * @author KaKaRot
 * @brief 
 * @version 0.1
 * @date 2026-09-03
 * 
 * @copyright Copyright (c) 2026
 */

#include "Item.h"
#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>

int main()
{
   std::vector<Item> vec;
   std::ostream_iterator<Item> os_it(std::cout, "\n");
   vec.emplace_back("Bob", 111, 5.9);
   vec.emplace_back("Ben", 106, 5.6);
   vec.emplace_back("Ros", 105, 5.7);
   vec.emplace_back("May", 102, 5.8);
   vec.emplace_back("Zac", 100, 5.2);

   std::sort(vec.begin(), vec.end(),
             [](const Item it1, const Item it2) { return it1.name < it2.name; });
   std::copy(vec.begin(), vec.end(), os_it);
   std::cout << std::endl;

   std::sort(vec.begin(), vec.end(),
             [](const Item it1, const Item it2) { return it1.iid < it2.iid; });
   for (const auto& item : vec) {
      os_it = item;
   }
   std::cout << std::endl;

   std::sort(vec.begin(), vec.end(),
             [](const Item it1, const Item it2) { return it1.value < it2.value; });
   for (const auto& item : vec) {
      *os_it++ = item;
   }
   std::cout << std::endl;
   
   std::fill_n(std::back_inserter(vec), 1, Item{"horse shoe",99,12.34});
   std::fill_n(std::back_inserter(vec), 1, Item{"Canon S400", 9988,499.95});
   std::copy(vec.begin(), vec.end(), os_it);
   std::cout << std::endl;
   
   vec.erase(std::remove_if(vec.begin(), vec.end(), [](const Item& it){ return it.name == "Ros"; }), vec.end());
   std::copy(vec.begin(), vec.end(), os_it);
   std::cout << std::endl;
   
   vec.erase(std::remove_if(vec.begin(), vec.end(), [](const Item& it){ return it.name == "Zac"; }), vec.end());
   std::copy(vec.begin(), vec.end(), os_it);
   std::cout << std::endl;
}
