/**
 * @file     exer_8_13.cpp
 * @author   KaKaRot
 * @date     21/4/2026
 * @brief    opration on vectors of strings.
 * @details  returns a vector<int> containing the number of characters in each string.
 *           find the longest and the shortest string and the lexicographically first and last string.
 */

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

struct Stats {
    std::string longest;
    std::string shortest;
    std::string lex_first;
    std::string lex_last;
};

std::vector<int> count_characters(const std::vector<std::string>& vec, Stats& stats) {
    std::vector<int> counts;
    for (const auto& s : vec) {
        counts.push_back(s.size());
    }

    if (!vec.empty()) {
        stats.longest = *std::max_element(vec.begin(), vec.end(), [](const std::string& a, const std::string& b) {
            return a.size() < b.size();
        });
        stats.shortest = *std::min_element(vec.begin(), vec.end(), [](const std::string& a, const std::string& b) {
            return a.size() < b.size();
        });
        stats.lex_first = *std::min_element(vec.begin(), vec.end());
        stats.lex_last = *std::max_element(vec.begin(), vec.end());
    }

    return counts;
}

int main()
{
    std::vector<std::string> vec;
    std::string word;
    std::cout << "Enter a list of strings:(Press q to finish)" << std::endl;
    std::string input_line;
    while (std::cout << ">"&& std::getline(std::cin, input_line)) {

        std::istringstream input_stream(input_line);

        if (input_line == "q") { break; }

        while (input_stream >> word) {
            vec.push_back(word);
        }
        Stats stats;
        std::vector<int> character_counts = count_characters(vec, stats);

        std::cout << "Character counts: ";
        for (const auto& count : character_counts) {
            std::cout << count << ' ';
        }
        std::cout << "\nLongest string: " << stats.longest
                  << "\nShortest string: " << stats.shortest
                  << "\nLexicographically first string: " << stats.lex_first
                  << "\nLexicographically last string: " << stats.lex_last << std::endl;

        if(std::cin.eof()) { break; }
    }
    return 0;
}