#ifndef NUMBER_H
#define NUMBER_H

#include <utility>
#include <stdexcept>

template <typename T>
class Number
{
    public:
    Number() : val() {}
    Number(const T& t) : val(t) {}
    Number(T&& t) : val(std::move(t)) {}
    Number(const Number& rhs) : val(rhs.val) {}
    Number& operator=(const Number& rhs)
    {
        if (this == &rhs) { return *this; }
        val = rhs.val;
        return *this;
    }
    Number(Number&& rhs) noexcept : val(std::move(rhs.val)) {}
    Number& operator=(Number&& rhs) noexcept
    {
        if (this == &rhs) { return *this; }
        val = std::move(rhs.val);
        return *this;
    }
    ~Number() {}
    Number operator+(const Number& rhs) const
    {
        return Number(val + rhs.val);
    }
    Number& operator+=(const Number& rhs)
    {
        val += rhs.val;
        return *this;
    }
    Number operator-(const Number& rhs) const
    {
        return Number(val - rhs.val);
    }
    Number operator*(const Number& rhs) const   
    {
        return Number(val * rhs.val);
    }
    Number operator/(const Number& rhs) const
    {
        if (rhs.val == 0) { throw std::runtime_error("Division by zero"); }
        return Number(val / rhs.val);
    }
    Number operator%(const Number& rhs) const
    {
        if (rhs.val == 0) { throw std::runtime_error("Division by zero"); }
        return Number(val % rhs.val);
    }
    bool operator==(const Number& rhs) const
    {
        return val == rhs.val;
    }
    bool operator!=(const Number& rhs) const
    {
        return val != rhs.val;
    }
    private:
    T val;
};

#endif //NUMBER_H