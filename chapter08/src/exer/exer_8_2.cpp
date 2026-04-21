#include "exer_8_2.h"

void print(const std::string& str, const std::vector<int>& vec, std::ostream& os)
{
    os << str << " = {" ;
    bool last = false;
    std::size_t count = 0;
    std::size_t vec_size = vec.size();
    for (const auto& i : vec) {
        count++;
        if (count == vec_size) {
            last = true;  
        }
        os << i << (last ? "" : ", ");

    }
    os << "}\n";
}

void print(const std::string& str, const std::vector<std::string>& vec, std::ostream& os)
{
    os << str << " = {" ;
    bool last = false;
    std::size_t count = 0;
    std::size_t vec_size = vec.size();
    for (const auto& i : vec) {
        count++;
        if (count == vec_size) {
            last = true;  
        }
        os << i << (last ? "" : ", ");

    }
    os << "}\n";
}