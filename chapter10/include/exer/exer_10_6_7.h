#ifndef EXER_10_6_7_H
#define EXER_10_6_7_H

#include <iostream>
#include <string>

// static int roman_to_int(const std::string& roman);
// static std::string int_to_roman(int val);

class Roman_int {
    friend std::ostream& operator<<(std::ostream& os, const Roman_int& r);
    friend std::istream& operator>>(std::istream& is, Roman_int& r);
    friend Roman_int operator+(const Roman_int& lhs, const Roman_int& rhs);
public:
    Roman_int() : val(0) { }
    Roman_int(const std::string& s);
    Roman_int(int v);
    int as_int() const { return val; }
    operator int() const { return val; }
    std::string to_string() const { return roman; }
    operator std::string() const { return roman; }
private:
    std::string roman;
    int val;
};

#endif // EXER_10_6_7_H