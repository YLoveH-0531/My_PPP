/**
 * @file exer_17_3.cpp
 * @author KaKaRot
 * @brief  
 * @version 0.1
 * @date 2026-05-25
 * 
 * @copyright Copyright (c) 2026
 */

#include <iostream>

void to_lower(char* s)
{
    for (int i = 0; s[i] != '\0'; ++i)
    {
        if (s[i] >= 'A' && s[i] <= 'Z')
        {
            s[i] = s[i] - 'A' + 'a';
        }
        std::cout << s[i];
    }
    std::cout << std::endl;
} 

int main()
{
    char str[] = "Hello, World!";
    for (int i = 0; "Hello, World!"[i] != '\0'; ++i){
        std::cout << "Hello, World!"[i];
    }
    to_lower(str);
    return 0;
}

/**
 * 这是 C/C++ 极其底层的设计：下标运算符 [] 只是指针算术（Pointer Arithmetic）的语法糖。
 * 在编译器底层，对于任意的表达式 A[B]，都会被无条件地转化为：*(A + B)
 * 
 *1).数组退化为指针（Array Decay）：
 * 在表达式 "Hello, World!" + i 中，类型为 const char[14] 的字符串字面量自动退化（Decay）为指向其第一个字符 'H' 的指针，类型变为 const char*。
 *
 *2).指针偏移：
 * + i 表示将这个指针向后移动 i 个 char 的位置（即增加 i * sizeof(char) 个字节）。
 *
 *3).解引用（Dereference）：
 * 最外层的 * 或者是 [] 运算符负责去那个偏移后的内存地址，把对应的字符给抠出来。
 */