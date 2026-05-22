/**
 * @file exer_11_4.cpp
 * @author KaKaRot
 * @brief 
 * @version 0.1
 * @date 2026-05-17
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include <cwctype>
#include <iomanip>
#include <ios>
#include <iostream>
#include <string>
#include <cctype>
#include <stdexcept>

/**
 * @brief 
 * 
 * @param str 
 */
void read_multi_int(const std::string& str)
{
    try {
        std::string base;
        long long value;

        if (str.length() >= 3 && str[0] == '0' && str[1] == 'x') {
            base = "hexadecimal";
        }else if (str.length() >= 2 && str[0] == '0') {
            base = "octal";
        }else {
            base = "decimal";
        }

        value = std::stoll(str, nullptr, 0);

        std::cout << std::right << std::setw(10) << str << "  " << std::left << std::setw(15) << base
                  << " converts to " << std::right << std::setw(10) << value << " decimal\n";
        
    } catch (const std::invalid_argument&) {
        std::cout << "format error...\n";
    } catch(const std::out_of_range&){
        std::cout << "out of range...\n";
    }
}

int main()
{
    std::string s;
    std::cout << "input integers using octal, decimal, or hexadecimal\n";
    while (std::cin >> s) {
       read_multi_int(s); 
    }
    return 0;
}