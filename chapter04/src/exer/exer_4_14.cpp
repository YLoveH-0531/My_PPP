/**
 * @file     exer_4_14.cpp
 * @author   KaKaRot
 * @date     26/3/2026
 * @brief    PPP Chapter 4 exercise 14.
 * @details  Judge if a number is prime using Sieve of Eratosthenes
 */

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>

void judgeAndprint(const int max)
{
    std::vector<bool> vec_data(max +1, true);
    std::vector<int> vec_prime;

    vec_data[0] = vec_data[1] = false;

    for(int prime = 2; prime * prime <= max;)
    {
        for(int multiple = prime ; multiple * prime <= max; multiple++)
        {
            vec_data[multiple * prime] = false;
        }

        while(!vec_data[++prime])
        {
            continue;
        }
    }

    for(int i = 2; i <= max; i++)
    {
        if(vec_data[i]) { vec_prime.emplace_back(i); }
    }

    if(!vec_prime.empty())
    {
        std::cout << "The prime are: ";
        for(const int& i : vec_prime)
        {
            std::cout << i << " ";
        }
        std::cout << "\n";
    }
    else
    {
        std::cout << "No prime~\n";
    }
}

void SieveEratosthenes(const std::string& line)
{
    std::stringstream ss(line);
    std::string token;

    if(ss >> token)
    {
        try
        {
            std::size_t pos;
            int number = std::stoi(token, &pos);
            if(pos == token.size())
            {
                judgeAndprint(number);
            }
            else
            {
                std::cout << "[!] do not forget using space ...\n";
            }
        }
        catch(const std::exception&)
        {
            std::cout << "[!] input error ...\n";
        }
    }     
}

int main()
{
    std::cout << "===========================================\n";
    std::cout << "                 Prime Game                \n";
    std::cout << "          Enter a number ('q' to quit )    \n";
    std::cout << "===========================================\n";

    std::string line_input;
    while (true)
    {
        std::cout << ">" << std::flush;
        if (!std::getline(std::cin, line_input)) { break; }
        if (line_input == "q") { break; }

        SieveEratosthenes(line_input);

        if (std::cin.eof()) { break; }
    }

    std::cout << "Good Luck! Bye~\n";
    return 0;
}