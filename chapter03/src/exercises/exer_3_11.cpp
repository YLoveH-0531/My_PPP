/*
    @copyright 2026 KaKaRot
    exercises from chapter 3
    exercise 11 count the coins
*/

#include "std_lib_facilities.h"

int main()
{
    int  pennies, nickels, dimes, quarters, half_dollars, one_dollar;
    double sum;
    std::cout << "Please input the numbers of pennies, nickels, dimes, quarters, half_dollars and one_dollar you have." << std::endl;
    std::cin >>  pennies >> nickels >> dimes >> quarters >> half_dollars >> one_dollar;
    sum = pennies + 5*nickels + 10*dimes + 25*quarters +  50*half_dollars + 100*one_dollar;

    std::cout << "You have " << pennies << ((pennies > 1) ? " pennies." : " penny") << std::endl; 
    std::cout << "You have " << nickels << " nickel" << ((nickels > 1)? "s." : ".") << std::endl;
    std::cout << "You have " << dimes << " dime"<< ((dimes > 1)? "s." : ".") << std::endl;
    std::cout << "You have " << quarters << " quarter" << ((quarters > 1)? "s." : ".") << std::endl;
    std::cout << "You have " << half_dollars << " half dollar" << ((half_dollars > 1)? "s." : ".")<< std::endl;
    std::cout << "You have " << one_dollar << " one dollar" << ((one_dollar > 1)? "s." : ".") << std::endl;
    std::cout << "The value of all of your coins is "
              << pennies + 5*nickels + 10*dimes + 25*quarters +  50*half_dollars + 100*one_dollar
              <<" cents or " << sum/100 << " dollars."
              << std::endl;

    return 0;
}