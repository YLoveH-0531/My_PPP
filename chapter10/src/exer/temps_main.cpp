/**
 * @file     temps_main.cpp
 * @author   KaKaRot
 * @date     8/5/2026
 * @brief    main function for the temperature statistics program
 * @details  tests the functions defined in Temp.h and implemented in temp_stats.cpp and store_temps.cpp
 */

#ifndef DATA_DIR
#error "DATA_DIR must be defined by CMake. Use cmake to build this project."
#endif

#include "Temp.h"
#include <iostream>

const std::string DATA = DATA_DIR;

int main()
{
    char terminator = '|';
    std::vector<Readings> record = {
        {0, 32.0},
        {1, 31.5},
        {2, 30.0},
        {3, 29.5},
        {4, 28.0},
        {5, 27.5},
        {6, 26.0},
        {7, 25.5},
        {8, 24.0},
        {9, 23.5},
        {10, 22.0}, 
        {11, 21.5},
        {12, 20.0},
        {13, 19.5},
        {14, 18.0},
        {15, 17.5},
        {16, 16.0},
        {17, 15.5},
        {18, 14.0},
        {19, 13.5},
        {20, 12.0},
        {21, 11.5},
        {22, 10.0},
        {23, 9.5}
    };
    std::ofstream outfile(DATA + "raw_temps.txt", std::ios::app);
    if(!outfile){
        std::cerr << "Could not create the file!" << std::endl;
        return 1;
    }

    fill_from_vector(record, outfile, terminator);
    outfile.close();

    std::ifstream infile(DATA + "raw_temps.txt");
    if(!infile){
        std::cerr << "Could not open the file!" << std::endl;
        return 1;
    }

    infile.exceptions(infile.exceptions() | std::ifstream::badbit);

    std::vector<Readings> new_record;
    fill_to_vector(new_record, infile, terminator);
    if(infile.fail() && !infile.eof()){
        std::cerr << "Error reading the file!" << std::endl;
        return 1;
    }
    infile.close();

    for(const auto& r : new_record){
        std::cout << "Hour: " << r.hour << ", Temperature: " << r.temperature << '\n';
    }   

    return 0;
}