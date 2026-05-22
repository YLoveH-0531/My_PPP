/**
 * @file     store_temps.cpp
 * @author   KaKaRot
 * @date     7/5/2026
 * @brief    creates a file of data in Reading type and fill the file 
 * @details  file name: raw_temps.txt.(exercise 10-2 10-4)
 */

#include "Temp.h"

void fill_from_vector(const std::vector<Readings>& record, std::ofstream& outfile, char terminator)
{
    for(const auto& r : record){
        outfile << r.hour << ' ' << r.temperature << 'C' << '\n';
    }
    outfile << terminator << '\n';
}
