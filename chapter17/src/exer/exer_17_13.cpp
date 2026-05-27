/**
 * @file exer_17_13.cpp
 * @author KaKaRot
 * @brief 
 * @version 0.1
 * @date 2026-05-27
 * 
 * @copyright Copyright (c) 2026
 */

#include <iostream>
#include "Link.h"

void print_all(const std::string str, Link* p)
{
    if (!p){
        std::cout << "No data" << '\n';
        return ;
    }
    
    std::cout << str << '\n';
    while(p){
        std::cout << " {";
        std::cout << p->get_data().name << ' '
                  << p->get_data().mythology << ' '
                  << p->get_data().vehicle << ' '
                  << p->get_data().weapon << "}\n";
        p = p->next();
    }
}

void link_exam()
{   
    Link* gods = new Link({"Apollo", "Greek", "Sun chariot", "Silver bow and arrows"});
    gods = gods->insert(new Link({"Zeus", "Greek", "", "lightning"}));
    gods = gods->insert(new Link({"Athena",  "Greek", "",  "Spear and aegis"}));
    gods = gods->insert(new Link({"Freya",     "Norse", "Chariot pulled by cats", "Magic necklace Brisingamen"}));
    gods = gods->insert(new Link({"Odin",      "Norse", "Eight-legged horse Sleipnir", "Spear Gungnir"}));
    gods = gods->insert(new Link({"Diana",     "Roman", "",              "Bow and arrows"}));
    gods = gods->insert(new Link({"Jupiter",   "Roman", "Eagle",          "Lightning bolt"}));

    Link* greek_gods = nullptr;
    Link* Norse_gods = nullptr;
    Link* Roman_gods = nullptr;

    while (gods){

        Link* current_god = gods;
        gods = gods->erase();

        if (current_god->get_data().mythology == "Greek"){
            if (!greek_gods){
                greek_gods = current_god;
            }else{
                greek_gods = greek_gods->add_ordered(current_god);
            }
        }else if (current_god->get_data().mythology == "Norse"){
            if (!Norse_gods){
                Norse_gods = current_god;
            }else{
                Norse_gods = Norse_gods->add_ordered(current_god);
            }
        }else if (current_god->get_data().mythology == "Roman"){
            if (!Roman_gods){
                Roman_gods = current_god;
            }else{
                Roman_gods = Roman_gods->add_ordered(current_god);
            }          
        }
    }

    //print_all("", gods);
    print_all("Greek", greek_gods);
    print_all("Norse", Norse_gods);
    print_all("Roman", Roman_gods);
}

int main()
{
    link_exam();
    return 0;
}