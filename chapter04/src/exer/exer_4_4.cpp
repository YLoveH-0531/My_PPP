/**
 * @file     exer_4_4.cpp
 * @author   KaKaRot
 * @date     2026
 * @brief    PPP Chapter 4 exercise 4.
 * @details  Play a numbers guessing game with user.
 */

#include<iostream>
#include <string>

int main()
{
    int lower = 1, upper = 100, medium;
    while(lower < upper)
    {   
        medium = (lower + upper) / 2;
        std::cout << "Is The number you guess greater than " << medium << "? Please enter y/n.\n" << std::flush;
        
        std::string word;

        if(!(std::cin >> word))
        {
            std::cout << "Game closed. Welcome again!" << std::endl;
            break;
        }
        char anwser = word[0];

        if(anwser == 'y' || anwser == 'Y')
        {
            lower = medium + 1;
        }
        else if(anwser == 'n'|| anwser == 'N')
        {
            upper = medium;
        }
        else
        {
            std::cout << "Hint:  Please input y/n\n" << std::flush;
        }
    }

    if(lower == upper)
    {
        std::cout << "The number you guess is " << upper << std::endl;
    }

    return 0;
 }