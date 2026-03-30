/**
 * @file     exer_4_3.cpp
 * @author   KaKaRot
 * @date     2026
 * @brief    PPP Chapter 4 exercise 3.
 * @details  Read a seqsence of double input and output the smaller, larger, toal and mean of the seqsence
 */

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>


int main()
{
    std::string line;

    std::cout << "Please input the distance of cities: " << std::flush;
    while(std::getline(std::cin, line))
    {
        std::stringstream ss(line);
        std::vector<double> vec;

        for(double data; ss >> data;)
        {
            vec.emplace_back(data);
        }

        if(vec.empty())
        {
            std::cout << "Illegal input! Ignored~ ";
            std::cout << "\nPlease re-input the right format : " << std::flush;
            continue; 
        }

        std::sort(vec.begin(), vec.end());
        double sum  = std::accumulate(vec.cbegin(), vec.cend(), 0.0);

        std::cout << "The smaller is : " << vec.front();
        std::cout << "\nThe larger is : " << vec.back();
        std::cout << "\nThe sum is : " << sum;
        std::cout << "\nThe mean is : " << sum / vec.size();
        std::cout << "\nPlease re-input the distance of cities: " << std::flush;
    }
    return 0;
}