/**
 * @file Mail_Program.cpp
 * @author KaKaRot
 * @brief 
 * @version 0.1
 * @date 2026-09-16
 * 
 * @copyright Copyright (c) 2026
 */

#include "Mail_Program.h"
#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>

Mail_file::Mail_file(const std::string& file) {
    std::ifstream is(file);

    if (!is) { std::cerr << "file laod failed..." << std::endl; exit(1); }
    
    std::string line;
    while (std::getline(is, line))
        lines.push_back(line);
    
    Line_Iter first = lines.begin();
    for (auto p = lines.begin(); p != lines.end(); ++p) {
        if (*p == "----") {
            letter.push_back(Message(first,p));
            first = p + 1;
        }
    }
}

int is_find(const std::string& str1, const std::string& str2){
    std::size_t n = str2.size();
    if (std::string(str1, 0, n) == str2) return n;
    return 0; 
}

bool find_by_address(const Message* m, std::string& s){    
    for (const auto& item : *m) {
        if (int n = is_find(item, "From:")) {
            s = std::string(item, n + 1);
            return true;
        }
    }
    return false;
}

std::string find_subject(const Message *m) {
    for (const auto &item : *m) {
        if (int n = is_find(item, "Subject:"))
            return std::string(item, n + 1);
    }
    return std::string();
}