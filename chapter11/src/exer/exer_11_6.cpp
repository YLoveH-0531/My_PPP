/**
 * @file exer_11_6.cpp
 * @author KaKaRot
 * @brief  replaces punctuation with whitespace
 * @version 0.1
 * @date 2026-05-17
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#include <iostream>
#include <cctype>

void replace_with_space()
{
    std::cout << "Input a sentence...\n";
    char ch;
    while(std::cin.get(ch)){
        if(std::ispunct(ch) && ch != '"'){
            ch = ' ';
        }
        std::cout << ch;
    }
}

int main()
{
    replace_with_space();
    return 0;
}