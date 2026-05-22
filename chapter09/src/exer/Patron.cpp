/**
 * @file     Patron.cpp
 * @author   KaKaRot
 * @date     23/4/2026
 * @brief    create a class Patron
 * @details  create a class Patron with data members: name, card_number, and fine_amount
 */

#include "Patron.h"

Patron::Patron(const std::string& name, const std::string& card_number) : name(name), card_number(card_number) {
}
const std::string& Patron::get_name() const{
    return name;
}
const std::string& Patron::get_card_number() const{
    return card_number;
}
double Patron::get_fine_amount() const{
    return fine_amount;
}
bool Patron::has_fines() const{
    return fine_amount > 0;
}