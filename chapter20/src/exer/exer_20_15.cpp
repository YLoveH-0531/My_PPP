/**
 * @file exer_20_15.cpp
 * @author KaKaRot
 * @brief
 * @version 0.1
 * @date 2026-07-09
 *
 * @copyright Copyright (c) 2026
 */

#include "exer_20_15.h"

#include <cassert>
#include <iostream>
#include <string>

// 辅助：打印测试结果
void check(const char *name, bool ok) 
{
    std::cout << (ok ? "[PASS] " : "[FAIL] ") << name << '\n';
}

// ── 测试用类型 ────────────────────────────────────────────────
struct Animal {
    std::string name;
    int age;
    Animal(std::string n, int a) : name{std::move(n)}, age{a} {
    }
};

// ── 测试函数 ──────────────────────────────────────────────────

void test_push_and_size() {
    Pvector<int> pv;
    pv.push_back(new int(10));
    pv.push_back(new int(20));
    pv.push_back(new int(30));
    check("push_back / size == 3", pv.size() == 3);
}

void test_index_operator() {
    Pvector<int> pv;
    pv.push_back(new int(42));
    pv.push_back(new int(99));
    check("operator[] [0] == 42", *pv[0] == 42);
    check("operator[] [1] == 99", *pv[1] == 99);
}

void test_empty() {
    Pvector<int> pv;
    check("empty() on default-constructed", pv.empty());
    pv.push_back(new int(1));
    check("not empty() after push_back", !pv.empty());
}

void test_range_for() {
    Pvector<int> pv;
    pv.push_back(new int(1));
    pv.push_back(new int(2));
    pv.push_back(new int(3));

    int sum = 0;
    for (auto p : pv)
        sum += *p;
    check("range-for sum == 6", sum == 6);
}

void test_reserve() {
    Pvector<int> pv;
    pv.reserve(100);
    // reserve 不改变 size
    check("reserve does not change size", pv.size() == 0);
    pv.push_back(new int(7));
    check("push_back after reserve", pv.size() == 1);
}

void test_struct_elements() {
    Pvector<Animal> pv;
    pv.push_back(new Animal("cat", 3));
    pv.push_back(new Animal("dog", 5));
    check("struct: size == 2", pv.size() == 2);
    check("struct: [0].name == cat", pv[0]->name == "cat");
    check("struct: [1].age  == 5", pv[1]->age == 5);
}

void test_destructor_no_leak() {
    // 用一个作用域触发析构，valgrind 下跑可验证无泄漏
    {
        Pvector<int> pv;
        for (int i = 0; i < 5; ++i)
            pv.push_back(new int(i));
    } // ~Pvector 在此触发
    check("destructor called without crash", true);
}

// ── 私有继承保护（编译期验证，取消注释会编译失败）────────────
// void test_private_base_conversion()
// {
//     Pvector<int> pv;
//     std::vector<int*>* p = &pv;   // ERROR: inaccessible base
// }

int main() 
{
    std::cout << "=== Pvector<T> Tests ===\n\n";

    test_push_and_size();
    test_index_operator();
    test_empty();
    test_range_for();
    test_reserve();
    test_struct_elements();
    test_destructor_no_leak();

    std::cout << "\nDone.\n";
    return 0;
}
