/**
 * @file exer_17_11.cpp
 * @author KaKaRot
 * @brief 
 * @version 0.1
 * @date 2026-05-27
 * 
 * @copyright Copyright (c) 2026
 */

#include <iostream>
#include "Link.h"

void print_all(Link* p)
{
    std::cout << "{";
    while(p){
        std::cout << p->get_data().name;
        if((p = p->next())){ std::cout << ", ";}
    }
    std::cout << "}" << std::endl;
}

void link_use()
{
    Link* gods = new Link({"Apollo", "Greek", "Sun chariot", "Silver bow and arrows"});
    gods = gods->insert(new Link({"Zeus", "Greek", "", "lightning"}));
    gods = gods->insert(new Link({"Athena",  "Greek", "",  "Spear and aegis"}));
    gods = gods->insert(new Link({"Freya",     "Norse", "Chariot pulled by cats", "Magic necklace Brisingamen"}));
    gods = gods->insert(new Link({"Odin",      "Norse", "Eight-legged horse Sleipnir", "Spear Gungnir"}));
    gods = gods->insert(new Link({"Diana",     "Roman", "",              "Bow and arrows"}));
    gods = gods->insert(new Link({"Jupiter",   "Roman", "Eagle",          "Lightning bolt"}));
}

int main()
{
    link_use();
    return 0;
}