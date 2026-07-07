#ifndef EXER_20_12_13_H
#define EXER_20_12_13_H

template<typename Elem>
struct Link {
    Link* prev;
    Link* succ;
    Elem val;
};

template<typename Elem>
class list {
public:
    class iterator;
    iterator begin();
    iterator end( );
    iterator insert(iterator p, const Elem& v);
    iterator erase(iterator p); 
    void push_back(const Elem& v);
    void push_front(const Elem& v);
    void pop_front();
    void pop_back();
    Elem& front();
    Elem& back();
};

template<typename Elem>
// requires Element<Elem>() (§19.3.3)
class list<Elem>::iterator {
    Link<Elem>* curr; // current link
public:
    iterator(Link<Elem>* p) :curr{p} { }
    iterator& operator++() {curr = curr->succ; return *this; } // forward
    iterator& operator--() { curr = curr->prev; return *this; } // backward
    Elem& operator*() { return curr->val; } // get value (dereference)
    bool operator==(const iterator& b) const { return curr==b.curr; }
    bool operator!= (const iterator& b) const { return curr!=b.curr; }
};

#endif //EXER_20_12_13_H