/**
 * @file     temp_stats.cpp
 * @author   KaKaRot
 * @date     7/5/2026
 * @brief    reads the data from raw_temps.txt
 * @details  reads the data from raw_temps.txt(exercise 10-3 10-4)
 */

#include "Temp.h"
#include <stdexcept>

void fill_to_vector(std::vector<Readings>& record, std::ifstream& infile, char terminator)
{
    int hour = 0;
    double temps = 0.0;
    char unit;
    while(infile >> hour >> temps >> unit){
        
        if(unit != 'C' && unit != 'F'){
            infile.unget();   // maybe caller can use the character for something else
            infile.clear(std::ios::failbit); // set the bad bit to indicate an error
            return;
        }
        if(unit == 'C'){
            temps = temps * 9/5 + 32;     // convert Celsius to Fahrenheit
        }

        record.push_back({hour, temps});
    }

    if(infile.eof()) { return; }

    infile.clear();
    char ch;
    infile >> ch;
    if(ch != terminator){
        infile.unget();   // maybe caller can use the character for something else
        infile.clear(std::ios::failbit); // set the bad bit to indicate an error
    }
}
