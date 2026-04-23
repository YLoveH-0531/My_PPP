#ifndef PATRON_H
#define PATRON_H

#include <string>

class Patron {
public:
    Patron(const std::string& name, const std::string& card_number);
    const std::string& get_name() const;
    const std::string& get_card_number() const;
    double get_fine_amount() const;
    bool has_fines() const;
private:
    std::string name;
    std::string card_number;
    double fine_amount = 0.0;
};

#endif // PATRON_H