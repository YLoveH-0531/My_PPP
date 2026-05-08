#ifndef TEMP_H
#define TEMP_H

#include <vector>
#include <fstream>

struct Readings{
    int hour; // hour after midnight [0:23]
    double temperature; // in Fahrenheit
};

void fill_to_vector(std::vector<Readings>& record, std::ifstream& infile, char terminator);
void fill_from_vector(const std::vector<Readings>& record, std::ofstream& outfile, char terminator);

#endif // TEMP_H