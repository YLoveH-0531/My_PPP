/**
 * @file     exer_4_19_21.cpp
 * @author   KaKaRot
 * @date     29/3/2026
 * @brief    PPP Chapter 4 exercise 19-21.
 * @details  grade
 */

#include <cstddef>
#include <cmath>
#include <exception>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <limits>

struct Grade
{
    std::string name;
    int scores;
};

std::vector<Grade> vec_gra;

void display()
{
    for (const auto& i : vec_gra)
    {
        std::cout << i.name << " scores " << i.scores << " points\n";
    }
    
}

void gradeInput()
{
    std::cout << "=========================================================\n";
    std::cout << "       Please Input Name and Score (eg, Jack 80)         \n";
    std::cout << "               ('Input Noname 0 to quit )                \n";
    std::cout << "=========================================================\n";
    std::string line_input;

    while (std::cout << ">" << std::flush && std::getline(std::cin, line_input)) {
        if (line_input == "Noname 0") { display(); break; }

        std::stringstream ss(line_input);
        std::string name;
        int score;
        while (ss >> name ) {
            if (!(ss >> score))
            {
                std::cout << "[!] '" << name << "' has no matching score. Re-input.\n";
                break;
            }
            
            std::vector<Grade>::const_iterator it = std::find_if(vec_gra.cbegin(), vec_gra.cend(), [&name](const Grade& val) { return val.name == name; });
            if (it == vec_gra.cend())
            {
                vec_gra.push_back({name, score});
            }
            else{
                std::cout << "[!] Name " << name << "already exist.\n";  
            }
        }
        if (!ss.eof()){
            std::cout << "[!] Illegal format. Re-input.\n";   
        }
        if (std::cin.eof()) {  break; }
    }
}
                
void checkByName()
{
    std::cout << "=========================================================\n";
    std::cout << "             Please Input Name you want to check         \n";
    std::cout << "               ('q' to return to the main )              \n";
    std::cout << "=========================================================\n";
    std::string line_input;

    while (std::cout << ">" << std::flush && std::getline(std::cin, line_input)) {
        if (line_input == "q") { break; }

        std::stringstream ss(line_input);
        std::string token;

        while (ss >> token) {
            std::vector<Grade>::const_iterator it = std::find_if(vec_gra.cbegin(), vec_gra.cend(), 
                                                [&token](const Grade& val) { return val.name == token; });

            if (it != vec_gra.cend())
            {
                std::cout << it->name << " scores " << it->scores << " points\n";
            }
            else{
                std::cout << "The name " << token << " is not found!\n";
            }
        }
        if (std::cin.eof()) { break; }
    }
}
                
void checkByGrade()
{
    std::cout << "=========================================================\n";
    std::cout << "            Please Input Scores you want to check        \n";
    std::cout << "               ('q' to return to the main )              \n";
    std::cout << "=========================================================\n";
    std::string line_input;

    while (std::cout << ">" << std::flush && std::getline(std::cin, line_input)) {
        if (line_input == "q") { break; }

        std::stringstream ss(line_input);
        std::string token;

        while (ss >> token) {
            try 
            {         
                std::size_t pos;
                int number = std::stoi(token, &pos);
                if (pos == token.size())
                {
                    bool isFound = false;   
                    for (const auto& i : vec_gra)
                    {
                        if (i.scores == number)
                        {
                            std::cout << i.name << " scores " << i.scores << " points\n";
                            isFound = true;
                        }                     
                    }

                    if (!isFound)
                    {
                        std::cout << "The score " << number << " is not found!\n";
                    }               
                }
                else
                {
                    std::cout << "[!] Format issue. Re-input.\n";
                    break;
                }                            
            } catch (const std::exception&) {
                std::cout << "[!] Format issue. Re-input.\n";
                break;
            }
        }
        if (std::cin.eof()) { break; }
    }
}

void gradeProcess(const std::string& line)
{
    std::stringstream ss(line);
    int choise;

    if (ss >> choise)
    {
        if (choise == 1) {
            gradeInput();
        }
        else if (choise == 2) {
            checkByName();
        }
        else if (choise == 3) {
            checkByGrade();
        }else {
            std::cout << "[!] Wrong number! Please input 1, 2 or 3 (Press q to quit).\n";          
        }
    }
    else
    {
        std::cout << "[!] Illegal format! Please input 1, 2 or 3 (Press q to quit).\n";          
    }
    
}

int main()
{
    std::cout << "=========================================================\n";
    std::cout << "                        Grade print                      \n";
    std::cout << "              1.Input Name and Score (Press 1)           \n";
    std::cout << "              2.Input Name to look up (Press 2)          \n";
    std::cout << "              3.Input Score and look up (Press 3)        \n";
    std::cout << "               ('q' to quit )                            \n";
    std::cout << "=========================================================\n";

    std::string line_input;

    while (std::cout << ">" << std::flush && std::getline(std::cin, line_input)) {
        if (line_input == "q") { break; }
        gradeProcess(line_input);
        if (std::cin.eof()) {  break; }
        std::cout << "=========================================================\n";
        std::cout << "                        Grade print                      \n";
        std::cout << "              1.Input Name and Score (Press 1)           \n";
        std::cout << "              2.Input Name to look up (Press 2)          \n";
        std::cout << "              3.Input Score and look up (Press 3)        \n";
        std::cout << "               ('q' to quit )                            \n";
        std::cout << "=========================================================\n";
    }
    std::cout << "Good Luck! Bye~\n";
    return 0;
}