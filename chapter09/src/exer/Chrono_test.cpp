/**
 * @file     Chrono_test.cpp
 * @author   KaKaRot
 * @date     23/4/2026
 * @brief    Test program for Chrono library
 * @details  Test for Chrono library
 */

 #include "Chrono.h"
 #include <iostream>

 int main()
 {
     using Chrono::Date;
     try {
         Date today {1978, Chrono::Month::jun, 25};
         Date tomorrow {today};
         tomorrow.add_day(1);
         std::cout << "today: " << today << std::endl;
         std::cout << "tomorrow: " << tomorrow << std::endl;
     }
     catch (Date::Invalid) {
         std::cerr << "Error: Invalid date\n";
         return 1;
     }
     return 0;
 }