/**
 * @file exer_11_9.cpp
 * @author KaKaRot
 * @brief  Write a program that reads a sequence of integers from a text file, 
 *         stores them in a vector, and then writes them to a binary file. 
 *         Then write another program that reads the binary file and writes the integers to another text file.
 * @version 0.1
 * @date 2026-05-19
 * 
 * @copyright Copyright (c) 2026
 */

#include <fstream>
#include <iostream>
#include <vector>
#include <string>

#ifndef DATA_DIR
#error "DATA_DIR must be defined by CMake. Use cmake to build this project."
#endif

const std::string DATA = DATA_DIR;

template<typename T>
char* as_int(T& value)
{
    void* p = &value;
    return static_cast<char*>(p);
}

void from_txt_to_bin(const std::string& filename)
{
    std::cout << "Converting from text to binary...\n";
    std::ifstream txt_file(filename);
    if (!txt_file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::ofstream bin_file(DATA + "exer_11_9_output.bin", std::ios::binary);
    if (!bin_file) {
        std::cerr << "Error opening file for writing: " << DATA + "exer_11_9_output.bin" << std::endl;
        return;
    }

    std::vector<int> numbers;
    for (int n; txt_file >> n;) {
        numbers.push_back(n);
    }

    for (auto& n : numbers) {
        bin_file.write(as_int(n), sizeof(int));
    }
    std::cout << "Converting from text to binary down...\n";
}

void from_bin_to_txt(const std::string& filename)
{

    std::cout << "Converting from binary to text...\n";
    std::ifstream bin_file(filename, std::ios::binary);
    if (!bin_file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::ofstream txt_file(DATA + "exer_11_9_output.txt");
    if (!txt_file) {
        std::cerr << "Error opening file for writing: " << DATA + "exer_11_9_output.txt" << std::endl;
        return;
    }

    int n;
    while (bin_file.read(as_int(n), sizeof(int))) {
        txt_file << n << ' ';
    }
    std::cout << "Converting from binary to text down...\n";
}

int main()
{
    std::string txt_filename{DATA + "exer_11_9_input.txt"};
    std::string bin_filename{DATA + "exer_11_9_output.bin"};
    from_txt_to_bin(txt_filename);
    from_bin_to_txt(bin_filename);
    return 0;
}