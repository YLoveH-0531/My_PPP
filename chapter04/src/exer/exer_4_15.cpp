/**
 * @file     exer_4_10.cpp
 * @author   KaKaRot
 * @date     26/3/2026
 * @brief    PPP Chapter 4 exercise 15.
 * @details  Output the first N prime
 */

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <cmath>

void judgeAndprint(const int n)
{
    std::vector<int> vec_prime;
    vec_prime.reserve(n);
    vec_prime.emplace_back(2);

    for(int prime = 3; vec_prime.size() < static_cast<std::size_t>(n); prime+=2)
    {
        bool is_prime = true;
        int root = static_cast<int>(sqrt(prime));

        for(const auto& i : vec_prime)
        {
            if(i > root) { break; }
            if(prime % i == 0)
            {
                is_prime = false;
                break;
            }
        }

        if(is_prime)
        {
            vec_prime.emplace_back(prime);
        }

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