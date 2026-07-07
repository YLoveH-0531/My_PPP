#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <cstddef>
#include <istream>
#include <list>
#include <vector>
#include <string>

using Line = std::vector<char>; // a line is a vector of characters

class Text_iterator { 
    // keep track of line and character position within a line
    std::list<Line>::iterator ln;
    Line::iterator pos;
public:
    // start the iterator at line ll’s character position pp:
    using iterator_category = std::forward_iterator_tag;
    using value_type = char;
    using difference_type = std::ptrdiff_t;
    using pointer = char*;
    using reference = char&;
    
    Text_iterator(std::list<Line>::iterator ll, Line::iterator pp) : ln{ll}, pos{pp} { }
    char& operator*() { return *pos; }
    Text_iterator& operator++();
    bool operator==(const Text_iterator& other) const
        { return ln==other.ln && pos==other.pos; }
    bool operator!=(const Text_iterator& other) const
        { return !(*this==other); }

    friend Text_iterator 
    find_and_replace(Text_iterator first, Text_iterator last, const std::string& find, const std::string& replace);
};

struct Document {
    std::list<Line> line;                       // a document is a list of lines
    Document() { line.push_back(Line{}); }
    Text_iterator begin()                       // first character of first line
    { return Text_iterator(line.begin(), (*line.begin()).begin()); }
    Text_iterator end()                         // one beyond the last character of the last line
    {
        auto last = line.end();
        --last;                                 // we know that the document is not empty
        return Text_iterator(last, (*last).end());
    }
};


std::istream& operator>>(std::istream& is, Document& d);
void print(Document& d);
void erase_line(Document& d, int n);
bool match(Text_iterator first, Text_iterator last, const std::string& s);

Text_iterator find_txt(Text_iterator first, Text_iterator last, const std::string& s);

Text_iterator find_and_replace(Text_iterator first, Text_iterator last,
                               const std::string& find, const std::string& replace);

std::size_t count_char(Text_iterator first, Text_iterator last);
std::size_t count_words_with_space(Text_iterator first, Text_iterator last);
std::size_t count_words_with_character(Text_iterator first, Text_iterator last);

#endif //DOCUMENT_H