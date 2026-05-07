/**
 * @file     Point_main.cpp
 * @author   KaKaRot
 * @date     29/4/2026
 * @brief    Point class to exercise input and output
 * @details  Point class to exercise input and output
 */
#ifndef DATA_DIR
#error "DATA_DIR must be defined by CMake. Use cmake to build this project."
#endif

#include "Point.h"

int main() {
    //test read and write points with command line
    read_points_with_cmd();
    write_points_with_cmd();

    // test read and write points with file
    write_points_with_file(original_points, DATA_DIR "MyData.txt");
    read_points_with_file(processed_points, DATA_DIR "MyData.txt");
    return 0;
}