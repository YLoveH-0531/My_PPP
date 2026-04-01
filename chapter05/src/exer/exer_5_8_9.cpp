/**
 * @file     exer_5_8_9.cpp
 * @author   KaKaRot
 * @date     1/4/2026
 * @brief    PPP Chapter 5 exer 8, 9.
 * @details  caculate the sum of the first N numbers give errors when input wrong
 */

#include <iostream>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <string>

void calculate(unsigned long count)
{
   std::vector<int> vec_num;
   std::cout << "Enter the " << count << " numbers.\n>";
   std::string line;
   if (std::getline(std::cin, line))
   {
      std::stringstream oss(line);
      int number;
      for (unsigned long  int i = 0; (oss >> number) && (i < count); i++)
      {
         vec_num.push_back(number);
      }

      if (vec_num.size() == count)
      {
         unsigned long sum = 0;
         for (const auto i : vec_num)
         {
            sum += i;
         }
         std::cout << "The sum of the first " << count << " numbers are " << sum << '\n';
      }
      else
      {
         std::cout << "[!] The data are illegal!\n";
      }
   }
}

void program(const std::string& line)
{
   try
   {
      std::size_t pos;
      int number = std::stoi(line, &pos);
      if (pos == line.size()){
         if (number > 0)
         {
            calculate(number);
         }
         else
         {
            std::cout << "[!] N must greater then zero. Re-input\n";
            return;
         }  
      }else{
         std::cout << "[!] Illegal foramt. Re-input.\n";
         return;
      }
   }
   catch(const std::exception&)
   {
      std::cout << "[!] Illegal foramt. Re-input.\n";
   }
}

void printfun()
{
   std::cout << "===================================================\n" ;
   std::cout << "                       Sum Game                    \n";
   std::cout << "       Enter numbers want to add('q' to quit )     \n";
   std::cout << "===================================================\n" ;
}

int main()
{
   printfun();
   std::string line_input;
   while (std::cout << ">" << std::flush && std::getline(std::cin, line_input))
   {
      if (line_input == "q" || line_input == "quit") { break; }
      program(line_input);
      if (std::cin.eof()) { break; }
      printfun();
   }
   std::cout << "Good Luck! Bye!\n";
   return 0;
}