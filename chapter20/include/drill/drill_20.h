#include <iostream>

namespace KaKaLot {


template<typename Iter1, typename Iter2>
// requires Input_iterator<Iter1>() && Output_iterator<Iter2>()
Iter2 copy(Iter1 f1, Iter1 e1, Iter2 f2)
{
    if (f1 == e1) { return f2; }
    while (f1 != e1) {
        *f2++ = *f1++;
    }
    return f2;
}

template<typename Iter1>
void print(Iter1 f1, Iter1 e1)
{
    for (auto it = f1; it != e1; ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}


}