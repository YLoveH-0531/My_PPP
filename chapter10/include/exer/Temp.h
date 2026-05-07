#ifndef TEMP_H
#define TEMP_H

#include <vector>
#include <iostream>
#include <fstream>
#include <string>

struct Readings{
    int hour; // hour after midnight [0:23]
    double temperature; // in Fahrenheit
};

void fill_with_vector(std::vector<Readings>& record, std::ifstream& infile, char terminator);

#endif // TEMP_H