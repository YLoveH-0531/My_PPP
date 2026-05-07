/**
 * @file     temp_stats.cpp
 * @author   KaKaRot
 * @date     7/5/2026
 * @brief    reads the data from raw_temps.txt
 * @details  reads the data from raw_temps.txt(exercise 10-3 10-4)
 */

#include "Temp.h"

void fill_with_vector(std::vector<Readings>& record, std::ifstream& infile, char terminator)
{
    while(true){
        int hour = 0;
        double temps = 0.0;
        for(; infile >> hour >> temps)
    }
}
