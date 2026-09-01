#ifndef LEARN_AND_PRACTICE_H
#define LEARN_AND_PRACTICE_H
#include <numeric>
#include <vector>

template<typename It, typename V>
It find(It first, It last, V value){
    while (first != last && *first!=value) ++first;
    return first; 
}

template<typename It, typename Pred>
It find_if(It first, It last, Pred pred){
    while (first != last && !pred(*first)) ++first;
    return first; 
}

template<typename type>
void f(std::vector<type>& v, int x)
{
    auto fu = find_if(v.begin(), v.end(), [x](int a) { return a > x; });
    if (fu != v.end()) {
        // do something
    }
}

template<typename In, typename T>
T accumulate(In first, In last, T value)
{
    while (first != last) {
        value = value + *first;
        ++first;
    }
    return value;
}

#endif // LEARN_AND_PRACTICE_H