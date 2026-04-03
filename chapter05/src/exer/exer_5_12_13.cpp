/**
 * @file     exer_5_12_13.cpp
 * @author   KaKaRot
 * @date     2/4/2026
 * @brief    PPP Chapter 5 exer 12, 13.
 * @details  Bulls and Cows.
 */

#include <iostream>
#include <sstream>
#include <string>
#include <random>
#include <stdexcept>

int generateRandom()
{
   static std::mt19937 gen(std::random_device{}());
   std::uniform_int_distribution<int> d(0,9);
   return d(gen);
}

void Play(const std::string& anwser, const std::string& input)
{
   int Bulls = 0;
   int Cows = 0;

   for (std::string::size_type i = 0; i < input.size(); i++)
   {
      std::string::size_type index = anwser.find(input[i]);
      if(index != std::string::npos)
      {
         if (index == i){ Bulls++; }
         else{
            Cows++;
         }         
      }
   }
   if (Bulls == 4){
      std::cout << Bulls << " Bulls!!! \n You are the champion!";
   }else{
      std::cout << Bulls << " Bulls and " << Cows << " Cows. Try again.\n";
   }   
}

void printfun()
{
   std::cout << "===================================================\n" ;
   std::cout << "                   Number Guess Game               \n";
   std::cout << "       Enter 4 numbers to play('q' to quit )       \n";
   std::cout << "===================================================\n" ;
}

std::string handleInput(const std::string& line_input)
{
   std::istringstream iss(line_input);
   std::string match;
   int num_cnt = 0;

   try
   {
      char ch;        
      while (iss >> ch && num_cnt < 5)
      {
         if (std::isdigit(ch)){
            match.push_back(ch);
            num_cnt++;
         }else{
            std::cout << "[!] " << ch << " is not digit! Re-input!\n";
            break;
         }
      }

      if (num_cnt == 5){
         std::cout << "[!] Wrong number counts. Re-input!\n";
        return std::string();
      }
      
      if (!iss.good()){
         std::cout << "[!] " << ch << " is not valid charatter! Re-input!\n";
         return std::string();
      }
   }
   catch(const std::exception&)
   {
      std::cout << "[!] Illegal format! Re-input!\n";
      return std::string();
   }
   
   if (match.size() == 4)
   {
      return match;
   }
}

void Game()
{
   printfun();
   std::string line_input;
   std::string anwser;
   std::string match;
   
   while (std::cout << ">" << std::flush)
   {
      std::getline(std::cin, line_input);
      if (line_input == "q" || line_input == "Q") { break; }
      match = handleInput(line_input);
      if(!match.empty()) { Play(match); }
      if (std::cin.eof()) { break; }
   }
   std::cout << "Good Luck! Bye~ \n";
}

int main()
{
   std::cout << "===================================================\n" ;
   std::cout << "           Welcom to Number Guess Game             \n";
   std::cout << "===================================================\n" ;
   std::string line_input;

   do
   {
      Game();
      std::cout << "===================================================\n" ;
      std::cout << "         Still want to play(y/n to choose)         \n";
      std::cout << "===================================================\n" ;
      if (std::cout << ">" << std::flush && std::getline(std::cin, line_input))
      {
         if (line_input == "n" || line_input == "N") { break; }
         if (std::cin.eof()) { break; }
      }
   }while (true);
   Game();
   return 0;
}
