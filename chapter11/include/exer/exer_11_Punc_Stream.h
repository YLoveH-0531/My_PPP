#ifndef EXER_11_PUNC_STREAM_H
#define EXER_11_PUNC_STREAM_H

#include <iostream>
#include <sstream>
#include <string>

class Punc_Stream
{
    friend Punc_Stream& operator>>(Punc_Stream& ps, std::string& s);
    public:
        Punc_Stream() : is(std::cin), ws(" \t\n"), sensitive(false) {}
        Punc_Stream(std::istream& s) : is(s), ws(" \t\n"), sensitive(false) {}
        void whitespace(const std::string& s) { ws = s; }
        void add_punct(std::string s) { ws += s; }
        bool is_whitespace(char c) { return ws.find(c) != std::string::npos; }
        bool IsSensitive() { return sensitive; }
        void setSensitive(bool b) { sensitive = b; }
        explicit operator bool() const { return is.good(); }
    private:
        std::istream& is;
        std::istringstream buffer;
        std::string ws;
        bool sensitive;
};

Punc_Stream& operator>>(Punc_Stream& ps, std::string& s)
{
    while(!(ps.buffer >> s)){
        if(ps.buffer.bad() || !ps.is.good()) return ps;
        ps.buffer.clear();

        std::string line;
        std::getline(ps.is, line);
        for(char& c : line){
            if(ps.is_whitespace(c)){
                c = ' ';
            } else if(!ps.IsSensitive()){
                 c = static_cast<char>(std::tolower(c));
            }            
        }
        ps.buffer.str(line);
    }
    return ps;
}

#endif // EXER_11_PUNC_STREAM_H