/**
 * @file     exer_6_10.cpp
 * @author   KaKaRot
 * @date     13/4/2026
 * @brief    PPP Chapter 6 exercise 10.
 * @details  permutation
 */

#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>

long long factorial(int n) {
    long long res = 1;
    for (int i = 2; i <= n; ++i) {
        res *= i;
    }
    return res;
}

void permutation()
{
   int a, b;
   std::cout << "Please enter two int numbers (q to quit): \n";
   std::string line;

   while (std::cout << ">" << std::flush && std::getline(std::cin, line)) {
      if (line == "q") { break; }
      std::istringstream iss(line);
      if (!(iss >> a >> b)) {
         std::cout << "Invalid input. Re-input: ";
         continue;
      }
      if (a < 0 || b < 0) {
         std::cout << "Negative numbers are not allowed. Re-input: \n";
         continue;
      }
      if (a < b) {
         std::cout << "a should be greater than or equal to b. Re-input: \n";
         continue;
      }
      long long res = factorial(a) / factorial(a - b);
      std::cout << "P(" << a << ", " << b << ") = " << res << "\n";
      if(std::cin.eof()) { break; }
      std::cout << "Please enter two int numbers (q to quit): \n";
   }
}

void combinations()
{  
   int a, b;
   std::cout << "Please enter two int numbers (q to quit): \n";
   std::string line;

   while (std::cout << ">" << std::flush && std::getline(std::cin, line)) {
      if (line == "q") { break; }
      std::istringstream iss(line);
      if (!(iss >> a >> b)) {
         std::cout << "Invalid input. Re-input: ";
         continue;
      }
      if (a < 0 || b < 0) {
         std::cout << "Negative numbers are not allowed. Re-input: \n";
         continue;
      }
      if (a < b) {
         std::cout << "a should be greater than or equal to b. Re-input: \n";
         continue;
      }
      long long res = factorial(a) / (factorial(b) * factorial(a - b));
      std::cout << "C(" << a << ", " << b << ") = " << res << "\n";
      if(std::cin.eof()) { break; }
      std::cout << "Please enter two int numbers (q to quit): \n";
   }
}

void print()
{
   std::cout << "================================================================\n";
   std::cout << "                Welcome to our permutation game                 \n";
   std::cout << "                     1. Permutation                             \n";
   std::cout << "                     2. Combination                             \n";
   std::cout << "                 Please enter 1 or 2 (q to quit)                \n";
   std::cout << "================================================================\n";
}

int main()
{
   print();
   std::string line_input;
   while (std::cout << ">" << std::flush && std::getline(std::cin, line_input)) {
      if (line_input == "q"){ break; }
      if (line_input == "1"){
         permutation();
      }else if (line_input == "2")
      {
        combinations();
      }else{
         std::cout << "Invalid input. Re-input" << '\n';
         continue;
      }
      if(std::cin.eof()) { break; }
      print();
   }
   std::cout << "Good Luck! Bye~" << std::endl;
   return 0;
}