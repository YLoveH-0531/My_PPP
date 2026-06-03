/**
 * @file exer_18_5_6_7.cpp
 * @author KaKaRot
 * @brief 
 * @version 0.1
 * @date 2026-06-03
 * 
 * @copyright Copyright (c) 2026
 */
#include "exer_18_5_6_7.h"

std::string cat_dot(const std::string& s1, const std::string& s2)
{
    return s1 + "." + s2;
}

std::string cat_dot(const std::string& s1, const std::string& s2, char ch)
{
    return s1 + ch + s2;
}

char* cat_dot(const char* s1, const char* s2)
{
    if (s1 == nullptr || s2 == nullptr) { return nullptr; }
    const char* s1p = s1;
    const char* s2p = s2;
    int count = 0;
    while (*s1p++ != 0) { count++; }
    while (*s2p++ != 0) { count++; }
    char* p = new char[count + 2];
    char* q = p;
    s1p = s1;
    s2p = s2;
    while (*s1 != 0) { *q++ = *s1++; }
    *q++ = '.';
    while (*s2 != 0) { *q++ = *s2++; }
    *q = '\0';
    return p;
}

char* cat_dot(const char* s1, const char* s2, char ch)
{
    if (s1 == nullptr || s2 == nullptr) { return nullptr; }
    const char* s1p = s1;
    const char* s2p = s2;
    int count = 0;
    while (*s1p++ != 0) { count++; }
    while (*s2p++ != 0) { count++; }
    char* p = new char[count + 2];
    char* q = p;
    s1p = s1;
    s2p = s2;
    while (*s1 != 0) { *q++ = *s1++; }
    *q++ = ch;
    while (*s2 != 0) { *q++ = *s2++; }
    *q = '\0';
    return p;
}
