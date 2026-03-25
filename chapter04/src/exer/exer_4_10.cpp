/**
 * @file     exer_4_10.cpp
 * @author   KaKaRot
 * @date     25/3/2026
 * @brief    PPP Chapter 4 exercise 10.
 * @details  Rock, Paper, Scissors game
 */

#include <iostream>
#include <vector>
#include <string>
#include <limits>

 int main()
 {
    std::cout << "===========================================\n";
    std::cout << "           Rock, Paper, Scissors           \n";
    std::cout << "        Enter r,p or s ('q' to quit )      \n";
    std::cout << "===========================================\n";

    while (true)
    {
        std::cout << ">" << std::flush;
        char ch;
        
        if (std::cin >> ch)
        {
            std::cout << "normal state\n";
        }
        else if (std::cin.eof())
        {
            std::cout << "[System] Input stream terminated by user. Exiting...\n";
            break;
        }
        else
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Illegal Format! Re-input!";
        }    
    }  
    return 0;
 }
