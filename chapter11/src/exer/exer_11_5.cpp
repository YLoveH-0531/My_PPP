/**
 * @file exer_11_5.cpp
 * @author KaKaRot
 * @brief reads strings and for each string outputs the character classification of each character
 * @version 0.1
 * @date 2026-05-17
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include <string>
#include <iostream>
#include <cctype>

/**
 * @brief judge if a char belongs to one certain class
 * 
 */
void judge_char()
{
    while (true) {
        std::string str;
        if (std::cin >> str) {

            std::cout << "\nString: \"" << str << "\"\n";
            for (const auto& ch : str) {
                std::cout << " '" << ch << "': ";
                    
                unsigned char uc = static_cast<unsigned char>(ch);
                
                if (std::isalpha(uc))  std::cout << "alpha ";
                if (std::isdigit(uc))  std::cout << "digit ";
                if (std::isalnum(uc))  std::cout << "alnum ";
                if (std::isspace(uc))  std::cout << "space ";
                if (std::islower(uc))  std::cout << "lower ";
                if (std::isupper(uc))  std::cout << "upper ";
                if (std::ispunct(uc))  std::cout << "punct ";
                if (std::isprint(uc))  std::cout << "print ";
                if (std::isgraph(uc))  std::cout << "graph ";
                if (std::iscntrl(uc))  std::cout << "cntrl ";
                if (std::isxdigit(uc))  std::cout << "xdigit ";
                std::cout << '\n';
            }
        }else {
            std::cout << "Something error!!!";
            return;
        }
    }
}

int main()
{
    judge_char();
    return 0;
}