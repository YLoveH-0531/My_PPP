#ifndef DRILL_POINTER_H
#define DRILL_POINTER_H

#include "std_lib_facilities.h"

struct Point {
    double x;
    double y;
};

extern vector<Point> original_points;
extern vector<Point> processed_points;

void read_points_with_cmd();
void write_points_with_cmd();
void read_points_with_file(vector<Point>& points, const std::string& filename);
void write_points_with_file(const vector<Point>& points, const std::string& filename);

#endif // DRILL_POINTER_H