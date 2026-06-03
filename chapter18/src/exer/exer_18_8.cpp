/**
 * @file exer_18_8.cpp
 * @author KaKaRot
 * @brief 
 * @version 0.1
 * @date 2026-06-03
 * 
 * @copyright Copyright (c) 2026
 */

#include "exer_18_8.h"
#include "exer_18_1_to_4.h"
#include <algorithm>

bool is_palindrome(const std::string& s)
{
    std::string s1{s};
    std::reverse(s1.begin(), s1.end());
    return s == s1;
}

bool is_palindrome(const char s[], int n)
{
    if (s == nullptr) { return false; }
    char* p = new char[n + 1];
    for (int i = 0 ; i < n; i++) {
        p[i] = s[n - i - 1];
    }
    p[n] = '\0';
    bool palindrome = strcmp(s, p) == 0;
    delete[] p;
    return palindrome;
}

bool is_palindrome(const char* first, const char* last)
{
    if (first == nullptr || last == nullptr) { return false; }
    int count = 1;
    char* p = const_cast<char*>(first);
    while (p++ != last) { count++; }
    p = new char[count + 1];
    for (int i = 0 ; i < count; i++) {
        p[i] = first[count - i - 1];
    }
    p[count] = '\0';
    bool palindrome = strcmp(first, p) == 0;
    delete[] p;
    return palindrome;
}

