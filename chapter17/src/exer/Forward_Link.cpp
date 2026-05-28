/**
 * @file Forward_Link.cpp
 * @author KaKaRot
 * @brief 
 * @version 0.1
 * @date 2026-05-28
 * 
 * @copyright Copyright (c) 2026
 */

#include "Forward_Link.h"

Forward_link* Forward_link::add(Forward_link* n)
{
    if (n == nullptr){ return this; }
    n->succ = succ;
    succ = n;
    return n;   
}   

Forward_link* Forward_link::erase(Forward_link* head)
{
    if (head == nullptr) { return nullptr;}
    
    if (head == this) { 
        head = succ;
        setnext(nullptr);
        return this;
    }

    Forward_link* p = head;
    while (p->succ != this){
        if (p->succ == nullptr) { return nullptr;}
        
        p = p->next();
    }
    
    return p->erase_next();
}
                                  
Forward_link* Forward_link::erase_next()
{
    if (!succ) { return nullptr; }
    
    Forward_link* tmp = succ;
    succ = succ->next();
    tmp->setnext(nullptr);
    return tmp;
}
                                  
const Forward_link* Forward_link::find(const std::string& s) const
{
    const Forward_link* p = this;
    while(p != nullptr && p->str != s ){
        p = p->next();
    }
    return p;  
}

Forward_link* Forward_link::find(const std::string& s)
{
    Forward_link* p = this;
    while(p != nullptr && p->str != s ){
        p = p->next();
    }
    return p;  
}