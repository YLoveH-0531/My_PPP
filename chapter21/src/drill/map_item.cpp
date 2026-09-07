/**
 * @file map_item.cpp
 * @author KaKaRot
 * @brief 
 * @version 0.1
 * @date 2026-09-03
 * 
 * @copyright Copyright (c) 2026
 */

#include <istream>
#include <map>
#include <iostream>
#include <numeric>
#include <string> 

void fill_msi(std::istream &os, std::map<std::string, int> &m) {
   std::string key;
   int value;
   while (os >> key >> value) { 
      m[key] = value;
   }
}

int main()
{
   std::map<std::string, int> msi;
   msi["lecture"] = 21;
   msi["Bob"] = 20;
   msi["Jessy"] = 30;
   msi["Ben"] = 17;
   msi["Rose"] = 25;
   msi["Jack"] = 26;
   msi["Lenerd"] = 31;
   msi["Shelden"] = 32;
   msi["Howard"] = 33;
   msi["Penny"] = 35;
   
   //output
   for (const auto& item : msi) {
      std::cout << "(" << item.first << "," << item.second << ")\n";
   }
   
   //erase
   std::cout << "erase..." << std::endl;
   msi.erase("lecture");
   auto pos = msi.find("Jessy");
   if (pos != msi.end()) {
      msi.erase(pos);
   }
   for (const auto& item : msi) {
      std::cout << "(" << item.first << "," << item.second << ")\n";
   }
   
   //read from cin
//   fill_msi(std::cin, msi);
//   std::cout << "read from cin" << std::endl;
//   for (const auto& item : msi) {
//      std::cout << "(" << item.first << "," << item.second << ")\n";
//   }
   
   //output sum integers of the msi
   int sum = 0;
   std::accumulate(msi.cbegin(), msi.cend(), 0.0,
                   [&sum](int init, std::pair<std::string, int> elm) { sum = init + elm.second; return sum; });
   std::cout << "sum is " << sum << "\n";
   
   std::map<int, std::string> mis;
   for (const auto& item : msi) {
      mis[item.second] = item.first;
   }
   std::cout << "mis..." << "\n";
   for (const auto& item : mis) {
      std::cout << "(" << item.first << "," << item.second << ")\n";
   }
}