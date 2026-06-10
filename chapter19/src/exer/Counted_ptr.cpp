/**
 * @file Counted_ptr.cpp
 * @author KaKaRot
 * @brief counted_ptr<T> 测试（参考 PPP 题目：作为函数参数、容器元素等场景）
 * @version 0.1
 * @date 2026-06-10
 *
 * @copyright Copyright (c) 2026
 *
 * 编译运行（在 chapter19 目录下）：
 *   g++ -std=c++17 -g -fsanitize=address,undefined \
 *       -I include/exer src/exer/Counted_ptr.cpp -o /tmp/cptr_test && /tmp/cptr_test
 */

#include "Counted_ptr.h"
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

// 跟踪析构次数
static int g_destroy_count = 0;
struct Tracker {
    int val;
    Tracker(int v) : val(v) {}
    ~Tracker() { ++g_destroy_count; }
};

// ============ 测试 1：基本构造与引用计数 ============
void test_basic() {
    std::cout << "[Test 1] 基本构造与引用计数\n";

    counted_ptr<int> empty;
    check(!empty,                "默认构造：bool = false");
    check(empty.use_count() == 0,"默认构造：use_count = 0");

    counted_ptr<int> p(new int(42));
    check(bool(p),               "持有对象：bool = true");
    check(p.use_count() == 1,    "初始引用计数 = 1");
    check(*p == 42,              "*p = 42");
}

// ============ 测试 2：拷贝构造共享所有权 ============
void test_copy_ctor() {
    std::cout << "[Test 2] 拷贝构造共享所有权\n";

    counted_ptr<int> a(new int(10));
    counted_ptr<int> b(a);   // 共享

    check(a.use_count() == 2, "拷贝后 a 计数 = 2");
    check(b.use_count() == 2, "拷贝后 b 计数 = 2");
    check(a.get() == b.get(), "a 和 b 指向同一对象");

    *b = 99;
    check(*a == 99, "通过 b 修改，a 也看到变化（共享）");
}

// ============ 测试 3：拷贝赋值 ============
void test_copy_assign() {
    std::cout << "[Test 3] 拷贝赋值\n";

    g_destroy_count = 0;
    counted_ptr<Tracker> a(new Tracker(1));
    counted_ptr<Tracker> b(new Tracker(2));

    b = a;   // b 放弃旧对象（计数降到0 → 析构），共享 a 的对象
    check(g_destroy_count == 1, "赋值时 b 原对象被析构");
    check(a.use_count() == 2,   "赋值后计数 = 2");
    check(b->val == 1,          "b 现在指向 a 的对象");
}

// ============ 测试 4：析构减少计数，最后一个析构时释放 ============
void test_destructor() {
    std::cout << "[Test 4] 析构减少计数\n";

    g_destroy_count = 0;
    {
        counted_ptr<Tracker> a(new Tracker(1));
        {
            counted_ptr<Tracker> b(a);
            counted_ptr<Tracker> c(a);
            check(a.use_count() == 3, "三个共享者：计数 = 3");
            check(g_destroy_count == 0, "对象未析构");
        }   // b, c 析构，计数降到 1
        check(a.use_count() == 1, "b,c 离开后：计数 = 1");
        check(g_destroy_count == 0, "a 还在，对象未析构");
    }   // a 析构，计数降到 0 → 释放
    check(g_destroy_count == 1, "最后一个离开后对象被析构");
}

// ============ 测试 5：移动构造和移动赋值 ============
void test_move() {
    std::cout << "[Test 5] 移动语义\n";

    counted_ptr<int> a(new int(7));
    counted_ptr<int> b(std::move(a));   // 移动，计数不变

    check(!a,                  "移动后 a 为空");
    check(b.use_count() == 1,  "移动构造不改变计数");
    check(*b == 7,             "移动后值正确");

    counted_ptr<int> c(new int(99));
    g_destroy_count = 0;
    counted_ptr<Tracker> t1(new Tracker(1));
    counted_ptr<Tracker> t2(new Tracker(2));
    t2 = std::move(t1);
    check(g_destroy_count == 1, "移动赋值时旧对象被析构");
    check(t1 == nullptr,        "移动赋值后 t1 为空");
    check(t2->val == 1,         "移动赋值后 t2 持有原对象");
}

// ============ 测试 6：reset ============
void test_reset() {
    std::cout << "[Test 6] reset\n";

    g_destroy_count = 0;
    counted_ptr<Tracker> a(new Tracker(1));
    counted_ptr<Tracker> b(a);   // 计数 = 2

    a.reset();   // a 放弃，计数降到 1，不释放
    check(g_destroy_count == 0, "reset 后计数=1，对象未析构");
    check(!a,                   "reset 后 a 为空");
    check(b.use_count() == 1,   "b 计数降到 1");

    b.reset();   // 最后一个放弃，计数降到 0 → 释放
    check(g_destroy_count == 1, "最后一个 reset 后对象析构");
}

// ============ 测试 7：作为函数参数传递（题目场景）============

// 按值传递：计数 +1，函数返回后 -1
int get_count_in_func(counted_ptr<int> p) {
    return (int)p.use_count();
}

// 按引用传递：不改变计数
void modify_via_ref(counted_ptr<int>& p, int new_val) {
    *p = new_val;
}

void test_as_argument() {
    std::cout << "[Test 7] 作为函数参数\n";

    counted_ptr<int> p(new int(10));
    check(p.use_count() == 1, "调用前计数 = 1");

    // 按值传递：函数内计数 = 2
    int cnt = get_count_in_func(p);
    check(cnt == 2,            "按值传入时函数内计数 = 2");
    check(p.use_count() == 1,  "函数返回后计数恢复 = 1");

    // 按引用传递：计数不变，但能修改对象
    modify_via_ref(p, 999);
    check(*p == 999,           "通过 const ref 修改对象成功");
    check(p.use_count() == 1,  "按引用传递计数不变");
}

// ============ 测试 8：作为容器元素（题目场景）============
void test_in_container() {
    std::cout << "[Test 8] 作为容器元素\n";

    g_destroy_count = 0;
    std::vector<counted_ptr<Tracker>> v;

    // push_back：拷贝进容器，计数 +1
    counted_ptr<Tracker> p(new Tracker(42));
    v.push_back(p);
    check(p.use_count() == 2,   "push_back 后计数 = 2");

    // 容器里多个元素共享同一对象
    v.push_back(p);
    check(p.use_count() == 3,   "第二次 push_back 后计数 = 3");

    // 通过容器修改共享对象
    v[0]->val = 100;
    check(p->val == 100,        "通过容器修改共享对象，p 也看到变化");

    // 清空容器，计数减少
    v.clear();
    check(p.use_count() == 1,   "容器清空后计数 = 1");
    check(g_destroy_count == 0, "p 还存在，对象未析构");

    // 使用 emplace_back（移动语义）
    v.emplace_back(std::move(p));
    check(!p,                   "move 进容器后 p 为空");
    check(v[0].use_count() == 1,"容器持有唯一所有权，计数 = 1");
}

// ============ 测试 9：容器 sort（需要 operator< 或自定义比较）============
void test_container_sort() {
    std::cout << "[Test 9] 容器内排序\n";

    std::vector<counted_ptr<int>> v;
    v.push_back(counted_ptr<int>(new int(3)));
    v.push_back(counted_ptr<int>(new int(1)));
    v.push_back(counted_ptr<int>(new int(2)));

    std::sort(v.begin(), v.end(), [](const counted_ptr<int>& a, const counted_ptr<int>& b){
        return *a < *b;
    });

    check(*v[0]==1 && *v[1]==2 && *v[2]==3, "排序后 1,2,3 顺序正确");
}

// ============ 测试 10：make_counted ============
void test_make_counted() {
    std::cout << "[Test 10] make_counted\n";

    auto p = make_counted<int>(55);
    check(bool(p),            "make_counted 返回非空");
    check(*p == 55,           "make_counted 构造值正确");
    check(p.use_count() == 1, "make_counted 计数 = 1");

    struct Point { int x, y; };
    auto pp = make_counted<Point>(Point{3, 4});
    check(pp->x==3 && pp->y==4, "make_counted 聚合构造正确");
}

// ============ 测试 11：空指针的安全性（原版 bug 验证）============
void test_null_safety() {
    std::cout << "[Test 11] 空指针安全性（原版 bug 验证）\n";

    // 原版 Free() 里 --*count 不检查 nullptr → 崩溃
    // 修完后这些操作全部安全
    counted_ptr<int> a;       // 默认构造
    counted_ptr<int> b(a);    // 拷贝空指针
    b = a;                    // 赋值空指针
    counted_ptr<int> c(std::move(a));  // 移动空指针
    check(!b && !c, "空指针的拷贝/赋值/移动全部安全");

    // nullptr 比较
    counted_ptr<int> p;
    check(p == nullptr, "空 counted_ptr == nullptr");
    p = make_counted<int>(1);
    check(p != nullptr, "非空 counted_ptr != nullptr");
}

int main()
{
    std::cout << "===== counted_ptr<T> 测试开始 =====\n";

    test_basic();
    test_copy_ctor();
    test_copy_assign();
    test_destructor();
    test_move();
    test_reset();
    test_as_argument();
    test_in_container();
    test_container_sort();
    test_make_counted();
    test_null_safety();

    std::cout << "\n===== 测试结束 =====\n";
    std::cout << "PASS: " << g_pass << "   FAIL: " << g_fail << "\n";
    return g_fail == 0 ? 0 : 1;
}
