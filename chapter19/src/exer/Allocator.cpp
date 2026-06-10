/**
 * @file Allocator.cpp
 * @author KaKaRot
 * @brief Allocator<T> + KaKaRot::vector<T> 测试（PPP §19.4）
 * @version 0.1
 * @date 2026-06-10
 *
 * @copyright Copyright (c) 2026
 *
 * 编译运行（在 chapter19 目录下）：
 *   g++ -std=c++17 -g -fsanitize=address,undefined \
 *       -I include/exer src/exer/Allocator.cpp -o /tmp/alloc_test && /tmp/alloc_test
 */

#include "Allocator.h"
#include <iostream>
#include <string>

// ---- 极简测试框架 ----
static int g_pass = 0, g_fail = 0;
void check(bool cond, const std::string& msg) {
    if (cond) { ++g_pass; }
    else       { ++g_fail; std::cout << "  [FAIL] " << msg << "\n"; }
}

// ============ 测试 1：Allocator 直接使用 ============
void test_allocator_basic() {
    std::cout << "[Test 1] Allocator 直接使用\n";
    KaKaRot::Allocator<int> a;

    // allocate：只拿内存，不构造
    int* p = a.allocate(3);
    check(p != nullptr, "allocate 返回非空");

    // construct：在原始内存上构造
    a.construct(&p[0], 10);
    a.construct(&p[1], 20);
    a.construct(&p[2], 30);
    check(p[0]==10 && p[1]==20 && p[2]==30, "construct 后值正确");

    // destroy：析构，不释放
    a.destroy(&p[0]);
    a.destroy(&p[1]);
    a.destroy(&p[2]);

    // deallocate：释放内存
    a.deallocate(p, 3);
    check(true, "destroy + deallocate 无崩溃");
}

// ============ 测试 2：Allocator 用于 string（有析构的类型）============
void test_allocator_string() {
    std::cout << "[Test 2] Allocator<string>\n";
    KaKaRot::Allocator<std::string> a;

    std::string* p = a.allocate(2);
    a.construct(&p[0], "hello");
    a.construct(&p[1], std::string("world"));
    check(p[0] == "hello", "string construct 左值正确");
    check(p[1] == "world", "string construct 右值正确");

    a.destroy(&p[0]);
    a.destroy(&p[1]);
    a.deallocate(p, 2);
    check(true, "string destroy + deallocate 无泄漏");
}

// ============ 测试 3：vector 默认构造 ============
void test_vector_default() {
    std::cout << "[Test 3] vector 默认构造\n";

    KaKaRot::vector<int> v;
    check(v.size()     == 0, "默认构造 size=0");
    check(v.capacity() == 0, "默认构造 capacity=0");
}

// ============ 测试 4：vector 带初值构造 ============
void test_vector_init() {
    std::cout << "[Test 4] vector 带初值构造\n";

    KaKaRot::vector<int> v(5, 42);
    check(v.size()     == 5,  "size=5");
    check(v.capacity() == 5,  "capacity=5");
    for (size_t i = 0; i < v.size(); ++i)
        check(v[i] == 42, "v[" + std::to_string(i) + "]=42");
}

// ============ 测试 5：push_back + 自动扩容 ============
void test_vector_push_back() {
    std::cout << "[Test 5] push_back + 扩容\n";

    KaKaRot::vector<int> v;
    for (int i = 0; i < 10; ++i)
        v.push_back(i);

    check(v.size() == 10, "push_back 10 次后 size=10");
    check(v.capacity() >= 10, "capacity >= size");
    for (int i = 0; i < 10; ++i)
        check(v[i] == i, "v[" + std::to_string(i) + "]=" + std::to_string(i));
}

// ============ 测试 6：reserve ============
void test_vector_reserve() {
    std::cout << "[Test 6] reserve\n";

    KaKaRot::vector<int> v;
    v.reserve(100);
    check(v.capacity() >= 100, "reserve(100) 后 capacity>=100");
    check(v.size() == 0, "reserve 不改变 size");

    // reserve 比当前 capacity 小，不缩减
    size_t cap_before = v.capacity();
    v.reserve(10);
    check(v.capacity() == cap_before, "reserve 小值不缩减 capacity");
}

// ============ 测试 7：拷贝构造 ============
void test_vector_copy_ctor() {
    std::cout << "[Test 7] 拷贝构造\n";

    KaKaRot::vector<int> a;
    for (int i = 0; i < 5; ++i) a.push_back(i);

    KaKaRot::vector<int> b(a);
    check(b.size() == 5, "拷贝后 size=5");
    for (int i = 0; i < 5; ++i)
        check(b[i] == i, "拷贝后 b[" + std::to_string(i) + "] 正确");

    // 修改 a 不影响 b（值语义）
    a[0] = 999;
    check(b[0] == 0, "修改原对象不影响副本");
}

// ============ 测试 8：拷贝赋值 ============
void test_vector_copy_assign() {
    std::cout << "[Test 8] 拷贝赋值\n";

    KaKaRot::vector<int> a, b;
    for (int i = 0; i < 5; ++i) a.push_back(i * 10);
    for (int i = 0; i < 3; ++i) b.push_back(i);

    b = a;
    check(b.size() == 5, "赋值后 size=5");
    for (int i = 0; i < 5; ++i)
        check(b[i] == i*10, "赋值后 b[" + std::to_string(i) + "] 正确");
}

// ============ 测试 9：vector<string>（有析构的类型）============
void test_vector_string() {
    std::cout << "[Test 9] vector<string>\n";

    KaKaRot::vector<std::string> v;
    v.push_back("hello");
    v.push_back("world");
    v.push_back("!");

    check(v.size() == 3,         "size=3");
    check(v[0] == "hello",       "v[0]=hello");
    check(v[1] == "world",       "v[1]=world");
    check(v[2] == "!",           "v[2]=!");

    KaKaRot::vector<std::string> v2(v);
    check(v2[0] == "hello",      "拷贝后 v2[0]=hello");
    v.push_back("extra");
    check(v2.size() == 3,        "拷贝后 push_back 原 vector 不影响副本");
}

// ============ 测试 10：迭代器 ============
void test_vector_iterator() {
    std::cout << "[Test 10] 迭代器\n";

    KaKaRot::vector<int> v;
    for (int i = 1; i <= 5; ++i) v.push_back(i);

    int sum = 0;
    for (auto it = v.begin(); it != v.end(); ++it)
        sum += *it;
    check(sum == 15, "迭代器遍历求和=15");

    // range-for
    sum = 0;
    for (int x : v) sum += x;
    check(sum == 15, "range-for 遍历求和=15");
}

int main()
{
    std::cout << "===== Allocator + vector 测试开始 =====\n";

    test_allocator_basic();
    test_allocator_string();
    test_vector_default();
    test_vector_init();
    test_vector_push_back();
    test_vector_reserve();
    test_vector_copy_ctor();
    test_vector_copy_assign();
    test_vector_string();
    test_vector_iterator();

    std::cout << "\n===== 测试结束 =====\n";
    std::cout << "PASS: " << g_pass << "   FAIL: " << g_fail << "\n";
    return g_fail == 0 ? 0 : 1;
}
