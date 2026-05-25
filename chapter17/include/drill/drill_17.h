#ifndef DRILL_17_H
#define DRILL_17_H
#include <iostream>
#include <vector>

std::ostream& print_array(std::ostream& os, int* p, int n)
{
    for (int i = 0; i < n; ++i)
    {
        os << p[i] << " ";
    }
    return os;
}

void array_fun()
{
    int* p10 = new int[10];
    for (int i = 0; i < 10; ++i)
    {
        p10[i] = 100 + i;
    }

    int* p11 = new int[11];
    for (int i = 0; i < 11; ++i)
    {
        p11[i] = 100 + i;
    }

    int* p20 = new int[20];
    for (int i = 0; i < 20; ++i)
    {
        p20[i] = 100 + i;
    }

    print_array(std::cout, p10, 10);
    std::cout << '\n';
    print_array(std::cout, p11, 11);
    std::cout << '\n';
    print_array(std::cout, p20, 20);
    std::cout << '\n';
    delete[] p10;
    delete[] p11;
    delete[] p20;
}

void print_vector(const std::vector<int>& v)
{
    for (const auto& elem : v)
    {
        std::cout << elem << " ";
    }
    std::cout << '\n';
}

void vector_fun()
{
    std::vector<int> v10(10);
    for (int i = 0; i < 10; ++i)
    {
        v10[i] = 100 + i;
    }
    print_vector(v10);

    std::vector<int> v11(11);
    for (int i = 0; i < 11; ++i)
    {
        v11[i] = 100 + i;
    }
    print_vector(v11);

    std::vector<int> v20(20);
    for (int i = 0; i < 20; ++i)
    {
        v20[i] = 100 + i;
    }
    print_vector(v20);
}

#endif // DRILL_17_H