/** 
 * @file     exer_11_1.cpp
 * @author   KaKaRot
 * @date     14/5/2026
 * @brief    reads a text file and converts all characters to lowercase, writing the result to another file.
 * @details  reads a text file and converts all characters to lowercase, writing the result to another file. The program should then read the new file and print its contents to the console.
 */

#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include <stdexcept>

#ifndef DATA_DIR
#error "DATA_DIR must be defined by CMake. Use cmake to build this project."
#endif

const std::string DATA = DATA_DIR;

/** 
 * @brief    Converts the contents of one file to lowercase and writes it to another file, then prints the new file's contents. --- IGNORE ---
 * @param    in The name of the input file. --- IGNORE ---
 * @param    out The name of the output file. --- IGNORE ---
 */
void convert(const std::string& in, const std::string& out)
{
    std::cout << "start converting..." << '\n';
    std::ifstream ifs(DATA + in);
    std::ofstream ofs(DATA + out);
    if(!ifs || !ofs) {
        throw std::runtime_error("open/create file failed!");
    }

    std::string line;
    while(std::getline(ifs, line)){
        for(auto& ch : line){
            ch = std::tolower(static_cast<unsigned char>(ch));
        }
        ofs << line << '\n';
    }

    ofs.close();
    std::ifstream ifs_check(DATA + out);

    std::cout << ifs_check.rdbuf();
    std::cout << "finish converting..." << '\n';
}

int main()
{
    try {
        std::string file1{"exer_11_1_input.txt"}, file2{"exer_11_1_output.txt"};
        convert(file1, file2);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
    return 0;
}