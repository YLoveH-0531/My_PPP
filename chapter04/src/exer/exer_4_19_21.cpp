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

std::vector<std::string> names;
std::vector<int> scores;

void gradeInput()
{
    std::cout << "=========================================================\n";
    std::cout << "       Please Input Name and Score (eg, Jack 80)         \n";
    std::cout << "               ('Input Noname 0 to quit )                \n";
    std::cout << "=========================================================\n";
    std::string line_input;

    while (std::cout << ">" << std::endl && std::getline(std::cin, line_input)) {
        if (line_input == "Noname 0") { break; }

        std::stringstream ss(line_input);
        std::string token;
        while (ss >> token) {
            try {
                std::size_t pos;
                int number = std::stoi(token, &pos);

            } catch (const std::exception&) {
                std::cout << "[!] Wrong choise input. Re-input.\n";
                break;
            }
        }
        if (std::cin.eof()) {  break; }
    }
}
                
void checkByName()
{

}
                
void checkByGrade()
{

}

void gradeProcess(const std::string& line)
{
    std::stringstream ss(line);
    std::string token;
    while (ss >> token) {
        try {
            std::size_t pos;
            int number = std::stoi(token, &pos);
            if (pos == token.size()) {
                if (number == 1) {
                    gradeInput();
                }
                else if (number == 2) {
                    checkByName();
                }
                else if (number == 3) {
                    checkByGrade();
                }else {
                    std::cout << "[!] Please input 1, 2 or 3 (Press q to quit).\n";          
                }
            }
            else {
                std::cout << "[!] Wrong choise input. Re-input.\n";
            }
        } catch (const std::exception&) {
            std::cout << "[!] Wrong choise input. Re-input.\n";
            break;
        }
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

    while (std::cout << ">" << std::endl && std::getline(std::cin, line_input)) {
        if (line_input == "q") { break; }
        gradeProcess(line_input);
        if (std::cin.eof()) {  break; }
    }
    std::cout << "Good Luck! Bye~\n";
    return 0;
}