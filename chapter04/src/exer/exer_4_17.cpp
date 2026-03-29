/**
 * @file     exer_4_17.cpp
 * @author   KaKaRot
 * @date     28/3/2026
 * @brief    PPP Chapter 4 exercise 17.
 * @details  Find the mode of a seqence of strings.
 */

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>

void CaculateMode(std::vector<char>& vec)
{
    std::sort(vec.begin(), vec.end());

    int count = 1, mode_count = 1;
    char mode = vec[0];

    for (std::size_t i = 1; i < vec.size(); i++) {
        if (vec[i] != vec[i - 1]) {
            if (count > mode_count) {
                mode = vec[i -1];
                mode_count = count;
            }
            count = 1;
        }
        else {
            count++;
        }
    }

    std::cout << "The mode is " << mode << " and occurs " << mode_count << " time" << ((mode_count > 1) ? "s." : ".") << "\n"; 
}

void ModeGame(const std::string& line_input)
{
    std::stringstream ss(line_input);
    std::string token;
    std::vector<char> vec_char;

    while (ss >> token) 
    {
        try {
            for (const auto i : token ) {
                vec_char.emplace_back(i);
            }
        } catch (const std::exception&) {
            continue;
        } 
    }

    CaculateMode(vec_char);
}

int main()
{
    std::cout << "===========================================\n";
    std::cout << "                 Mode  Game                \n";
    std::cout << "   Enter a line of string ('q' to quit )   \n";
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