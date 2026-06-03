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
    char* pa = new char[count + 1];
    char* pb = pa;
    for (int i = 0; i < count; i++){
        *pb++ = *p++;
    }
    *pb ='\0';
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
    char* pa = new char[count + 1];
    char* pb = pa;
    for (int i = 0; i < count; i++){
        *pb++ = *p++;
    }
    *pb = '\0';
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
    for (int i = 0; i < max; i++) {
        const char* s1 = s + i;
        const char* x1 = x;
        while (*x1 != 0 && (s1 - s) < max && *x1 == *s1) {
            x1++;
            s1++;
        }
        if (*x1 == 0) { return const_cast<char*>(s + i); }
    }
    return nullptr;    
}

int strcmp(const char* s1, const char* s2)
{
    for (; *s1 != 0 && *s2 != 0; s1++, s2++) {
        if (*s1 < *s2) { return -1; }
        if (*s1 > *s2) { return 1; }
    }
    if (*s1 == 0 && *s2 == 0) { 
        return 0; 
    }else if (*s1 == 0 && *s2 != 0) {
        return -1;
    }else{
        return 1;
    }
}

int strcmp(const char* s1, const char* s2, int max)
{
    int count = 0;
    for (; *s1 != 0 && *s2 != 0 && ++count <= max ; s1++, s2++) {
        if (*s1 < *s2) { return -1; }
        if (*s1 > *s2) { return 1; }
    }
    if (*s1 == 0 && *s2 != 0) { 
        return -1; 
    }else if (*s1 != 0 && *s2 == 0) {
        return 1;
    }else{
        return 0;
    }
}