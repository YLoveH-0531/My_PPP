#ifndef EXER_19_1_H
#define EXER_19_1_H

#include <vector>

template <typename T>
void add(std::vector<T>& v1, const std::vector<T>& v2)
{
    std::size_t count = (v1.size() < v2.size()) ? v1.size() : v2.size();
    for (std::size_t i = 0; i < count; i++) {
        v1[i] = v2[i]; 
    }
}

template <typename T, typename U>
T inner_product(const std::vector<T>& vt, const std::vector<U>& vu)
{
    T sum{};   // fix: 原来 T sum 对基本类型是垃圾值，需值初始化为 0
    std::size_t count = (vt.size() < vu.size()) ? vt.size() : vu.size();
    for (std::size_t i = 0; i < count; i++) {
        sum += vt[i] * vu[i];
    }
    return sum;
}

#endif // EXER_19_1_H