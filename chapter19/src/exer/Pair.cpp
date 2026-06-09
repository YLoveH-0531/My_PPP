/**
 * @file Pair.cpp
 * @author KaKaRot
 * @brief Pair<T,U> 测试
 * @version 0.1
 * @date 2026-06-09
 *
 * @copyright Copyright (c) 2026
 *
 * 编译运行（在 chapter19 目录下）：
 *   g++ -std=c++17 -g -fsanitize=address,undefined \
 *       -I include/exer src/exer/Pair.cpp -o /tmp/pair_test && /tmp/pair_test
 */

#include "Pair.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// ---- 极简测试框架 ----
static int g_pass = 0, g_fail = 0;
void check(bool cond, const std::string& msg) {
    if (cond) { ++g_pass; }
    else       { ++g_fail; std::cout << "  [FAIL] " << msg << "\n"; }
}

// ============ 测试 1：默认构造 ============
void test_default_ctor() {
    std::cout << "[Test 1] 默认构造\n";

    KaKaRot::Pair<int, double> p;
    check(p.first  == 0,   "int 默认构造 = 0");
    check(p.second == 0.0, "double 默认构造 = 0.0");

    KaKaRot::Pair<std::string, int> ps;
    check(ps.first.empty(), "string 默认构造 = 空串");
    check(ps.second == 0,   "int 默认构造 = 0");
}

// ============ 测试 2：带参构造（左值）============
void test_value_ctor() {
    std::cout << "[Test 2] 带参构造（左值）\n";

    KaKaRot::Pair<int, std::string> p{42, "hello"};
    check(p.first  == 42,      "带参构造 int 正确");
    check(p.second == "hello", "带参构造 string 正确");

    KaKaRot::Pair<double, char> p2{3.14, 'A'};
    check(p2.first  == 3.14, "带参构造 double 正确");
    check(p2.second == 'A',  "带参构造 char 正确");
}

// ============ 测试 3：带参构造（右值）============
void test_rvalue_ctor() {
    std::cout << "[Test 3] 带参构造（右值）\n";

    // 传临时对象（右值），应走移动而非拷贝
    KaKaRot::Pair<std::string, std::string> p{std::string("move_me"), std::string("too")};
    check(p.first  == "move_me", "右值带参构造 first 正确");
    check(p.second == "too",     "右值带参构造 second 正确");
}

// ============ 测试 4：拷贝构造 ============
void test_copy_ctor() {
    std::cout << "[Test 4] 拷贝构造\n";

    KaKaRot::Pair<int, std::string> a{10, "world"};
    KaKaRot::Pair<int, std::string> b(a);

    check(b.first  == 10,      "拷贝构造后 first 正确");
    check(b.second == "world", "拷贝构造后 second 正确");

    a.first  = 999;
    a.second = "changed";
    check(b.first  == 10,      "修改原对象后副本 first 不变");
    check(b.second == "world", "修改原对象后副本 second 不变");
}

// ============ 测试 5：拷贝赋值 ============
void test_copy_assign() {
    std::cout << "[Test 5] 拷贝赋值\n";

    KaKaRot::Pair<int, double> a{1, 1.1};
    KaKaRot::Pair<int, double> b{2, 2.2};
    b = a;
    check(b.first  == 1,   "赋值后 first 正确");
    check(b.second == 1.1, "赋值后 second 正确");
}

// ============ 测试 6：移动构造 / 移动赋值 ============
void test_move() {
    std::cout << "[Test 6] 移动构造 / 移动赋值\n";

    KaKaRot::Pair<std::string, int> a{"hello", 42};
    KaKaRot::Pair<std::string, int> b(std::move(a));
    check(b.first == "hello" && b.second == 42, "移动构造后新对象值正确");

    KaKaRot::Pair<std::string, int> c{"world", 99};
    c = std::move(b);
    check(c.first == "hello" && c.second == 42, "移动赋值后值正确");
}

// ============ 测试 7：operator== / != ============
void test_comparison_eq() {
    std::cout << "[Test 7] operator== / operator!=\n";

    KaKaRot::Pair<int, std::string> a{1, "a"};
    KaKaRot::Pair<int, std::string> b{1, "a"};
    KaKaRot::Pair<int, std::string> c{2, "b"};

    check(a == b,    "相同值 == true");
    check(!(a == c), "不同值 == false");
    check(a != c,    "不同值 != true");
    check(!(a != b), "相同值 != false");
}

// ============ 测试 8：operator< 字典序 ============
void test_less() {
    std::cout << "[Test 8] operator< 字典序\n";

    KaKaRot::Pair<int, int> a{1, 2};
    KaKaRot::Pair<int, int> b{1, 3};   // first 相同，second 更大
    KaKaRot::Pair<int, int> c{2, 0};   // first 更大

    check(a < b,    "{1,2} < {1,3}");
    check(a < c,    "{1,2} < {2,0}");
    check(!(c < a), "{2,0} 不小于 {1,2}");
    check(!(a < a), "自身不小于自身");

    // 能放进 sort
    std::vector<KaKaRot::Pair<int,int>> v{{3,1},{1,2},{1,1},{2,5}};
    std::sort(v.begin(), v.end());
    check(v[0].first==1 && v[0].second==1, "sort 后第1个 = {1,1}");
    check(v[1].first==1 && v[1].second==2, "sort 后第2个 = {1,2}");
    check(v[2].first==2 && v[2].second==5, "sort 后第3个 = {2,5}");
    check(v[3].first==3 && v[3].second==1, "sort 后第4个 = {3,1}");
}

// ============ 测试 9：make_pair ============
void test_make_pair() {
    std::cout << "[Test 9] make_pair\n";

    auto p1 = KaKaRot::make_pair(1, std::string("hi"));
    check(p1.first == 1 && p1.second == "hi", "make_pair 左值正确");

    auto p2 = KaKaRot::make_pair(std::string("rval"), 3.14);
    check(p2.first == "rval" && p2.second == 3.14, "make_pair 右值正确");

    // 类型推导正确
    auto p3 = KaKaRot::make_pair(42, 'Z');
    check(p3.first == 42 && p3.second == 'Z', "make_pair int+char 类型推导正确");
}

int main()
{
    std::cout << "===== Pair<T,U> 测试开始 =====\n";

    test_default_ctor();
    test_value_ctor();
    test_rvalue_ctor();
    test_copy_ctor();
    test_copy_assign();
    test_move();
    test_comparison_eq();
    test_less();
    test_make_pair();

    std::cout << "\n===== 测试结束 =====\n";
    std::cout << "PASS: " << g_pass << "   FAIL: " << g_fail << "\n";
    return g_fail == 0 ? 0 : 1;
}
