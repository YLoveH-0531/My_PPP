#ifndef DRILL_19_1_H
#define DRILL_19_1_H

#include <cctype>
#include <utility>
#include <iostream>
#include <vector>

// operator<< / operator>> 必须在 S<T> 之前定义
// 原因：S<T>::read_val 调用 is >> val（val 是 T 类型）
// 两阶段名称查找：第一阶段（定义时）做普通查找，必须在此时就能看到 operator>>
// 否则第二阶段只做 ADL（只查 std 命名空间），全局命名空间的 operator>> 找不到
// GCC 宽松（实例化时也做完整查找），Clang/严格模式下会报错 —— 移到前面是正确做法

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{
    os << '{';
    size_t count = 0;
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

    if (is >> ch && ch == '}') {
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
    if (this == &s) { return *this; }
    val = s.val;
    return *this;
}

template<typename T>
S<T>::S(S&& s) noexcept : val(std::move(s.val))
{
}

template<typename T>
S<T>& S<T>::operator=(S&& s) noexcept
{
    if (this == &s) { return *this; }
    val = std::move(s.val);
    return *this;
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

#endif // DRILL_19_1_H
