/**
 * @file     Point.cpp
 * @author   KaKaRot
 * @date     29/4/2026
 * @brief    Point class to exercise input and output
 * @details  Point class to exercise input and output
 */

#include "Pointer.h"
#include "std_lib_facilities.h"

vector<Point> original_points;

void read_points_with_cmd() {
    std::cout << "Enter points (x y):" << std::endl;
    Point p;
    while (true) {
        char c = std::cin.peek();
        double x, y;
        if (c != '(') {
            error("Expected '(' at the beginning of a point.");
            break;
        }
        std::cin.get(); // consume '('
        if (!(std::cin >> x >> y)) {
            error("Invalid input format. Please enter points as (x y).");
            break;
        }

        c = std::cin.peek();
        if (c != ')') {
            error("Expected ')' at the end of a point.");
            break;
        }
        std::cin.get(); // consume ')'
        p.x = x;
        p.y = y;
        original_points.push_back(p);
        if (std::cin.peek() == '\n') {
            break; // stop reading if the user presses Enter
        }
    }
}

void write_points_with_cmd() {
    std::cout << "You entered the following points:" << std::endl;
    for (const auto& p : original_points) {
        std::cout << "(" << p.x << ", " << p.y << ")" << std::endl;
    }
}