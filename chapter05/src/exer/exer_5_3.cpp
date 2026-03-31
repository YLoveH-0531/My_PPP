/**
 * @file     exer_5_3.cpp
 * @author   KaKaRot
 * @date     31/3/2026
 * @brief    PPP Chapter 5 exer 3.
 * @details  find error
 */

#include "std_lib_facilities.h"

double ctok(double c)       // converts Celsius to Kelvin
{
    double k = c + 273.15;
    return k;
} 

int main()
{
    double c = 0.0;         // declare input variable
    cin >> c;               // retrieve temperature to input variable
    if (c < -271.3)
    {
        cout << "input the right value\n";            
    }
    else
    {
        double k = ctok(c);     // convert temperature
        cout << k << '\n' ;     // print out temperature
    }
}