/*
    @copyright 2026 KaKaRot
    drills from chapter 4, drill 7-10 while lope, 
    operation: 
        1)  read one double data plus unit once loop and ignore whitespace between them.  @drill 7
        2)  reject values without units or with “illegal” representations of units.       @drill 8
        3)  keep track of the smallest, the largest, the number of values, and the sum of values(use meters)    @drill 9
        4)  Keep all the values entered (converted into meters) in a vector. At the end, sort them and write out those values.   @@drill 10-11
*/

#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <string>

double convert_fun(double len, const std::string& unit)
{
    if (unit == "cm") {
        return len * 0.01;    
    }else if (unit == "in") {
        return len * 0.0254;
    }else if (unit == "ft") {
        return len * 0.3048;
    }else if (unit == "m"){
        return len;
    }else{
        return -1.0;
    }    
}

int main()
{
    std::vector<double> values_in_meters;
    std::vector<std::string> str_vec;
    std::string strline;

    std::cout << "Please input the data(press quit to exit): ";
    
    while(std::getline(std::cin, strline))
    {
        std::stringstream string_input;
        double length;
        std::string word;        
        string_input.str(strline);

        if(string_input >> length >> word)
        {
            double meters = convert_fun(length, word);
            if (meters >= 0.0)
            {
                values_in_meters.emplace_back(meters);
                std::cout << "Please re-input the data(press quit to exit): ";
            }
            else{
                std::cout << "Please input right unit!" << std::endl;
            }      
        }
        else
        {  
            string_input.clear();
            std::string cmd;
            if (string_input >> cmd && cmd == "quit")
            {
                std::cout << "Good luck! Bye ~" << std::endl;
                break;
            }
            else
            {
                std::cout << "Please input right format! " << "Igored " << std::endl; 
            }            
        }
    }

    if (!values_in_meters.empty()) {
        std::stable_sort(values_in_meters.begin(), values_in_meters.end());
        std::cout << "The smaller is " << values_in_meters.front() << std::endl;
        std::cout << "The larger is " << values_in_meters.back() << std::endl;
        std::cout << "The count of the data is " << values_in_meters.size() << std::endl;
        double sum = std::accumulate(values_in_meters.cbegin(), values_in_meters.cend(), 0.0);
        std::cout << "The sum of the data is " << sum << std::endl;
        
        std::cout << "The value is :";
        for(const auto& i : values_in_meters)
        {
            std::cout << i << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}