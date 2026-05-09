/**
 * @file     exer_10_6_7.cpp
 * @author   KaKaRot
 * @date     8/5/2026
 * @brief    Roman_int class for holding Roman numerals 
 * @details  Define a Roman_int class for holding Roman numerals
 */

#include "exer_10_6_7.h"
#include <unordered_map>
#include <cctype>
#include <vector>
#include <utility>

// convert roman number to integer, throws out_of_range if s contains invalid character
static int roman_to_int(const std::string& roman) {
    static std::unordered_map<char, int> roman_map = {
    {'I', 1},
    {'V', 5},
    {'X', 10},
    {'L', 50},
    {'C', 100},
    {'D', 500},
    {'M', 1000}
};
    int total = 0;

    for(std::string::size_type i = 0; i < roman.length(); ++i){
        int value = roman_map.at(std::toupper(roman[i]));
        if( (i < roman.length() - 1) && value < roman_map.at(std::toupper(roman[i + 1]))){
            total -= value;
        }else{
            total += value;
        }
    }
    return total;
}

// convert integer to roman number
static std::string int_to_roman(int val) {
    static const std::vector<std::pair<int, std::string>> value_symbols{
        {1000, "M"}, {900, "CM"}, {500, "D"}, {400, "CD"}, {100, "C"}, {90, "XC"},
        {50, "L"}, {40, "XL"}, {10, "X"}, {9, "IX"}, {5, "V"}, {4, "IV"}, {1, "I"}
    };
    std::string s;
    for (const auto& p : value_symbols) {
        while (val >= p.first) {
            s += p.second;
            val -= p.first;
        }
        if (val == 0)
            break;
    }
    return s;
}

Roman_int::Roman_int(const std::string& s) : roman(s), val(roman_to_int(s)) { }

Roman_int::Roman_int(int v) : roman(int_to_roman(v)), val(v) { }

std::ostream& operator<<(std::ostream& os, const Roman_int& r) {
    os << r.to_string(); // Output the Roman numeral string
    return os;
}

std::istream& operator>>(std::istream& is, Roman_int& r) {
    std::string input;
    if(is >> input){
        r = Roman_int(input);
    }
    return is;
}

Roman_int operator+(const Roman_int& lhs, const Roman_int& rhs)
{
    return lhs.val + rhs.val;
}