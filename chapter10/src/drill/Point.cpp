/**
 * @file     Point.cpp
 * @author   KaKaRot
 * @date     29/4/2026
 * @brief    Point class to exercise input and output
 * @details  Point class to exercise input and output
 */

#include "Point.h"
#include "std_lib_facilities.h"

vector<Point> original_points;

void read_points_with_cmd() {
    std::cout << "Enter points (x y):" << std::endl;
    Point p;
    while (true) {
        while (std::isspace(std::cin.peek()) && std::cin.peek() != '\n') {
            std::cin.ignore();
        }

        if (std::cin.peek() == '\n') {
            std::cin.ignore();
            break;
        }

        char c;
        double x, y;
        std::cin >> c;
        if (c != '(') {
            error("Expected '(' at the beginning of a point.");
            break;
        }
        if (!(std::cin >> x >> y)) {
            error("Invalid input format. Please enter points as (x y).");
            break;
        }

        std::cin >> c;
        if (c != ')') {
            error("Expected ')' at the end of a point.");
            break;
        }
        p.x = x;
        p.y = y;
        original_points.push_back(p);
    }
}

void write_points_with_cmd() {
    std::cout << "You entered the following points:" << std::endl;
    for (const auto& p : original_points) {
        std::cout << "(" << p.x << ", " << p.y << ")" << std::endl;
    }
}