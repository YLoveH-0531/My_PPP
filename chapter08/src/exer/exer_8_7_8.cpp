/**
 * @file     exer_8_7_8.cpp
 * @author   KaKaRot
 * @date     21/4/2026
 * @brief    Read names and ages and input in pairs.
 * @details  Read a set of names first and then ages into two separate vectors. 
 *           Then, sort the name vector and print each name with the corresponding age.
 */

 #include <iostream>
 #include <vector>
 #include <algorithm>
 #include <string>
 #include <sstream>

 int main()
 {
        std::vector<std::string> names;
        std::vector<double> ages;
    
        std::string name;
        double age;
    
        std::cout << "Enter names first:(Press q to finish)" << std::endl;
        std::string input_line;
        while (std::cout << "Name: "&& std::getline(std::cin, input_line)) {

            std::istringstream input_stream(input_line);

            if (input_line == "q") {
                break;
            }

            while (input_stream >> name) {
                names.push_back(name);
            }
        }

        std::cout << "Enter ages with same count as names:(Press q to finish)" << std::endl;
        while (std::cout << "Age: "&& std::getline(std::cin, input_line)) {

            std::istringstream input_stream(input_line);
            
            if (input_line == "q") {
                break;
            }
            while (input_stream >> age) {
                ages.push_back(age);
            }
        }

        if (names.size() != ages.size()) {
            std::cerr << "Error: The number of names and ages must be the same." << std::endl;
            return 1;
        }

        std::vector<std::string> sorted_names = names;
        std::vector<double> sorted_ages(ages.size());
        std::sort(sorted_names.begin(), sorted_names.end());
        for (size_t i = 0; i < sorted_names.size(); i++)
        {
            auto it = std::find(names.begin(), names.end(), sorted_names[i]);
            if (it != names.end()) {
                size_t index = std::distance(names.begin(), it);
                sorted_ages[i] = ages[index];
            }
        }
        
        std::cout << "Sorted names and corresponding ages:" << std::endl;
        for (size_t i = 0; i < sorted_names.size(); i++) {
            std::cout << sorted_names[i] << ": " << sorted_ages[i] << std::endl;
        }

    return 0;
 }