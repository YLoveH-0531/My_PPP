/**
 * @file exer_17_9.cpp
 * @author KaKaRot
 * @brief 
 * @version 0.1
 * @date 2026-05-26
 * 
 * @copyright Copyright (c) 2026
 */

#include <iostream>

void callee(int* caller_var) {
    int local = 0;
    std::cout << "caller var: " << caller_var << "\n";
    std::cout << "callee var: " << &local    << "\n";
    if (&local < caller_var)
        std::cout << "Stack grows DOWN (toward lower addresses)\n";
    else
        std::cout << "Stack grows UP (toward higher addresses)\n";
}

int main()
{
    // ── 栈方向 ──────────────────────────
    int a = 0;
    callee(&a);  // 传入 main 的局部变量地址，与 callee 内部比较

    // ── 堆方向 ──────────────────────────

    int* pa = new int[3];
    int* pb = new int[5];
    for (int i = 0; i < 3; ++i)
    {
        pa[i] = i + 1;
    }
    for (int i = 0; i < 5; ++i)
    {
        pb[i] = i + 1;
    }
    std::cout << "Addresses of pa and pb: " << pa << " " << pb << std::endl;
    delete[] pa;
    delete[] pb;
    return 0;
}