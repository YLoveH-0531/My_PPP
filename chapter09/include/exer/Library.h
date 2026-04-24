#ifndef LIBRARY_H
#define LIBRARY_H

#include "Book.h"
#include "Patron.h"
#include <vector>
class Library {
    public:
        void add_book(const Book& book);
        void add_patron(const Patron& patron);
        void check_out_book(const std::string& isbn, const std::string& card_number, const Chrono::Date& date);
        void check_in_book(const std::string& isbn, const std::string& card_number);
        std::vector<std::string> owned_fees() const;
    private:
        std::vector<Book> books;
        std::vector<Patron> patrons;
        struct Transaction {
            Book book;
            Patron patron;
            Chrono::Date date;
        };
        std::vector<Transaction> transactions;
};

#endif // LIBRARY_H