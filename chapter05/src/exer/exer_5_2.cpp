/**
 * @file     exer_5_2.cpp
 * @author   KaKaRot
 * @date     31/3/2026
 * @brief    PPP Chapter 5 exer 2.
 * @details  correct error
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
    double k = ctok(c);     // convert temperature
    cout << k << '\n' ;     // print out temperature
}