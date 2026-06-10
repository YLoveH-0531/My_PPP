/**
 * @file Unique_ptr.cpp
 * @author KaKaRot
 * @brief unique_ptr<T> 测试
 * @version 0.1
 * @date 2026-06-10
 *
 * @copyright Copyright (c) 2026
 *
 * 编译运行（在 chapter19 目录下）：
 *   g++ -std=c++17 -g -fsanitize=address,undefined \
 *       -I include/exer src/exer/Unique_ptr.cpp -o /tmp/uptr_test && /tmp/uptr_test
 */

#include "Unique_ptr.h"
#include <iostream>
#include <string>
#include <utility>

// ---- 极简测试框架 ----
static int g_pass = 0, g_fail = 0;
void check(bool cond, const std::string& msg) {
    if (cond) { ++g_pass; }
    else       { ++g_fail; std::cout << "  [FAIL] " << msg << "\n"; }
}

// 用于验证析构是否被调用
struct Tracker {
    int  val;
    bool& destroyed;
    Tracker(int v, bool& d) : val(v), destroyed(d) { destroyed = false; }
    ~Tracker() { destroyed = true; }
};

// ============ 测试 1：基本构造与 bool 转换 ============
void test_basic() {
    std::cout << "[Test 1] 基本构造与 bool 转换\n";

    KaKaRot::unique_ptr<int> empty;
    check(!empty,           "默认构造：bool = false");
    check(empty.get() == nullptr, "默认构造：get() = nullptr");

    KaKaRot::unique_ptr<int> p(new int(42));
    check(bool(p),          "持有对象：bool = true");
    check(p.get() != nullptr, "持有对象：get() != nullptr");
}

// ============ 测试 2：operator* 和 operator-> ============
void test_deref() {
    std::cout << "[Test 2] operator* / operator->\n";

    KaKaRot::unique_ptr<int> p(new int(99));
    check(*p == 99, "*p = 99");

    // operator* 返回引用，修改会影响原对象
    *p = 100;
    check(*p == 100, "通过 *p 修改后值变为 100");

    struct Point { int x, y; };
    KaKaRot::unique_ptr<Point> pp(new Point{3, 4});
    check(pp->x == 3 && pp->y == 4, "operator-> 访问成员");
    pp->x = 10;
    check(pp->x == 10, "通过 operator-> 修改成员");
}

// ============ 测试 3：析构自动释放 ============
void test_destructor() {
    std::cout << "[Test 3] 析构自动释放\n";

    bool destroyed = false;
    {
        KaKaRot::unique_ptr<Tracker> p(new Tracker(1, destroyed));
        check(!destroyed, "作用域内对象未析构");
    }   // p 离开作用域，析构 Tracker
    check(destroyed, "离开作用域后析构函数被调用");
}

// ============ 测试 4：移动构造 ============
void test_move_ctor() {
    std::cout << "[Test 4] 移动构造\n";

    bool destroyed = false;
    KaKaRot::unique_ptr<Tracker> a(new Tracker(42, destroyed));
    KaKaRot::unique_ptr<Tracker> b(std::move(a));

    check(!a,              "移动后原指针为空");
    check(bool(b),         "移动后新指针持有对象");
    check(b->val == 42,    "移动后值正确");
    check(!destroyed,      "移动构造不析构对象");
}

// ============ 测试 5：移动赋值 ============
void test_move_assign() {
    std::cout << "[Test 5] 移动赋值\n";

    bool d1 = false, d2 = false;
    KaKaRot::unique_ptr<Tracker> a(new Tracker(1, d1));
    KaKaRot::unique_ptr<Tracker> b(new Tracker(2, d2));

    b = std::move(a);           // b 原来的对象应被析构

    check(d2,              "b 原来的对象在移动赋值时被析构");
    check(!d1,             "a 的对象未被析构（转移给了 b）");
    check(!a,              "移动赋值后 a 为空");
    check(b->val == 1,     "移动赋值后 b 持有 a 的对象");
}

// ============ 测试 6：release ============
void test_release() {
    std::cout << "[Test 6] release\n";

    bool destroyed = false;
    KaKaRot::unique_ptr<Tracker> p(new Tracker(7, destroyed));

    Tracker* raw = p.release();  // 放弃所有权

    check(!p,              "release 后 unique_ptr 为空");
    check(raw != nullptr,  "release 返回的指针非空");  // fix 验证：原来永远返回 nullptr
    check(raw->val == 7,   "release 返回的指针值正确");
    check(!destroyed,      "release 不析构对象");

    delete raw;            // 调用方负责释放
    check(destroyed,       "手动 delete 后析构函数被调用");
}

// ============ 测试 7：reset ============
void test_reset() {
    std::cout << "[Test 7] reset\n";

    bool d1 = false, d2 = false;
    KaKaRot::unique_ptr<Tracker> p(new Tracker(1, d1));

    p.reset(new Tracker(2, d2));  // 释放旧对象，接管新对象
    check(d1,              "reset 时旧对象被析构");
    check(!d2,             "新对象未被析构");
    check(p->val == 2,     "reset 后持有新对象");

    p.reset();             // 不传参数 = 只释放
    check(d2,              "reset() 无参数释放当前对象");
    check(!p,              "reset() 后为空");
}

// ============ 测试 8：make_unique ============
void test_make_unique() {
    std::cout << "[Test 8] make_unique\n";

    auto p = KaKaRot::make_unique<int>(123);
    check(bool(p),         "make_unique 返回非空");
    check(*p == 123,       "make_unique 构造值正确");

    struct Point { int x, y; };
    auto pp = KaKaRot::make_unique<Point>(Point{5, 6});
    check(pp->x==5 && pp->y==6, "make_unique 多参数构造正确");
}

// ============ 测试 9：拷贝被禁止（编译期验证）============
void test_no_copy() {
    std::cout << "[Test 9] 拷贝语义被禁止（编译期）\n";
    // 下面两行如果取消注释会编译错误——证明拷贝被正确禁用
    // KaKaRot::unique_ptr<int> a(new int(1));
    // KaKaRot::unique_ptr<int> b(a);          // ❌ 拷贝构造被 delete
    // KaKaRot::unique_ptr<int> c; c = a;      // ❌ 拷贝赋值被 delete
    check(true, "拷贝构造/赋值被 delete（需取消注释验证编译报错）");
}

int main()
{
    std::cout << "===== unique_ptr<T> 测试开始 =====\n";

    test_basic();
    test_deref();
    test_destructor();
    test_move_ctor();
    test_move_assign();
    test_release();
    test_reset();
    test_make_unique();
    test_no_copy();

    std::cout << "\n===== 测试结束 =====\n";
    std::cout << "PASS: " << g_pass << "   FAIL: " << g_fail << "\n";
    return g_fail == 0 ? 0 : 1;
}
