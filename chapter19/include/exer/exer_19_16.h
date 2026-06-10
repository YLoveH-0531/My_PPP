#ifndef EXER_19_16_H
#define EXER_19_16_H

#include <cstddef>

template <typename T>
class vector_s
{
    struct S{
        T* elem;
        size_t size;
        size_t cap;
    };
    private:
        S* s;
};


#endif //EXER_19_16_H