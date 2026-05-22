/**
 * @file     exer_4_18.cpp
 * @author   KaKaRot
 * @date     28/3/2026
 * @brief    PPP Chapter 4 exercise 16.
 * @details  Solve quadratic equations.
 */

#include <cstddef>
#include <cmath>
#include <exception>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>

void process(const std::vector<double>& vec)
{
    double a = vec[0], b = vec[1], c = vec[2], x, x1, x2, derta;
    derta = b * b - 4 * a * c;

    if (a != 0) {
        if (derta > 0) {
            x1 = ( -b + std::sqrt(derta)) / (2 * a);
            x2 = ( -b - std::sqrt(derta)) / (2 * a);
            std::cout << "There result to the " << a << "x² + " << b << "x + " << c << " = 0 is " << x1 << " and " << x2 << " .\n";    
        }
        else if (derta == 0) {
            double x = -b / (2 * a);
            std::cout << "The equation has one real root: x = " << x << ".\n";        
        }
        else {
            std::cout << "There are no answer to the " << a << "x² + " << b << "x + " << c << " = 0.\n";
        }

    }
    else
    {
        if (b == 0) {
            std::cout << "Quadratic is " << c << " = 0. No result.\n";      
        }
        else {
            x = -(c / b);
            std::cout << "There result to the "  << b << "x + " << c << " = 0 is " << x << " .\n";
        }

    }

}

void compute(const std::string& line)
{
    std::stringstream ss(line);
    std::string token;
    std::vector<double> vec_double;
    while (ss >> token) {
        try {
            std::size_t pos;
            double number = std::stod(token, &pos);

            if (pos == token.size()) {
                vec_double.emplace_back(number);
            }
            else {
                std::cout << "[!] Please input right format...\n";
                return;
            }
        } catch (const std::exception&) {
            std::cout << "[!] Please input right format...\n";
            break;
        }

        if (vec_double.size() > 3) {
            std::cout << "[!] Please input right numbers...\n";
            break;
        }
    }

    if (vec_double.size() == 3) {
        process(vec_double);
    }
    else {
        std::cout << "[!] Please input right numbers...\n";   
    }
}

int main()
{
    std::cout << "================================================\n";
    std::cout << "                 Quadratic Math                 \n";
    std::cout << "      slove the quadratic  ax² + bx + c = 0     \n";
    std::cout << "         Enter a, b, c, ('q' to quit )          \n";
    std::cout << "================================================\n";

    std::string line_input;
    while (std::cout << ">" << std::flush && std::getline(std::cin, line_input)) {
        if (line_input == "q") { break; }
        compute(line_input);
        if(std::cin.eof()) { break; }
    }
    std::cout << "Good Luck! Bye!\n";
    return 0;
}
