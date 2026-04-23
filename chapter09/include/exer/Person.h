#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <vector>
#include <iostream>

class Person {
    public:
        void read_names();
        void read_ages();
        bool operator==(const Person& p) const;
        bool operator!=(const Person& p) const;
        std::vector<std::string> get_names() const { return name; }
        std::vector<double> get_ages() const { return age; }
    private:
        std::vector<std::string> name;
        std::vector<double> age;
};

std::ostream& operator<<(std::ostream& os, const Person& p);

#endif // PERSON_H