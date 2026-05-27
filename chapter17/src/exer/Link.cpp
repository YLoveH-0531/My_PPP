/**
 * @file Link.cpp
 * @author KaKaRot
 * @brief Link class member function definitions
 * @version 0.1
 * @date 2026-05-27
 * 
 * @copyright Copyright (c) 2026
 */

#include "Link.h"

Link* Link::insert(Link* n){
    if (n == nullptr) return this;
    n->succ = this;
    if (prev) prev->succ = n;
    n->prev = prev;
    prev = n;
    return n;
}

Link* Link::add(Link* n){
    if (n == nullptr) return this;
    n->prev = this;
    if (succ) succ->prev = n;
    n->succ = succ;
    succ = n;
    return n;   
}

Link* Link::find(const std::string& s){   
    return const_cast<Link*>(const_cast<const Link*>(this)->find(s));
}

const Link* Link::find(const std::string& s) const{
    const Link* p = this;
    while(p != nullptr && p->god.name != s ){
        p = p->next();
    }
    return p;
}

Link* Link::advance(int n) const{
    Link* p = const_cast<Link*>(this);
    if(n>0){
        while(n--){
            if(p->succ == nullptr) return nullptr;
            p = p->succ;
        }
    }else if(n<0){
        while(n++){
            if(p->prev == nullptr) return nullptr;
            p = p->prev;
        }
    }
    return p;
}

Link* Link::erase() { 
    if (prev) prev->succ = succ;
    if (succ) succ->prev = prev;
    return succ;
}

Link* Link::add_ordered(Link* p){
    if(p == nullptr) return this;

    if(p->god.name < this->god.name){
        return insert(p);
    }

    Link* n = this;
    while((n = n->succ) && p->god.name >= n->succ->god.name)
    n->add(p);

    return this;
}
