/**
 * @file exer_17_5.cpp
 * @author KaKaRot
 * @brief 
 * @version 0.1
 * @date 2026-05-26
 * 
 * @copyright Copyright (c) 2026
 */

#include <iostream>

char* findx(const char* s, const char* x)
{
    for(int i = 0; s[i]!= '\0'; ++i){
        int j = 0;
        while (s[i + j] != '\0' && x[j] != '\0' && s[i + j] == x[j] ){
            j++;
        }
        if (x[j] == '\0'){
            return const_cast<char*>(s + i);
        }

    }
    return nullptr;
}

int main()
{
    const char* str = "Hello, World!";
    const char* substr = "World";
    char* result = findx(str, substr);
    if (result != nullptr){
        std::cout << "Substring found at position: " << (result - str) << std::endl;
    } else {
        std::cout << "Substring not found." << std::endl;
    }
    return 0;
}