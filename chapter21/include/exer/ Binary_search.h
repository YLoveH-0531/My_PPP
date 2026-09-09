#ifndef BINARY_SEARCH_H
#define BINARY_SEARCH_H

#include <iterator>
template <typename Iter, typename T>
Iter lower_bound(Iter first, Iter last, const T &value) {
    auto count = std::distance(first, last);
    while (count > 0) {
        auto it = first;
        auto step = count / 2;
        std::advance(it, step);
        if ((*it) < value) {
            first = ++it;
            count -= step + 1;
        } else {
            count = step;
        }
    }
    return first;
}

template <typename Iter, typename T>
bool binary_search(Iter first, Iter last, const T &value) {
    auto it = lower_bound(first, last, value);
    if (it != last && !(value < *it)) {
        return true;
    }
    return false;
}

template <typename Iter, typename T>
Iter upper_bound(Iter first, Iter last, const T &value) {
    auto count = std::distance(first, last);
    while (count > 0) {
        auto it = first;
        auto step = count / 2;
        std::advance(it, step);
        if ((*it) <= value) {
            first = ++it;
            count -= step + 1;
        } else {
            count = step;
        }
    }
    return first;
}

template <typename Iter, typename T>
std::pair<Iter, Iter> equal_range(Iter first, Iter last, const T &value) {
    return std::make_pair(lower_bound(first, last, value), upper_bound(first, last, value));
}

#endif //  BINARY_SEARCH_H