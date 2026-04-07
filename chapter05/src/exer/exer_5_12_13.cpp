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
#include <algorithm>
#include <vector>

std::string generateRandom()
{
   std::string digits{"0123456789"};
   //std::random_device 通常被称为 非确定性随机数生成器，它提供了一个随机数生成器，可以生成真正的随机数。它通常使用硬件设备（如噪声源）来生成随机数，因此它的输出是不可预测的。
   //std::mt19937 是一种确定性随机数生成器，它基于梅森旋转算法（Mersenne Twister）。它提供了一个高质量的随机数生成器，具有较长的周期和良好的统计性质。它的输出是可预测的，因为它使用一个固定的种子值来生成随机数序列。
   static std::mt19937 gen(std::random_device{}());
   //std::shuffle 是一个算法函数，用于随机打乱一个范围内的元素顺序。它接受三个参数：第一个参数是要打乱的范围的起始迭代器，第二个参数是要打乱的范围的结束迭代器，第三个参数是一个随机数生成器对象。
   std::shuffle(digits.begin(), digits.end(), gen);
   return digits.substr(0, 4);
}

bool Play(const std::string& anwser, const std::string& input)
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
      std::cout << Bulls << " Bulls!!! \nYou are the champion!\n";
      return true;
   }else{
      std::cout << Bulls << " Bulls and " << Cows << " Cows. Try again.\n";
      return false;
   }   
}

void printfun()
{
   std::cout << "==================================================================\n" ;
   std::cout << "       Enter 4 numbers to play('q' to quit/'a' to anwser )        \n";
   std::cout << "==================================================================\n" ;
}

std::string handleInput(const std::string& line_input)
{
   std::istringstream iss(line_input);
   std::string match;
   int num_cnt = 0;

   char ch;        
   while (iss >> ch)
   {
      if (std::isdigit(ch)){
         if (++num_cnt< 5){
            for (std::string::size_type i = 0; i < match.size(); i++){
               if (match[i] == ch){
                  std::cout << "[!] " << ch << " is duplicated! Re-input!\n";
                  return std::string();
               }
            }
            match.push_back(ch);
         }else {
            break;
         };
      }else{
         std::cout << "[!] " << ch << " is not digit! Re-input!\n";
         return std::string();
      }
   }

   if (num_cnt == 5){
      std::cout << "[!] too many number counts. Re-input!\n";
      return std::string();
   }
   else {
      if (num_cnt < 4){
         std::cout << "[!] too few number counts. Re-input!\n";
         return std::string();
      }
   }
   
   if (!iss.good() && !iss.eof()){
      std::cout << "[!] " << ch << " is not valid charatter! Re-input!\n";
      return std::string();
   }
   

   
   if (match.size() == 4){
      return match;
   }else {
      return std::string();
   }
}

void Game()
{
   std::string line_input;
   std::string anwser = generateRandom();
   std::string match;
      
   printfun();
   while (std::cout << ">" << std::flush && std::getline(std::cin, line_input))
   {
      if (line_input == "q" || line_input == "Q") { break; }
      if (line_input == "a" || line_input == "A") { 
         std::cout << "The anwser is " << anwser << '\n';
         break; 
      }
      match = handleInput(line_input);
      if(!match.empty()) { 
         bool bingo = Play(anwser, match);
         if (bingo) { break; }
      }
      if (std::cin.eof()) { break; }
   }
}

int main()
{
   std::cout << "           Welcom to Number Guess Game             \n";
   std::string line_input = "y";

   do
   {
      if (line_input == "y" || line_input == "Y") { Game();}
      std::cout << "===================================================\n" ;
      std::cout << "         Still want to play(y/n to choose)         \n";
      std::cout << "===================================================\n" ;
      if (std::cout << ">" << std::flush && std::getline(std::cin, line_input)){
         if (line_input == "n" || line_input == "N"){
            break;
         }else if (line_input == "y" || line_input == "Y"){ 
            continue; 
         }else {
            std::cout << "[!] Input illegal! Re-input.\n";
         }
      }
      else {
         break;
      }
   }while (true);
   std::cout << "Good Luck! Bye~ \n";
   return 0;
}
