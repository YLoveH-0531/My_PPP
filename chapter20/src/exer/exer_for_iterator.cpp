/**
 * @file exer_for_iterator.cpp
 * @author KaKaRot
 * @brief understand the iterator traits 
 * @version 0.1
 * @date 2026-08-31
 * 
 * @copyright Copyright (c) 2026
 */

template <typename It>
struct iterator_trait
{
    using value_type = typename It::value_type; 
};


template <typename T>
struct iterator_trait<T*>
{
    using value_type = T; 
};

template <typename T>
struct iterator_trait<const T *> {
    using value_type = T;
};