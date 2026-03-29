/**
 * @file     exer_4_16.cpp
 * @author   KaKaRot
 * @date     28/3/2026
 * @brief    PPP Chapter 4 exercise 16.
 * @details  Find the mode of a set of positive integers.
 */

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>

void Caculate(std::vector<int>& vec)
{
    std::sort(vec.begin(), vec.end());

    int count = 1, mode_count = 1, mode = vec[0];
    for(std::size_t i = 1; i < vec.size(); i++)
    {
        if(vec[i] != vec[i-1])
        {
            if(count > mode_count)
            {
                mode = vec[i-1];
                mode_count = count;
            }
            
           count = 1;
        }
        else{
            ++count;
        }
    }

    std::cout << "The mode is " << mode << "\n";
}

void ModeGame(const std::string& line)
{
    std::stringstream ss(line);
    std::string token;
    std::vector<int> vec_int;
    while(ss >> token)
    {
        try{
            std::size_t pos;
            int number = std::stoi(token, &pos);
            if(pos == token.size())
            {
                vec_int.emplace_back(number);
            }
            else{
                continue;
            }
        }
        catch(const std::exception&)
        {
            continue;
        }
    }
    if(!vec_int.empty())
    {
        Caculate(vec_int);
    }
    else
    {
        std::cout << "No valid data...\n";
    }
}

int main()
{
    std::cout << "===========================================\n";
    std::cout << "                 Mode  Game                \n";
    std::cout << "  Enter a line of numbers ('q' to quit )   \n";
    std::cout << "===========================================\n";

    std::string line_input;
    while(std::cout << ">" << std::flush && std::getline(std::cin, line_input))
    {
        if(line_input == "q") { break; }
        ModeGame(line_input);
        if(std::cin.eof()) { break; }
    }
    std::cout << "Good Luck! Bye~\n";
    return 0;
}