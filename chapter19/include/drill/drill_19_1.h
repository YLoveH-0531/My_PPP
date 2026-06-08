#ifndef DRILL_19_1_H
#define DRILL_19_1_H

#include <utility>

template <typename T>
struct S{
        S(T t);
        S(const S& s);
        S& operator=(const S& s);
        S(S&& s) noexcept; 
        S& operator=(S&& s) noexcept;
        T& get();
        const T& get() const;
    private:
        T val;
};

template<typename T>
S<T>::S(T t) : val(t)
{
}

template<typename T>
S<T>::S(const S& s) : val(s.val)
{
}

template<typename T>
S<T>& S<T>::operator=(const S& s)
{
    if (this == &s) {
        return *this;
    }
    val = s.val;
}

template<typename T>
S<T>::S(S&& s) noexcept : val(std::move(s.val))
{
}

template<typename T>
S<T>& S<T>::operator=(S&& s) noexcept
{
    if (this == &s) {
        return *this;
    }
    val = std::move(s.val);
}

template<typename T>
T& S<T>::get()
{
    return val;
}

template<typename T>
const T& S<T>::get() const
{
    return val;
}

#endif // DRILL_19_1_H