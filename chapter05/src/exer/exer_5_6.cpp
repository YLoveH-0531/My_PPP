/**
 * @file     exer_5_6.cpp
 * @author   KaKaRot
 * @date     31/3/2026
 * @brief    PPP Chapter 5 exer 6.
 * @details  Converts from Celsius to Fahrenheit
 */

#include "std_lib_facilities.h"
#include <stdexcept>

double ccof(double c)       // Converts from Celsius to Fahrenheit
{
    if (c < -273.15)
        throw std::invalid_argument("ccof: temperature can't be lower than 0 K");
    return  c * 1.8 + 32;
} 

int main()
{
    double c = 0.0;         // declare input variable
    cin >> c;               // retrieve temperature to input variable
    double k = ccof(c);     // convert temperature
    cout << k << '\n' ;     // print out temperature
}