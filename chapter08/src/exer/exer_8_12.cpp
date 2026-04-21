/**
 * @file     exer_8_12.cpp
 * @author   KaKaRot
 * @date     21/4/2026
 * @brief    Improve print_until_s() and create a new function print_until_ss().
 * @details  Improve the functionality of print_until_s().
 *           Create a new function print_until_ss() that prints elements until a specific string is encountered.
 */

#include <iostream>
#include <vector>
#include <string>

void print_until_s(const std::vector<std::string>& v, const std::string& quit) {
    for(const auto& s : v) {
        if (s == quit) return;
        std::cout << s << '\n';
    }
}

void print_until_ss(const std::vector<std::string>& v, const std::string& quit) {

    bool first_occurrence = true;
    for(const auto& s : v) {
        if (s == quit) {
            if (first_occurrence) {
                first_occurrence = false;
            } else {
                return;
            }
        }
        std::cout << s << '\n';
    }
}

int main()
{
    std::vector<std::string> vec = {"Hello", "World", "C++", "Programming", "Hello", "C++"};
    for(const auto& s : vec) {
        std::cout << s << ' ';
    }
    std::cout << "\n\n";
    std::cout << "Print until 'C++':" << std::endl;
    print_until_s(vec, "C++");
    std::cout << "\nPrint until 'Hello' (second occurrence):" << std::endl;
    print_until_ss(vec, "Hello");
    return 0;
}