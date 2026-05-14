/**
 * @file     Test_output.cpp
 * @author   KaKaRot
 * @date     14/5/2026
 * @brief    practice output format 
 * @details  Practice formatting output in C++
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

struct Person
{
    std::string last_name;
    std::string first_name;
    int age;
    std::string telephone;
    std::string email;
};

int main()
{
    int birth_year = 1994;
    int age = 2026 - birth_year;
    std::cout << "Birth year: " << '\n';
    std::cout << std::showbase;
    std::cout << birth_year << '\t';
    std::cout << std::hex << birth_year << '\t';
    std::cout << std::oct << birth_year << '\n';

    std::cout << "Age: " << '\n';
    std::cout << std::dec << age << '\t';
    std::cout << std::hex << age << '\t';
    std::cout << std::oct << age << '\n';

    std::cout.setf(std::ios::dec, std::ios::basefield);
    int a = 12345, b = 12345, c = 12345, d = 12345;
    std::cout << "The integers are: "; 
    std::cout << a << '\t'<< b << '\t'<< c << '\t'<< d << '\n' ;

    float a1 = 1234567.89;
    std::cout << "The float number is: " << '\n';
    std::cout << a1 << '\t';
    std::cout << std::fixed << a1 << '\t';
    std::cout << std::scientific << a1 << '\n';
    std::cout << "Scientific is better for large numbers, and fixed is better for small numbers.\n";

    std::vector<Person> people = {
        {"Smith", "John", 30, "123-456-7890", "john.smith@example.com"},
        {"Doe", "Jane", 25, "987-654-3210", "jane.doe@example.com"},
        {"Brown", "Charlie", 35, "555-555-5555", "charlie.brown@example.com"},
        {"Johnson", "Emily", 28, "111-222-3333", "emily.johnson@example.com"},
        {"Davis", "Michael", 40, "888-888-8888", "michael.davis@example.com"}
    };

    for (const auto& person : people) {
        std::cout << std::left << std::setw(15) << person.last_name
                  << std::setw(15) << person.first_name
                  << std::setw(5) << person.age
                  << std::setw(20) << person.telephone
                  << std::setw(30) << person.email
                  << '\n';
    }

    return 0;
}