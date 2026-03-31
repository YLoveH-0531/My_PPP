/**
 * @file     exer_5_4.cpp
 * @author   KaKaRot
 * @date     31/3/2026
 * @brief    PPP Chapter 5 exer 4.
 * @details  handle error in ctok
 */

#include "std_lib_facilities.h"
#include <stdexcept>

double ckoc(double k)       // converts Celsius to Kelvin
{
    if (k < 0)
        throw std::invalid_argument("ktoc: temperature can't be lower than 0 K");
    return  k - 273.15;
} 

int main()
{
    double c = 0.0;         // declare input variable
    cin >> c;               // retrieve temperature to input variable
    double k = ckoc(c);     // convert temperature
    cout << k << '\n' ;     // print out temperature
}