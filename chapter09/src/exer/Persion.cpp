/**
 * @file     Persion.cpp
 * @author   KaKaRot
 * @date     23/4/2026
 * @brief    create a class Person
 * @details  create a class Person with two data members: name and age, 
 *           and read the name and age from the user, 
 *           and overload the == and != operators to compare two Person objects, 
 *           and overload the << operator to print a Person object
 */

#include "Person.h"

void Person::read_names() {
    std::cout << "Enter names (type 'q' to finish): ";
    std::string input;
    while (std::cin >> input && input != "q") {
        name.push_back(input);
    }
}

void Person::read_ages() {
    std::cout << "Enter ages (type '-1' to finish): ";
    double input;
    while (std::cin >> input && input != -1) {
        age.push_back(input);
    }
}

bool Person::operator==(const Person& p) const {
    if (name.size() != p.name.size() || age.size() != p.age.size()) {
        return false;
    }

    for (size_t i = 0; i < name.size(); ++i) {
        if (name[i] != p.name[i] || age[i] != p.age[i]) {
            return false;
        }
    }
    return true;
}

bool Person::operator!=(const Person& p) const {
    return !(*this == p);
}

std::ostream& operator<<(std::ostream& os, const Person& p) {
    for (size_t i = 0; i < p.get_names().size(); ++i) {
        os << "Name: " << p.get_names()[i] << ", Age: " << p.get_ages()[i] << std::endl;
    }
    return os;
}