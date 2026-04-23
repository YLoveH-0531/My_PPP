#ifndef BOOK_H
#define BOOK_H

#include <string>
#include <iostream>
#include "Chrono.h"

class Book {
public:
    Book(const std::string& name, const std::string& author, const std::string& isbn, const Chrono::Date& publication_date);
    const std::string& get_name() const;
    const std::string& get_author() const;
    const std::string& get_isbn() const;
    const Chrono::Date& get_publication_date() const;
    bool is_available() const;
    void check_out() { is_checked_out = true; }
    void check_in() { is_checked_out = false; }

private:
    bool isvalid_isbn(const std::string& isbn) const;
    std::string name;
    std::string author;
    std::string isbn;
    Chrono::Date publication_date;
    bool is_checked_out = false;
};

#endif // BOOK_H