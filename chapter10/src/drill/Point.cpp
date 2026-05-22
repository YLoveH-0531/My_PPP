/**
 * @file     Point.cpp
 * @author   KaKaRot
 * @date     29/4/2026
 * @brief    Point class to exercise input and output
 * @details  Point class to exercise input and output
 */

#include "Point.h"
#include <fstream>

vector<Point> original_points;
vector<Point> processed_points;

void read_points_with_cmd() {
    std::cout << "Enter points (x y):" << std::endl;
    while (true) {

        char ch;
        while(std::cin.get(ch) && std::isspace(ch) && ch != '\n') ;

        if (!std::cin || ch == '\n') { break; } // End of input

        double x, y;
        if (ch != '(') {
            error("Expected '(' at the beginning of a point.");
            break;
        }
        if (!(std::cin >> x >> y)) {
            error("Invalid input format. Please enter points as (x y).");
            break;
        }

        while(std::cin.get(ch) && std::isspace(ch) && ch != '\n') ;
        if (!std::cin || ch == '\n') { 
            error("Unexpected end of input.");
            break; 
        } // End of input

        if ( ch != ')') {
            error("Expected ')' at the end of a point.");
            break;
        }
        original_points.push_back({x,y});
    }
}

void write_points_with_cmd() {
    std::cout << "You entered the following points:" << std::endl;
    for (const auto& p : original_points) {
        std::cout << "(" << p.x << ", " << p.y << ")" << std::endl;
    }
}

void read_points_with_file(vector<Point>& points, const std::string& filename) {
    std::ifstream infile(filename);
    if (!infile) {
        error("Could not open file: " + filename);
        return;
    }
    while (true){
        char ch;
        while(infile.get(ch) && std::isspace(ch));
        if (!infile) { break; } // End of file
        if (ch != '(') {
            error("Expected '(' at the beginning of a point.");
            break;
        }
        double x, y;
        if (!(infile >> x >> y)) {
            error("Invalid input format. Please enter points as (x y).");
            break;
        }
        while(infile.get(ch) && std::isspace(ch));
        if (!infile) {
            error("Unexpected end of file.");
            break; 
        } // End of file
        if (ch != ')') {
            error("Expected ')' at the end of a point.");
            break;  
        }
        points.push_back({x,y});
    }
}

void write_points_with_file(const vector<Point>& points, const std::string& filename) {
    std::ofstream outfile(filename, std::ios::app);
    if (!outfile) {
        error("Could not open file: " + filename);
        return;
    }
    for (const auto& p : points) {
        outfile << "(" << p.x << " " << p.y << ")" << '\n';
    }
}