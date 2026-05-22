#ifndef EXER_8_5_6_H
#define EXER_8_5_6_H
 
#include <vector>
#include <string>

typedef std::vector<int>::size_type int_vec_sz;
typedef std::vector<std::string>::size_type str_vec_sz;

void reverse_vector_int(std::vector<int>& vec);
std::vector<int> reverse_vector_int_not_inplace(std::vector<int>& vec);
void reverse_vector_str(std::vector<std::string>& vec);
std::vector<std::string> reverse_vector_str_not_inplace(std::vector<std::string>& vec);
 
#endif // EXER_8_5_6_H

