/**
 * @file Document.cpp
 * @author KaKaRot
 * @brief 
 * @version 0.1
 * @date 2026-06-29
 * 
 * @copyright Copyright (c) 2026
 */

#include "Document.h"
#include <iostream>
#include <algorithm>
#include <string>

std::istream& operator>>(std::istream& is, Document& d)
{
    for (char ch; is.get(ch); ) {
        d.line.back().push_back(ch);
        // add the character
        if (ch=='\n')
            d.line.push_back(Line{}); // add another line
    }
    if (d.line.back().size()) d.line.push_back(Line{}); // add final empty line
    return is;
}

Text_iterator& Text_iterator::operator++()
{
    ++pos;
    // proceed to next character
    if (pos==(*ln).end()) {
    ++ln; // proceed to next line
    pos = (*ln).begin();
    }
    // bad if ln==line.end(); so make sure it isn’t
    return *this;
}

void print(Document& d)
{
    for (auto p : d) std::cout << p;
}

void erase_line(Document& d, int n)
{
    if (n<0 || d.line.size()- 1 <= static_cast<size_t>(n)) return;
    auto p = d.line.begin();
    advance(p,n);
    d.line.erase(p);
}

bool match(Text_iterator first, Text_iterator last, const std::string& s)
{
    for (char c : s) {
        if (first==last || c!=*first) return false;
        ++first;
    }
    return true;
}

Text_iterator find_txt(Text_iterator first, Text_iterator last, const std::string& s)
{
    if (s.size()==0) return last;   // can’t find an empty string
    char first_char = s[0];
    while (true) {
        auto p = std::find(first,last,first_char);
        if (p==last || match(p,last,s)) return p;
        first = ++p;                // look at the next character
    }
}