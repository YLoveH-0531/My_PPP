/**
 * @file     exer_10_9.cpp
 * @author   KaKaRot
 * @date     11/5/2026
 * @brief    merge two files with order.
 * @details  Write a program that takes two files containing sorted 
             whitespace-separated words and merges them, preserving order.
 */

#ifndef DATA_DIR
#error "DATA_DIR must be defined by CMake. Use cmake to build this project."
#endif

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

const std::string DATA = DATA_DIR;

void merge(const std::string& file1, const std::string& file2, const std::string& out)
{
    std::cout << "Merge  start...\n";

    std::ifstream ifs1(DATA + file1);
    std::ifstream ifs2(DATA + file2);
    std::ofstream ofs(DATA + out);

    if (!ifs1 || !ifs2) { throw std::runtime_error("can not open file"); }
    if (!ofs) { throw std::runtime_error("can not create file"); }

    std::string s1, s2;
    if (!(ifs1 >> s1)) { ofs << ifs2.rdbuf(); return; }
    if (!(ifs2 >> s2)) { ofs << s1 << '\n'; ofs << ifs1.rdbuf(); return; }

    while(true){
        if (s1 < s2) {
            ofs << s1 << '\n';
            ifs1 >> s1;
        }else{
            ofs << s2 << '\n';
            ifs2 >> s2;
        }
        
        if( !ifs1 || !ifs2) { break; }
    }

    if(ifs1) {
        ofs << s1 << '\n';
        ofs << ifs1.rdbuf(); 
    }else if(ifs2) {
        ofs << s2 << '\n';
        ofs << ifs2.rdbuf(); 
    }

    ofs.close();

    std::ifstream ifs3(DATA + out);
    std::cout << ifs3.rdbuf();

    std::cout << "Merge  finish...\n";
}

int main()
{
    std::string file1{"exer_10_9_file1.txt"}, file2{"exer_10_9_file2.txt"}, file3{"exer_10_9_out.txt"};
    merge(file1, file2, file3);
    return 0;
}