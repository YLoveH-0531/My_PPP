#include "exer_8_2.h"

void print(const std::string& str, const std::vector<int>& vec, std::ostream& os)
{
    os << str << " = {" ;
    bool last = false;
    std::size_t count = 0;
    for (const auto& i : vec) {
        count++;
        if (count == vec.size()) {
            last = true;  
        }
        os << i << (last ? "" : ", ");

    }
    os << "}\n";
}