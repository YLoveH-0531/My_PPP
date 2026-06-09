#ifndef DRILL_19_1_H
#define DRILL_19_1_H

#include <cctype>
#include <utility>
#include <iostream>
#include <vector>

template <typename T>
struct S{
        S(T t);
        S(const S& s);
        S& operator=(const S& s);
        S(S&& s) noexcept; 
        S& operator=(S&& s) noexcept;
        T& get();
        const T& get() const;
        void read_val(std::istream& is);
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
    return *this;   // TODO: 原来漏了 return *this，赋值运算符必须返回 *this
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
    return *this;   // TODO: 原来漏了 return *this，移动赋值运算符也必须返回 *this
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

template<typename T>
void S<T>::read_val(std::istream& is)
{
   is >> val;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{
    os<< '{';
    size_t count = 0;   // TODO: 原来写 size_t 不是 vector 的成员类型，应是 size_type 或直接用 size_t
    for (const auto& i : v) {
        os << i;
        if (++count < v.size()) {
            os << ", ";
        }
    }
    os << '}';
    return os;
}

template<typename T>
std::istream& operator>>(std::istream& is, std::vector<T>& v)
{
    std::vector<T> v_tem;
    T val;
    char ch;
    if (!(is >> ch) || ch != '{') {
        is.setstate(std::ios::failbit);
        return is;
    }

    if (is >> ch && ch == '}' ) {
        v = v_tem;
        return is;
    }
    is.putback(ch);
    
    while (is >> val) {
        v_tem.push_back(val);
        if (!(is >> ch)) { break; }
        if (ch == '}') { 
            v = v_tem;  
            return is; 
        }
        if (ch != ',') { break; }
    }
    is.setstate(std::ios::failbit);
    return is;
}

#endif // DRILL_19_1_H