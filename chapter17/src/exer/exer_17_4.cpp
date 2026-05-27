/**
 * @file exer_17_4.cpp
 * @author KaKaRot
 * @brief 
 * @version 0.1
 * @date 2026-05-26
 * 
 * @copyright Copyright (c) 2026
 */

#include <iostream>

char* strdup(const char* s)
{
    int length = 0;
    for (int i = 0; s[i] != '\0'; ++i){
        length++;
    }

    char* copy = new char[length + 1];
    for (int i = 0; i <= length; ++i){
        copy[i] = s[i];
    }
    copy[length] = '\0';
    return copy;
}

int main()
{
    const char* str = "Hello, World!";
    char* copy = strdup(str);
    
    for (int i = 0; copy[i] != '\0'; ++i){
        std::cout << copy[i];
    }
    std::cout << std::endl;
    return 0;
}