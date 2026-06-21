template<typename Iter1, typename Iter2>
// requires Input_iterator<Iter1>() && Output_iterator<Iter2>()
Iter2 copy(Iter1 f1, Iter1 e1, Iter2 f2)
{
    if (f1 == e1) { return f2; }
    while (f1 != e1) {
        *f2++ = *f1++;
    }
    return f2;
}