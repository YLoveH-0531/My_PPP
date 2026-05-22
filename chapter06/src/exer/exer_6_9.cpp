/**
 * @file     exer_6_9.cpp
 * @author   KaKaRot
 * @date     13/4/2026
 * @brief    PPP Chapter 6 exercise 9.
 * @details  Bit operation
 */

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>

void print(const std::vector<char>& number)
{  
   const std::string singular[] = {"one", "ten", "hundred", "thousand"};
   const std::string plural[]   = {"ones", "tens", "hundreds", "thousands"};

   int value = 0;
   for (size_t i = 0; i < number.size(); i++)
   {
      value += (number[number.size() - i -1] - '0' ) * (int)std::pow(10, i);
   }
   
   std::cout << value << " is ";
   for (size_t i = 0; i < number.size(); i++)
   {
      int place = number.size() - i -1;
      int digit = number[i] - '0';
      std::cout << digit << " " << (digit == 1 ? singular[place] : plural[place]) << " ";
      if (i < number.size() - 1)
      {
         std::cout << "and ";
      }
   }
   std::cout << "\n";
}

void process(const std::string& line)
{
   std::vector<char> integer_value;
   std::istringstream iss(line);
   char ch;
   bool fisrtNumber = true;
   int count = 0;
   while (iss >> ch)
   {
      count++;
      if (ch >= 48 && ch <= 57 )
      {
         if (fisrtNumber){
            if (ch == 48) {     
               std::cout << "First number can not be zero. Re-input" << '\n';
               return ;            
            } else{
               fisrtNumber = false;
            }
         }
         integer_value.push_back(ch);
      }
      else
      {
         std::cout << "Not valid integer numbers. Re-input" << '\n';
         return ;
      }
      if (count > 4 )
      {
         std::cout << "Out of range. Re-input" << '\n';
         std::cout << "The valid output is: ";
         print(integer_value);
         return ;
      }
   }
   print(integer_value);
}

int main()
{
   std::cout << "=====================================================================\n";
   std::cout << "                     Welcome to our simple game test                 \n";
   std::cout << "          Please enter onw to four int numbers (q to quit)           \n";
   std::cout << "=====================================================================\n";
   std::string line_input;
   while (std::cout << ">" << std::flush && std::getline(std::cin, line_input)) {
      if (line_input == "q"){ break; }
      process(line_input);
      if(std::cin.eof()) { break; }
   }
   std::cout << "Good Luck! Bye~" << std::endl;
   return 0;
}