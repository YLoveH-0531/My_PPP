#ifndef LINK_H
#define LINK_H

#include <string>

struct God{
      std::string name;       
      std::string mythology;  
      std::string vehicle;    
      std::string weapon;     
    
};

class Link
{
    public:
        Link() : succ(0), prev(0) { }
        Link(const God& g) : god(g), succ(0), prev(0) { }
        Link* insert(Link* n) ;                          // insert n before this object
        Link* add(Link* n) ;                             // insert n after this object
        Link* erase() ;                                  // remove this object from list
        Link* find(const std::string& s);                // find s in list
        const Link* find(const std::string& s) const;    // find s in const list (see §18.5.1)
        Link* advance(int n) const;                      // move n positions in list
        Link* add_ordered(Link* n);                      // insert n in alphabetic order
        void set_data(God g) { god = g; }
        God get_data() const { return god; }
        Link* next() const { return succ; }
        Link* previous() const { return prev; }
    private:
        God god;
        Link* succ;
        Link* prev;
};


#endif // LINK_H