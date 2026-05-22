/**
 * @file     exer_5_7.cpp
 * @author   KaKaRot
 * @date     1/4/2026
 * @brief    PPP Chapter 5 exercise 7.
 * @details  Solve quadratic equations. same to chapter 4 exercise 18. this exercise will modify some details.
 */

#include <cstddef>
#include <cmath>
#include <exception>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>

std::string temStr(double n, const std::string& str)
{
    if (!str.empty() && n == 1)
    {
        return str;
    }
    
    std::ostringstream oss;
    oss << n << str;
    return oss.str();
}

std::string fomratEquation(double a, double b, double c)
{
    if (a == 0 && b == 0 && c==0)
    {
        return "0 = 0";
    }

    std::ostringstream oss;
    bool first = true;
    auto print_equ = [&](int n, const std::string& str){
        if (n == 0.0) { return; }

        if (first)
        {
            if (n < 0)
            {
                oss << "-";
            }
            oss << temStr(std::abs(n), str);
        }
        else
        {
            oss << (n > 0 ? " + " : " - ");
            oss << temStr(std::abs(n), str);
        }
        first = false;
    };

    print_equ(a, "x²");
    print_equ(b, "x");
    print_equ(c, "");

    oss << " = 0";
    return oss.str();
}

void process(const std::vector<double>& vec)
{
    if (vec.size() < 3) {   return;    }
    
    double a = vec[0], b = vec[1], c = vec[2], x, x1, x2, derta;
    derta = b * b - 4 * a * c;

    if (a != 0) {
        if (derta > 0) {
            x1 = ( -b + std::sqrt(derta)) / (2 * a);
            x2 = ( -b - std::sqrt(derta)) / (2 * a);
            std::cout << "There result to the "<< fomratEquation(a, b, c) <<" is " << x1 << " and " << x2 << " .\n";    
        }
        else if (derta == 0) {
            double x = -b / (2 * a);
            std::cout << "The equation has one real root: x = " << x << ".\n";        
        }
        else {
            std::cout << "There are no answer to the " <<fomratEquation(a, b, c) << "\n";
        }

    }
    else
    {
        if (b == 0) {
            std::cout << "Quadratic is " << c << " = 0. No result.\n";      
        }
        else {
            x = -(c / b);
            std::cout << "There result to the "  << fomratEquation(a, b, c) <<" is " << x << " .\n";
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
