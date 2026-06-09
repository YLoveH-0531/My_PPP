#ifndef PAIR_H
#define PAIR_H

#include <utility>
namespace KaKaRot {
template <typename T, typename U>
struct Pair
{
    T first;
    U second;
    Pair() : first(), second() {}
    Pair(const Pair& p) : first(p.first), second(p.second) {};
    Pair& operator=(const Pair& p)
    {
        if(this == &p) { return *this; }
        first = p.first;
        second = p.second;
        return *this;
    }
    Pair(Pair&& p) noexcept : first(std::move(p.first)), second(std::move(p.second)) {}
    Pair& operator=(Pair&& p) noexcept
    {
        if (this == &p) { return *this; }
        first = std::move(p.first);
        second = std::move(p.second);
        return *this;
    }
    Pair(const T& t, const U& u) : first(t), second(u) {}
    Pair(T&& t, U&& u) : first(std::move(t)), second(std::move(u)) {}
    bool operator==(const Pair& rhs) const
    {
        return first == rhs.first && second == rhs.second;
    }
    bool operator!=(const Pair& rhs) const
    {
        return !(*this == rhs);
    }
    bool operator<(const Pair& rhs) const
    {
        if (first != rhs.first) {
            return first < rhs.first;
        }
        return second < rhs.second;
    }
};

template <typename T, typename U>
Pair<T, U> make_pair(T&& t, U&& u)
{
    return Pair<T, U>(std::forward<T>(t), std::forward<U>(u));
}
}


#endif //PAIR_H