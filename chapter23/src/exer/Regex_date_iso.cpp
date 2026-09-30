/**
 * @file Regex_date_iso.cpp
 * @author KaKaRot
 * @brief Based on Regex_date.cpp (exercise 6): find every date in the input
 *        file and rewrite it to ISO yyyy-mm-dd format, leaving everything
 *        else in the file unchanged.
 * @version 0.1
 * @date 2026-09-23
 *
 * @copyright Copyright (c) 2026
 *
 */

#include <string>
#include <iostream>
#include <fstream>
#include <regex>
#include <map>
#include <cctype>

#ifndef DATA_DIR
#error "DATA_DIR must be defined by CMake. Use cmake to build this project."
#endif

const std::string DATA = DATA_DIR;

// Sub-groups per alternative (regex is 1-indexed):
//   1 month 2 day   3 year   -- m/d/y  (e.g. 12/24/2000, US convention)
//   4 year  5 month 6 day    -- y/m/d  (e.g. 2000/12/24)
//   7 monthName 8 day 9 year -- Month d, y (e.g. December 24, 2000)
//   10 day 11 monthName 12 year -- d Month y (e.g. 24 December 2000)
//
// Numeric slash/dot/dash dates are inherently ambiguous (m/d/y vs d/m/y);
// this assumes the US m/d/y convention, matching how the exercise's own
// Regex_date.txt data reads (e.g. "12/24/2000" has no valid d/m/y reading).
std::regex re_date(
    R"((\d{1,2})[./-](\d{1,2})[./-](\d{2,4}))"
    R"(|(\d{4})[./-](\d{1,2})[./-](\d{1,2}))"
    R"(|(Jan|Feb|Mar|Apr|May|Jun|Jul|Aug|Sep|Oct|Nov|Dec)[a-z]*\.?\s+(\d{1,2}),?\s+(\d{2,4}))"
    R"(|(\d{1,2})\s+(Jan|Feb|Mar|Apr|May|Jun|Jul|Aug|Sep|Oct|Nov|Dec)[a-z]*\.?\s+(\d{2,4}))",
    std::regex::icase);

std::string month_to_num(std::string mon)
{
    static const std::map<std::string, std::string> table = {
        {"jan", "01"}, {"feb", "02"}, {"mar", "03"}, {"apr", "04"},
        {"may", "05"}, {"jun", "06"}, {"jul", "07"}, {"aug", "08"},
        {"sep", "09"}, {"oct", "10"}, {"nov", "11"}, {"dec", "12"}};
    for (auto& c : mon) c = std::tolower(static_cast<unsigned char>(c));
    return table.at(mon.substr(0, 3));
}

std::string pad2(const std::string& s)
{
    return s.size() < 2 ? "0" + s : s;
}

std::string norm_year(const std::string& y)
{
    if (y.size() == 4) return y;
    int yy = std::stoi(y);
    return (yy < 50 ? "20" : "19") + pad2(std::to_string(yy));
}

// Build the yyyy-mm-dd string for whichever alternative matched.
std::string to_iso(const std::smatch& m)
{
    if (m[1].matched)   // m/d/y
        return norm_year(m[3]) + "-" + pad2(m[1]) + "-" + pad2(m[2]);
    if (m[4].matched)   // y/m/d
        return norm_year(m[4]) + "-" + pad2(m[5]) + "-" + pad2(m[6]);
    if (m[7].matched)   // Month d, y
        return norm_year(m[9]) + "-" + month_to_num(m[7]) + "-" + pad2(m[8]);
    // d Month y
    return norm_year(m[12]) + "-" + month_to_num(m[11]) + "-" + pad2(m[10]);
}

// Replace every date found in line with its ISO form; copy everything else verbatim.
std::string reformat_line(const std::string& line)
{
    std::string out;
    auto begin = std::sregex_iterator(line.begin(), line.end(), re_date);
    auto end = std::sregex_iterator();
    std::size_t last = 0;
    for (auto it = begin; it != end; ++it) {
        out.append(line, last, it->position() - last);
        out += to_iso(*it);
        last = it->position() + it->length();
    }
    out.append(line, last, std::string::npos);
    return out;
}

int main()
{
    std::ifstream in(DATA + "Regex_date.txt");
    if (!in) {
        std::cerr << "can not open input file!" << std::endl;
        return 1;
    }

    std::ofstream out(DATA + "Regex_date_iso.txt");
    if (!out) {
        std::cerr << "can not open output file!" << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(in, line))
        out << reformat_line(line) << '\n';

    return 0;
}
