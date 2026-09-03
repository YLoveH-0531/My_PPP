#ifndef LEARN_AND_PRACTICE_H
#define LEARN_AND_PRACTICE_H
#include <vector>

template<typename It, typename V>
It find(It first, It last, V value){
    while (first != last && *first!=value) ++first;
    return first; 
}

template<typename It, typename Pred>
It find_if(It first, It last, Pred pred){
    while (first != last && !pred(*first)) ++first;
    return first; 
}

template<typename C, typename V>
typename C::iterator find(C c, V v)
{
    return find(c.begin(), c.end(), v);
}

template<typename type>
void f(std::vector<type>& v, int x)
{
    auto fu = find_if(v.begin(), v.end(), [x](int a) { return a > x; });
    if (fu != v.end()) {
        // do something
    }
}

template<typename In, typename T>
T accumulate(In first, In last, T value)
{
    while (first != last) {
        value = value + *first;
        ++first;
    }
    return value;
}

template<typename In, typename T, typename BinOp>
T accumulate(In first, In end, T value, BinOp op)
{
    while (first != end) {
       value = op(value, *first);
       ++first;
    }
    return value;
}

template<typename In1, typename In2, typename T>
T inner_product(In1 first1, In1 end1, In2 first2, T init)
{
   while (first1 != end1) {
       init = init + *first1 * *first2;
       ++first1; 
       ++first2; 
   } 
   return init;
}

template<typename In1, typename In2, typename T, typename BinOp1, typename BinOp2>
T inner_product(In1 first1, In1 end1, In2 first2, T init, BinOp1 op1, BinOp2 op2)
{
   while (first1 != end1) {
       init = op1(init, op2(*first1, *first2));
       ++first1; 
       ++first2; 
   } 
   return init;
}

template<typename In, typename Out>
// requires Input_iterator<In>() && Output_iterator<Out>()
// do not check the range of Out
Out copy(In first, In last, Out out)
{
    while (first != last) {
       *out = *first;
       ++first;
       ++out; 
    }    
    return out;
}

template<typename In, typename Out, typename Pred>
Out copy_if(In first, In last, Out out, Pred pred)
{
    while (first != last) {
       if (pred(*first)) {
           *out = *first;
           ++out;
       } 
       ++first;
    }
    return out;
}

#endif // LEARN_AND_PRACTICE_H