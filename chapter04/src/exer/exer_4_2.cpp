/**
 * @file    exer_4_2.cpp
 * @author  KaKaRot
 * @date    2026
 * @brief   PPP Chapter 4 exercise 2.
 * @details  fix the program in §4.6.3 so that it always prints out a median.
 */

 #include <iostream>
 #include <sstream>
 #include <vector>
 #include <string>
 #include <algorithm>

int main()
{
    std::string input_line;

    std::cout << "Please input some datas: " << std::flush;
    while(std::getline(std::cin, input_line))
    {
        std::stringstream line(input_line);
        std::vector<double> double_vec;

        for(double data; line >> data;)
        {   
            double_vec.emplace_back(data);
        }

        if(double_vec.empty())
        {
            std::cout << "No data found!\n" ;
            std::cout << "Please re-input some datas: " << std::flush;
            continue;
        }

        std::sort(double_vec.begin(), double_vec.end());
        const unsigned int sz = double_vec.size();
        std::cout << "The Median is ";

        if(sz % 2)
        {
            std::cout << double_vec[sz/2.0] << std::endl;
        }
        else
        {
            std::cout << (double_vec[sz/2.0] + double_vec[sz/2 -1]) / 2.0 << std::endl;
        }
    
        std::cout << "Please re-input some datas: " << std::flush;
    }

    return 0;
}