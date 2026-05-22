/**
 * @file exer_11_13.cpp
 * @author KaKaRot
 * @brief 
 * @version 0.1
 * @date 2026-05-19
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>
#include <string>

#ifndef DATA_DIR
#error "DATA_DIR must be defined by CMake. Use cmake to build this project."
#endif

const std::string DATA = DATA_DIR;

void reverse_words(const std::string file)
{
   std::cout << "start reverse words...\n";
   std::ifstream ifs(file);
   if (!ifs) {
      std::cerr << "Can not open " << file << " file\n";
      return ;
   }

   std::ofstream ofs(DATA + "exer_11_13_output.txt");
   if (!ofs) {
      std::cerr << "create file failed...\n";
      return ; 
   }

   std::vector<std::string> vec;
   std::string str;
   while (ifs >> str) {
      vec.push_back(str);
   }

   for (auto it = vec.rbegin(); it != vec.rend(); ++it) {
      ofs << *it << " ";
   }
   std::cout << "finish reverse words...\n";
}

int main()
{
   std::string filename = DATA + "exer_11_13_input.txt";
   reverse_words(filename);
   return 0;
}