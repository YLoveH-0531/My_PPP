/**
 * @file     Book.cpp
 * @author   KaKaRot
 * @date     23/4/2026
 * @brief    create a class Book
 * @details  create a class Book with data members: title, author, and isbn
 */

#include "Book.h"
#include <sstream>
#include <stdexcept>

Book::Book(const std::string& name, const std::string& author, const std::string& isbn, const Chrono::Date& publication_date)
    : name(name), author(author), isbn(isbn), publication_date(publication_date), is_checked_out(false) {
        if (!isvalid_isbn(isbn)) {
            throw std::invalid_argument("Invalid ISBN");
        }
    }

const std::string& Book::get_name() const {
    return name;
}
const std::string& Book::get_author() const {
    return author;
}
const std::string& Book::get_isbn() const {
    return isbn;
}
const Chrono::Date& Book::get_publication_date() const {
    return publication_date;
}
bool Book::is_available() const {
    return !is_checked_out;
}

bool Book::isvalid_isbn(const std::string& isbn) const{
    std::istringstream iss(isbn);
    int n;
    char c;
    char delim = '-';
    int count = 0;
    while (iss >> n && count < 3) {
        count++;
        if (iss.peek() == delim) {
            iss.get(c); // consume the delimiter
        } else {
            return false; 
        }
    }

    if (count == 3 && iss >> c && isalnum(c) && iss.peek() == EOF) {   
        return true; 
    }

    return false;
}
