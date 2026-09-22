/**
 * @file Mail_main.cpp
 * @author KaKaRot
 * @brief 
 * @version 0.1
 * @date 2026-09-16
 * 
 * @copyright Copyright (c) 2026
 */

#include "Mail_Program.h"
#include <iostream>
#include <map>
#include <utility>

#ifndef DATA_DIR
#error "DATA_DIR must be defined by CMake. Use cmake to build this project."
#endif

const std::string DATA = DATA_DIR;
typedef std::multimap<std::string, const Message*>::iterator map_it;

int main()
{
    std::string filename(DATA + "Mail_data.txt");
    Mail_file mail(filename);
    
    std::multimap<std::string, const Message*> mail_map;
    for (const auto& item : mail) {
        std::string key;
        if(find_by_address(&item, key)) mail_map.insert(std::make_pair(key, &item));
    }

    auto pp = mail_map.equal_range("John Doe <jdoe@machine.example>");
    for (map_it p = pp.first; p != pp.second; ++p) {
        //std::cout << find_subject(p->second) << "\n";
        std::cout << find_subject_regex(p->second) << "\n";
    }
    
    // output subject
    std::cout << "input the subject:";
    std::string search;
    while (std::getline(std::cin, search)){
        auto pp = mail.sub_map.equal_range(search);
        if (pp.first == pp.second) {
        std::cout << "Not found!" << std::endl;
            std::cout << "\ninput the subject:";
            continue;
        }
        std::cout << "find " << search << ": " << std::endl;
        int mnu = 1;
        for(auto p = pp.first; p != pp.second; ++p) {
            std::cout << "  Message " << mnu << std::endl;
            for (const auto& line : *(p->second)) {
                std::cout << "    " << line << std::endl;
            }
            ++mnu;
        }
        std::cout << "\ninput the subject:";
    }

    return 0;
}