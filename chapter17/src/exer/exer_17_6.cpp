/**
 * @file exer_17_6.cpp
 * @author KaKaRot
 * @brief 
 * @version 0.1
 * @date 2026-05-26
 * 
 * @copyright Copyright (c) 2026
 */

#include <iostream>
#include <new>

void memory_leak()
{
    char* p = new  char[1024 * 1024];
    p[0] = 'A';
    std::cout << "Allocated 1 MB of memory." << *p << std::endl;
}

int main()
{
    unsigned long count = 0;
    try {
        while (true){
            memory_leak();
            std::cout << "Memory leak " <<  ++count << " MBs" << std::endl;
        }
    }catch (const std::bad_alloc& e){
        std::cerr << "Memory allocation failed: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
