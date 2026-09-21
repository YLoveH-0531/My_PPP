/**
 * @file Regex_practices.cpp
 * @author KaKaRot
 * @brief 
 * @version 0.1
 * @date 2026-09-18
 * 
 * @copyright Copyright (c) 2026
 */

#include <regex>
#include <string>
#include <iostream>
#include <iterator>

// regex_replace function
void regex_replace_test()
{
   std::string text{"Quick start learn!"};
   std::regex reg("[a|u|i|o|e]");

   std::regex_replace(std::ostream_iterator<char>(std::cout), text.begin(), text.end(), reg,
                      "#");
   std::cout << std::regex_replace(text, reg, "[$&]") << std::endl;

   std::cout << std::regex_replace(text, reg, "\\d$&") << std::endl;
}

int main()
{
    regex_replace_test();
    return 0;
}