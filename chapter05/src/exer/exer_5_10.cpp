/**
 * @file     exer_5_10.cpp
 * @author   KaKaRot
 * @date     2/4/2026
 * @brief    PPP Chapter 5 exer 10.
 * @details  calculate the N–1 differences between adjacent values and write out that vector of differences.
 */

#include <iostream>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <string>

void calculate(unsigned long count)
{
    if (count <= 0)
    {
        std::cout << count << " is not greater than 0\n";
        return ;
    }
    
   std::vector<double> vec_num;
   std::vector<double> vec_diff;
   std::cout << "Enter the " << count << " numbers.\n>";
   std::string line;
   if (std::getline(std::cin, line))
   {
      std::stringstream iss(line);
      double number;
      for (unsigned long i = 0; (iss >> number) && (i < count); i++)
      {
         vec_num.push_back(number);
      }

      if (vec_num.size() == count)
      {
         for (unsigned long i = 0; i < (count - 1); i++)
         {
            vec_diff.push_back(vec_num[i + 1] - vec_num[i]);
         }

         std::cout << "The difference are ";
         for (const auto& i : vec_diff)
         {
            std::cout << i << " ";
         }

         std::cout << '\n';
         
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