#ifndef MAIL_PROGRAM_H
#define MAIL_PROGRAM_H

#include <map>
#include <string>
#include <vector>

typedef std::vector<std::string>::const_iterator Line_Iter;

class Message {
public:
    Message();
    Message(Line_Iter f, Line_Iter l) : first(f), last(l) {}

    Line_Iter begin() const { return first; }
    Line_Iter end() const { return last; }
    
private:
    Line_Iter first;
    Line_Iter last;
};

typedef std::vector<Message>::const_iterator iter;
struct Mail_file{

    Mail_file() {};
    Mail_file(const std::string& file); 

    std::string name;
    std::vector<std::string> lines;
    std::vector<Message> letter;
    std::multimap<std::string, const Message*> sub_map;

    iter begin() const { return letter.begin(); }
    iter end() const { return letter.end(); }
    
};

bool find_by_address(const Message* m, std::string& s);

bool find_by_address_regex(const Message* m, std::string& s);

std::string find_subject(const Message* m);

std::string find_subject_regex(const Message* m);

#endif // MAIL_PROGRAM_H