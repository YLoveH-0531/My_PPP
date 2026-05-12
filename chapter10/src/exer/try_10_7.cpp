/**
 * @file     ch10_7.cpp
 * @author   KaKaRot
 * @date     12/5/2026
 * @brief    practice on contents.
 * @details  rewrite some examples from chapter 10.7.
 */

#include <iostream>

void skip_to_int()
{
    std::cin.clear();
    for(char ch; std::cin >> ch;){
        if(std::isdigit(ch) || ch == '-'){
            std::cin.unget();
            return ;
        }
    }
}

int get_int()
{
    int n;
    while(true){
        if(std::cin >> n){
            return n;
        }else{
            if (std::cin.eof()) {  
                std::cout << "No input ..." << '\n';
                continue;
            }
            std::cout << "Format error ..." << '\n';
            skip_to_int();
        }
    }
}

int get_int(int low, int high)
{
    std::cout << "[" << low << ':' << high << "]\n";

    while(true){
        int n = get_int();
        if( n >= low || n <= high){
           return n;
        }
        std::cout << "Not in range..." << 'n';
    }
}

void read()
{
    std::cin.exceptions(std::cin.exceptions() | std::ios::badbit);
    int low = 0, high = 100;
    std::cout << "Please enter an integer in the range "
    << low << " to " << high << " (inclusive):\n";
    int n = get_int(low, high);
    std::cout << "Number is " << n << '\n';
}

int main()
{
    read();
    return 0;
}