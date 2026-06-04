/**
 * @file exer_18_9.cpp
 * @author KaKaRot
 * @brief  PPP 18.9 - observe which segment each kind of variable lives in.
 *
 * Observed address order (low -> high), confirmed by running this program:
 *
 *   .text   (code)            <-- &fun1                          (lowest)
 *   .rodata (read-only)       <-- const globals, static const,
 *                                 string literals
 *   .data   (init != 0)       <-- non-const globals/statics with
 *                                 a non-zero initializer
 *   .bss    (zero / uninit)   <-- non-const globals/statics that
 *                                 are 0 or uninitialized
 *   heap    (free store)      <-- new[] ; grows UPWARD
 *   ......large gap......
 *   stack   (locals)          <-- grows DOWNWARD                 (highest)
 *
 * Rules of thumb:
 *   - const            -> .rodata  (regardless of value, even 0)
 *   - non-const, != 0  -> .data
 *   - non-const, 0     -> .bss
 * Note: an address-of (&) is required, or the compiler may optimize the
 * variable away entirely.
 *
 * @version 0.1
 * @date 2026-06-03
 *
 * @copyright Copyright (c) 2026
 */

#include <iostream>

const int ga = 5;
const int gb = 0;
const int gc = 0;
int gd = 6;
int ge = 0;


void fun1()
{
    int fa = 2;
    int fb = 3;
    static int fc = 5;
    static const int fd = 5;
    static int fe;
    static int ff = 0;

    int* p = new int[10]();

    std::cout << ".data section: fc: " << &fc << std::endl;
    std::cout << ".rodata section: fd: " << &fd << std::endl;
    std::cout << ".bss section: fe: " << &fe << std::endl;
    std::cout << ".bss section: ff: " << &ff << std::endl;

    std::cout << "stack variable: " << &fa << std::endl;
    std::cout << "stack variable: " << &fb << std::endl;

    std::cout << "free store variable[0] in fun1: " << p << std::endl;
    std::cout << "free store variable[9] in fun1: " << p+9 << std::endl;

    delete[] p; // Don't forget to free the allocated memory
}

int main()
{
    int a = 2;
    int b = 3;
    static int c = 5;
    static const int d = 5;
    static int e;
    static int f = 0;

    int* p = new int[10]();

    std::cout << ".rodata section: ga: " << &ga << std::endl;
    std::cout << ".rodata section: gb: " << &gb << std::endl;
    std::cout << ".rodata section: gc: " << &gc << std::endl;
    std::cout << ".data section: gd: " << &gd << std::endl;
    std::cout << ".bss section: ge: " << &ge << std::endl;

    std::cout << ".data section: c: " << &c << std::endl;
    std::cout << ".rodata section: d: " << &d << std::endl;
    std::cout << ".bss section: e: " << &e << std::endl;
    std::cout << ".bss section: f: " << &f << std::endl;

    std::cout << "stack variable: " << &a << std::endl;
    std::cout << "stack variable: " << &b << std::endl;

    std::cout << "free store variable[0] in main: " << p << std::endl;
    std::cout << "free store variable[9] in main: " << p+9 << std::endl;

    // ---- additional categories that were missing ----

    // (1) string literal -> .rodata (the chars live in read-only memory)
    const char* lit = "hello, world";
    std::cout << ".rodata (string literal): "
              << static_cast<const void*>(lit) << std::endl;

    // (2) function code -> .text
    std::cout << ".text (address of fun1): "
              << reinterpret_cast<void*>(&fun1) << std::endl;

    // (3) a second heap block, allocated while p is still alive,
    //     to show the heap growing upward and both blocks coexisting
    int* p2 = new int[10]();
    std::cout << "free store block #2 in main: " << p2 << std::endl;

    // (4) a stack array, to compare against scalar stack vars and the heap
    char buf[16];
    std::cout << "stack array buf: "
              << static_cast<void*>(buf) << std::endl;

    delete[] p2;

    fun1();

    delete[] p; // Don't forget to free the allocated memory

    return 0;
}