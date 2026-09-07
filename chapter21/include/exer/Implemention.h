#ifndef IMPLEMENTION_H
#define IMPLEMENTION_H

#include <iterator>

template <typename InputIt, typename Type>
typename std::iterator_traits<InputIt>::difference_type count(InputIt begin, InputIt end,
                                                              const Type &value) {
    return count_if(begin, end, [&value](const typename std::iterator_traits<InputIt>::value_type& elem) {
        return elem == value;
    });
}

template <typename InputIt, typename Pred>
typename std::iterator_traits<InputIt>::difference_type count_if(InputIt begin, InputIt end,
                                                                 Pred pred) {
    typename std::iterator_traits<InputIt>::difference_type ret = 0;
    while (begin != end) {
        if (pred(*begin)) {
            ret++;
        }
        ++begin;
    }
    return ret;
}

#endif // IMPLEMENTION_H