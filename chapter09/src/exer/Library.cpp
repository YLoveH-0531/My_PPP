/**
 * @file     Library.cpp
 * @author   KaKaRot
 * @date     23/4/2026
 * @brief    create a class Library
 * @details  create a class Library
 */

 #include "Library.h"

void Library::add_book(const Book& book) {
    books.push_back(book);
}

void Library::add_patron(const Patron& patron) {
    patrons.push_back(patron);
}

void Library::check_out_book(const std::string& isbn, const std::string& card_number, const Chrono::Date& date) {
    for(std::vector<Book>::iterator it = books.begin(); it != books.end(); ++it) {
        if(it->get_isbn() == isbn) {
            if(it->is_available()) {
                bool patron_found = false;    // Flag to check if patron is found
                for(std::vector<Patron>::iterator pt = patrons.begin(); pt != patrons.end(); ++pt) {
                    if(pt->get_card_number() == card_number) {
                        if (pt->has_fines()){
                            throw std::runtime_error("Patron with the given card number has outstanding fines.");
                        }else {                     
                            patron_found = true;
                            it->check_out();      // Mark the book as checked out
                            Transaction transaction{*it, *pt, date};
                            transactions.push_back(transaction);
                            return;
                        }
                    }
                }
                if(!patron_found) {
                    throw std::runtime_error("Patron with the given card number not found.");
                }
            } else {
                throw std::runtime_error("Book is not available for checkout.");
            }
        }
    }
    throw std::runtime_error("Book with the given ISBN not found.");
}

void Library::check_in_book(const std::string& isbn, const std::string& card_number) {
    for(std::vector<Transaction>::iterator it = transactions.begin(); it != transactions.end(); ++it) {
        if(it->book.get_isbn() == isbn && it->patron.get_card_number() == card_number) {
            it->book.check_in();   // Mark the book as checked in
            transactions.erase(it); // Remove the transaction from the list
            return;
        }
    }
    throw std::runtime_error("Transaction with the given ISBN or card number not found.");
}

std::vector<std::string> Library::owned_fees() const {

    std::vector<std::string> names_with_fines;
    for (const auto& patron : patrons) {
        if (patron.has_fines()) {
            names_with_fines.push_back(patron.get_name());
        }
    }
    return names_with_fines;
}
