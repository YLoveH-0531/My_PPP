/**
 * @file     exer_4_8.cpp
 * @author   KaKaRot
 * @date     34/3/2026
 * @brief    PPP Chapter 4 exercise 8.
 * @details  chess and rice issue
 */

#include <iostream>

void calculate(long long count)
{
    long long total_rice = 0;
    long long rice_per_grid = 0;
    size_t grid = 0;

    for (; total_rice < count; grid++)
    {
        rice_per_grid = 1LL << grid;
        total_rice += rice_per_grid;
        std::cout << "The " << grid + 1 << " grid have " << rice_per_grid << " rices\n";
    }
    std::cout << "The total rices are " << total_rice << " and occurs in " << grid + 1 << " grid\n";
}

int main()
{
    std::cout << "===========================================\n" ;
    std::cout << "              Chess and Rice               \n";
    std::cout << "       Enter rice number('q' to quit )     \n";
    std::cout << "===========================================\n" ;

    while (true)
    {
        std::cout << ">" << std::flush;
        long long riceNum = 0;
        if (std::cin >> riceNum)
        {
            calculate(riceNum);
        }
        else{
            std::string cmd;
            std::cin.clear();
            if(std::cin >> cmd && cmd == "q")
            {
                std::cout << "Good luck! Bye~ \n";
                break;
            }
            else{
                std::cout << "Illegal format! Re-input.\n";
            }
        }  
    }
    return 0;
}