/**
 * @file     exer_8_10_11.cpp
 * @author   KaKaRot
 * @date     21/4/2026
 * @brief    Operations on vectors: finding max, min, mean, and median.
 * @details  Find max and min elements in a vector. and calculate mean and the median of the elements in a vector.
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <numeric>

struct Stats {
    double max;
    double min;
    double mean;
    double median;
};

Stats calculate_stats(const std::vector<double>& vec) {
    Stats stats;
    if (vec.empty()) {
        stats.max = stats.min = stats.mean = stats.median = 0.0;
        return stats;
    }

    stats.max = *std::max_element(vec.begin(), vec.end());
    stats.min = *std::min_element(vec.begin(), vec.end());
    stats.mean = std::accumulate(vec.begin(), vec.end(), 0.0) / vec.size();

    std::vector<double> sorted_vec = vec;
    std::sort(sorted_vec.begin(), sorted_vec.end());
    if (sorted_vec.size() % 2 == 0) {
        stats.median = (sorted_vec[sorted_vec.size() / 2 - 1] + sorted_vec[sorted_vec.size() / 2]) / 2.0;
    } else {
        stats.median = sorted_vec[sorted_vec.size() / 2];
    }

    return stats;
}

int main()
{
        std::vector<double> vec;
    
        double number;
    
        std::cout << "Enter a list of numbers:(Press q to finish)" << std::endl;
        std::string input_line;
        while (std::cout << ">"&& std::getline(std::cin, input_line)) {

            std::istringstream input_stream(input_line);

            if (input_line == "q") {
                break;
            }

            while (input_stream >> number) {
                vec.push_back(number);
            }
            Stats stats = calculate_stats(vec);
            std::cout << "Max: " << stats.max << ", Min: " << stats.min 
                      << ", Mean: " << stats.mean << ", Median: " << stats.median << std::endl;

            if(std::cin.eof()) {
                break;
            }
        }
    return 0;
}