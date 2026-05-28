/**
 * @file exer_17_14.cpp
 * @author KaKaRot
 * @brief 
 * @version 0.1
 * @date 2026-05-28
 * 
 * @copyright Copyright (c) 2026
 */

#include <iostream>
#include "Forward_Link.h"

void print_all(Forward_link* p)
{
    std::cout << "{";
    while(p){
        std::cout << p->get_data();
        if((p = p->next())){ std::cout << ", ";}
    }
    std::cout << "}" << std::endl;
}

void Forward_link_use()
{
    Forward_link* norse_head = new Forward_link{"Thor"};
    Forward_link* p = norse_head;
    p = p->add(new Forward_link{"Odin"});
    p = p->add(new Forward_link{"Zeus"});
    p = p->add(new Forward_link{"Freia"});

    Forward_link* greek_head = new Forward_link{"Hera"};
    Forward_link* q = greek_head;
    q = q->add(new Forward_link{"Athena"});
    q = q->add(new Forward_link{"Mars"});
    q = q->add(new Forward_link{"Poseidon"});

    Forward_link* p1 = greek_head->find("Mars");
    if (p1) p1->set_data("Ares");

    Forward_link* p2 = norse_head->find("Zeus");
    if (p2) {
        if (p2==norse_head) norse_head = p2->next();
        p2->erase(norse_head);
        p = p->add(p2);
    }

    print_all(norse_head);
    print_all(greek_head);
}

int main()
{
    Forward_link_use();
    return 0;
}