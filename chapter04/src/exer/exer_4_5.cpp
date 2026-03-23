/**
 * @file     exer_4_5.cpp
 * @author   KaKaRot
 * @date     2026
 * @brief    PPP Chapter 4 exercise 5.
 * @details  A simple caculater.
 */

#include <iostream>
#include <sstream>
#include <string>

int main()
{
    std::string line;
    
    std::cout << "Please enter an expression (e.g., 10 + 5): " << std::endl;    
    while(std::getline(std::cin, line))
    {
        std::stringstream ss(line);
        double num1, num2;
        char operation;

        if(ss >> num1 >> num2 >> operation)
        {
            switch(operation)
            {
                case '+': 
                    std::cout << "The add of " << num1 << " and " << num2 << " is " << num1 + num2 << std::flush;
                    break;
                case '-': 
                    std::cout << "The subtract of " << num1 << " and " << num2 << " is " << num1 - num2 << std::flush;
                    break;
                case '*': 
                    std::cout << "The multiply of " << num1 << " and " << num2 << " is " << num1 * num2 << std::flush;
                    break;
                case '/':
                    if(num2 == 0)
                    {
                        std::cout << "Error! Devided by 0" << std::flush;
                    }
                    else
                    {
                        std::cout << "The divide of " << num1 << " and " << num2 << " is " << num1 / num2 << std::flush;
                    }
                    break;
                default:
                    std::cout << "Invalid Operator! " << std::flush;
                    break;
            } 
            std::cout << "\nPlease input the two numbers and operation" << std::endl;
        }
        else
        {
            std::cout << "Please input right format! \n" << std::flush;
        }
    }
    return 0;
}