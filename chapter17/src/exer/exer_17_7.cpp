/**
 * @file exer_17_7.cpp
 * @author KaKaRot
 * @brief 
 * @version 0.1
 * @date 2026-05-26
 * 
 * @copyright Copyright (c) 2026
 */

#include <iostream>

void read(char terminator = '!')
{
    char ch;
    unsigned long size = 100;
    unsigned long buffer = 0;
    char *p = new char[size];
    while (std::cin.get(ch) && ch != terminator){
        p[buffer++] = ch;
        if (buffer == size){
            size *= 2;
            char* new_p = new char[size];
            for (unsigned long i = 0; i < buffer; ++i){
                new_p[i] = p[i];
            }
            delete[] p;
            p = new_p;
        }
    }
    // Do something with the read characters, e.g., print them
    for (unsigned long i = 0; i < buffer; ++i){
        std::cout << p[i];
    }
    std::cout << std::endl;
    delete[] p;
}

int main()
{
    read();
    return 0;
}