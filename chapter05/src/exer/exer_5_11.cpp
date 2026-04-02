/**
 * @file     exer_5_11.cpp
 * @author   KaKaRot
 * @date     2/4/2026
 * @brief    PPP Chapter 5 exer 11.
 * @details  Fibonacci series.
 */

#include <iostream>
#include <limits>

long long Fibonacci(int n)
{   
    if (n == 1)
    {
        return 1;
    }
    if (n == 0)
    {
        return 0;
    }
    
    return (Fibonacci(n-1) + Fibonacci(n-2));
}

int main()
{
    long long max_int = std::numeric_limits<int>::max();
    std::cout << "Game Start!\n";
    int index = 1;
    long long max_fib = 0;
    while (true)
    {
        long long fib = Fibonacci(index);
        if (fib > max_int) { break; }
        max_fib = fib;
        std::cout << max_fib << " " << std::flush;
        index++;
    }

    std::cout << "\nThe largest Fibonacci number that fits in an int is " << max_fib << '\n';
    return 0;
}