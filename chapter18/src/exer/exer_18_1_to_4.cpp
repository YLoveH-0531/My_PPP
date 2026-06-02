/**
 * @file exer_18_1_to_4.cpp
 * @author KaKaRot
 * @brief 
 * @version 0.1
 * @date 2026-06-02
 * 
 * @copyright Copyright (c) 2026
 */
#include "exer_18_1_to_4.h"

char* strdup(const char* pc)
{
    if (!pc){ return nullptr; }
    const char* p = pc;
    int count = 0;
    while (*p++ != 0){ count++; }
    
    p = pc;
    char* pa = new char[count];
    for (int i = 0; i < count; i++){
        *pa++ = *p++;
    }
    return pa;
}

char* strdup(const char* pc, int max)
{
    if (!pc){ return nullptr; }
    const char* p = pc;
    int count = 0;
    while (*p++ != 0){ 
        if (++count >= max) { break; }
    }
    
    p = pc;
    char* pa = new char[count];
    for (int i = 0; i < count; i++){
        *pa++ = *p++;
    }
    return pa;
}

char* findx(const char* s, const char* x)
{
    for (const char* s1 = s; *s1 != 0; s1++) {
        const char* s2 = s1;
        const char* x1 = x;
        for ( ;*x1 != 0 && *s2 != 0; x1++, s2++) {
            if (*x1 != *s2) { break; }
        }
        if (*x1 == 0) { return const_cast<char*>(s1); }
        if (*s2 == 0) { break; }
    }
    return nullptr;
}

char* findx(const char* s, const char* x, int max)
{

}

int strcmp(const char* s1, const char* s2)
{

}

int strcmp(const char* s1, const char* s2, int max)
{

}