#ifndef FORWARD_LINK_H
#define FORWARD_LINK_H

#include <string>

class Forward_link
{
    public:
        Forward_link() : succ(0){ }
        Forward_link(const std::string& s) : str(s), succ(0){ }
        Forward_link* add(Forward_link* n) ;                             // insert n after this object
        Forward_link* erase(Forward_link* head) ;                                          // remove this object from list
        Forward_link* erase_next() ;                                     // remove this object after this object
        Forward_link* find(const std::string& s);                           // find s in list
        const Forward_link* find(const std::string& s) const;            // find s in const list (see §18.5.1)
        void set_data(const std::string& s) { str = s; }
        std::string get_data() const { return str; }
        Forward_link* next() const { return succ; }
        void setnext(Forward_link* p) { succ = p; }
    private:
        std::string str;
        Forward_link* succ;
};

#endif //FORWARD_LINK_H