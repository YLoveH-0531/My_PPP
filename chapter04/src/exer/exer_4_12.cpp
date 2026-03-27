/**
 * @file     exer_4_10.cpp
 * @author   KaKaRot
 * @date     26/3/2026
 * @brief    PPP Chapter 4 exercise 10.
 * @details  Judge if a number is prime
 */

#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>

static const std::vector<int> prime_cons{2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97};

void judgePrime(const std::string& input, std::vector<int>& prime_vec)
{
   std::stringstream ss(input);
   std::string token;
   while(ss >> token)
   {
      try
      {
         std::size_t pos;
         int number = std::stoi(token, &pos);
         if (pos == token.size() && (std::find(prime_cons.cbegin(), prime_cons.cend(), number) != prime_cons.end()))
         {
            if(std::find(prime_vec.cbegin(), prime_vec.cend(), number) == prime_vec.end())
            {
               prime_vec.emplace_back(number);
            }  
         }        
      }
      catch(const std::exception&)
      {
        continue;
      }
   }
}

int main()
{

   std::cout << "===========================================\n";
   std::cout << "                 Prime Game                \n";
   std::cout << "   Enter a number(0 ~100) ('q' to quit )   \n";
   std::cout << "===========================================\n";

   std::string line_input;
   while(true)
   {
      std::cout << ">" << std::flush;
      if (!std::getline(std::cin, line_input)){ break; }
      if(line_input == "q") { break; }
      std::vector<int> prime;
      judgePrime(line_input, prime);
      if (prime.empty())
      {
         std::cout << "No prime\n";
      }
      else{
         std::cout << "Prime number: ";
         for (const auto& i : prime)
         {
            std::cout << i << " ";
         }
         std::cout << '\n'; 
      }
      if (std::cin.eof()) { break; }    
   }
   std::cout << "Good Luck! Bye~\n";
   return 0;
}