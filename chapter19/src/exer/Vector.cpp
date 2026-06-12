/**
 * @file Vector.cpp
 * @author KaKaRot
 * @brief KaKaRot::vector<T> 测试（PPP §19 自定义 vector）
 * @version 0.1
 * @date 2026-06-12
 *
 * @copyright Copyright (c) 2026
 *
 * 编译运行（在 chapter19 目录下）：
 *   g++ -std=c++17 -g -fsanitize=address,undefined \
 *       -I include/exer src/exer/Vector.cpp -o /tmp/vector_test && /tmp/vector_test
 *
 * 说明：Vector.h 使用 alloc.construct/destroy，仅适用于 C++11/14/17，
 *       请勿用 -std=c++20 编译。
 */

#include "Vector.h"
#include <iostream>
#include <string>
#include <stdexcept>
#include <utility>

using KaKaRot::vector;

// ---- 极简测试框架 ----
static int g_pass = 0, g_fail = 0;
static void check(bool cond, const std::string& msg) {
    if (cond) { ++g_pass; }
    else      { ++g_fail; std::cout << "  [FAIL] " << msg << "\n"; }
}

// ---- 计数类型：跟踪存活实例数与拷贝次数，用于验证无泄漏/无重复析构/移动而非拷贝 ----
struct Counted {
    static int alive;       // 当前存活实例数（构造 +1，析构 -1）
    static int copies;      // 拷贝构造累计次数
    int v;
    Counted(int x = 0) : v(x) { ++alive; }
    Counted(const Counted& o) : v(o.v) { ++alive; ++copies; }
    Counted(Counted&& o) noexcept : v(o.v) { ++alive; o.v = -1; }
    Counted& operator=(const Counted&) = default;
    Counted& operator=(Counted&&) noexcept = default;
    ~Counted() { --alive; }
    bool operator==(const Counted& o) const { return v == o.v; }
    static void reset() { alive = 0; copies = 0; }
};
int Counted::alive = 0;
int Counted::copies = 0;

// ---- 会在第 N 次拷贝时抛异常的类型，用于验证强异常保证 ----
struct Bomb {
    static int copies;      // 拷贝构造累计次数
    static int throw_at;    // 当 copies 达到该值时抛异常（0 表示不抛）
    int v;
    Bomb(int x = 0) : v(x) {}
    Bomb(const Bomb& o) : v(o.v) {
        ++copies;
        if (throw_at && copies >= throw_at) { throw std::runtime_error("boom"); }
    }
    Bomb& operator=(const Bomb&) = default;
    bool operator==(const Bomb& o) const { return v == o.v; }
    static void reset(int t = 0) { copies = 0; throw_at = t; }
};
int Bomb::copies = 0;
int Bomb::throw_at = 0;

// ============ 测试 1：默认构造 ============
void test_default_ctor() {
    std::cout << "[Test 1] 默认构造\n";
    vector<int> v;
    check(v.size() == 0,     "默认构造 size=0");
    check(v.capacity() == 0, "默认构造 capacity=0");
    check(v.empty(),         "默认构造 empty()=true");
    check(v.data() == nullptr, "默认构造 data()=nullptr");
    check(v.begin() == v.end(), "默认构造 begin()==end()");
}

// ============ 测试 2：count + value 构造 ============
void test_count_ctor() {
    std::cout << "[Test 2] count + value 构造\n";
    vector<int> v(5, 42);
    check(v.size() == 5,     "size=5");
    check(v.capacity() == 5, "capacity=5");
    bool all42 = true;
    for (std::size_t i = 0; i < v.size(); ++i) { if (v[i] != 42) all42 = false; }
    check(all42, "全部元素=42");

    // count == 0 不分配
    vector<int> z(0, 7);
    check(z.size() == 0 && z.capacity() == 0, "count=0 不分配");

    // 默认 value
    vector<int> d(3);
    check(d.size() == 3 && d[0] == 0 && d[2] == 0, "默认 value=T{}");
}

// ============ 测试 3：initializer_list 构造 ============
void test_init_list_ctor() {
    std::cout << "[Test 3] initializer_list 构造\n";
    vector<int> v{1, 2, 3, 4};
    check(v.size() == 4,     "size=4");
    check(v.capacity() == 4, "capacity=4");
    check(v[0] == 1 && v[1] == 2 && v[2] == 3 && v[3] == 4, "元素顺序正确");

    vector<int> empty{};
    check(empty.size() == 0, "空 initializer_list size=0");
}

// ============ 测试 4：拷贝构造（值语义 + 保留 capacity）============
void test_copy_ctor() {
    std::cout << "[Test 4] 拷贝构造\n";
    vector<int> a;
    a.reserve(20);
    for (int i = 0; i < 5; ++i) { a.push_back(i); }

    vector<int> b(a);
    check(b.size() == 5,            "拷贝后 size=5");
    check(b.capacity() == a.capacity(), "拷贝保留源 capacity");
    bool same = true;
    for (int i = 0; i < 5; ++i) { if (b[i] != i) same = false; }
    check(same, "元素逐一相等");

    a[0] = 999;
    check(b[0] == 0, "修改源不影响副本（值语义）");

    // 拷贝空 vector
    vector<int> e;
    vector<int> ec(e);
    check(ec.size() == 0 && ec.capacity() == 0, "拷贝空 vector");
}

// ============ 测试 5：拷贝赋值（copy-and-swap + 自赋值）============
void test_copy_assign() {
    std::cout << "[Test 5] 拷贝赋值\n";
    vector<int> a, b;
    for (int i = 0; i < 5; ++i) { a.push_back(i * 10); }
    for (int i = 0; i < 3; ++i) { b.push_back(i); }

    b = a;
    check(b.size() == 5, "赋值后 size=5");
    bool ok = true;
    for (int i = 0; i < 5; ++i) { if (b[i] != i * 10) ok = false; }
    check(ok, "赋值后元素正确");

    // 自赋值安全（经指针别名，避免编译器把字面自赋值优化掉）
    vector<int>* pa = &a;
    a = *pa;
    check(a.size() == 5 && a[2] == 20, "自赋值后内容不变");

    // 赋值给更小/更大的容器都行
    vector<int> c{1};
    c = a;
    check(c.size() == 5, "小容器被赋大容器");
}

// ============ 测试 6：移动构造 ============
void test_move_ctor() {
    std::cout << "[Test 6] 移动构造\n";
    vector<int> a{1, 2, 3};
    int* raw = a.data();
    vector<int> b(std::move(a));
    check(b.size() == 3,        "移动后目标 size=3");
    check(b.data() == raw,      "移动转移了底层指针（无拷贝）");
    check(a.size() == 0,        "移动后源 size=0");
    check(a.capacity() == 0,    "移动后源 capacity=0");
    check(a.data() == nullptr,  "移动后源 data()=nullptr");
}

// ============ 测试 7：移动赋值（含自赋值）============
void test_move_assign() {
    std::cout << "[Test 7] 移动赋值\n";
    vector<int> a{1, 2, 3, 4};
    vector<int> b{9, 9};
    b = std::move(a);
    check(b.size() == 4 && b[3] == 4, "移动赋值后目标正确");
    check(a.size() == 0,              "移动赋值后源被清空");

    // 自移动赋值安全（经指针别名，避免编译器把字面自移动优化掉）
    vector<int> c{5, 6, 7};
    vector<int>* pc = &c;
    c = std::move(*pc);
    check(c.size() == 3 && c[0] == 5, "自移动赋值后内容不变");
}

// ============ 测试 8：元素访问 operator[] / at / front / back / data ============
void test_element_access() {
    std::cout << "[Test 8] 元素访问\n";
    vector<int> v{10, 20, 30};
    check(v[0] == 10 && v[2] == 30, "operator[] 读取");
    v[1] = 99;
    check(v[1] == 99, "operator[] 写入");

    check(v.at(0) == 10, "at 合法下标");
    check(v.front() == 10, "front()");
    check(v.back() == 30,  "back()");
    check(*v.data() == 10, "data() 指向首元素");

    bool threw = false;
    try { v.at(3); } catch (const std::out_of_range&) { threw = true; }
    check(threw, "at 越界抛 out_of_range");

    // const 版本
    const vector<int> cv{1, 2, 3};
    check(cv[1] == 2 && cv.at(2) == 3 && cv.front() == 1 && cv.back() == 3, "const 访问");
}

// ============ 测试 9：push_back（左值/右值）+ 自动扩容 ============
void test_push_back() {
    std::cout << "[Test 9] push_back + 扩容\n";
    vector<int> v;
    for (int i = 0; i < 100; ++i) { v.push_back(i); }
    check(v.size() == 100,    "push_back 100 次后 size=100");
    check(v.capacity() >= 100, "capacity>=100");
    bool ok = true;
    for (int i = 0; i < 100; ++i) { if (v[i] != i) ok = false; }
    check(ok, "元素顺序正确");

    // 右值 push_back
    vector<std::string> s;
    std::string str = "hello";
    s.push_back(std::move(str));
    check(s[0] == "hello", "右值 push_back");

    // 别名安全：push_back(v[0]) 在扩容时不应使用悬垂引用
    vector<int> a;
    a.reserve(1);
    a.push_back(7);
    a.push_back(a[0]);          // size==capacity，触发扩容；value 引用容器内元素
    check(a.size() == 2 && a[1] == 7, "扩容时 push_back 自身元素（别名安全）");
}

// ============ 测试 10：emplace_back ============
void test_emplace_back() {
    std::cout << "[Test 10] emplace_back\n";
    vector<std::string> v;
    v.emplace_back(3, 'x');     // string(3,'x') == "xxx"
    v.emplace_back("abc");
    check(v.size() == 2,    "emplace_back 后 size=2");
    check(v[0] == "xxx",    "emplace_back 多参数构造");
    check(v[1] == "abc",    "emplace_back 单参数构造");
}

// ============ 测试 11：emplace（任意位置插入）============
void test_emplace() {
    std::cout << "[Test 11] emplace\n";
    vector<int> v{1, 2, 4, 5};
    int* pos = v.emplace(v.begin() + 2, 3);     // 中间插入 3 -> {1,2,3,4,5}
    check(v.size() == 5,                "中间插入后 size=5");
    check(*pos == 3,                    "返回指向新元素的指针");
    check(v[0]==1 && v[1]==2 && v[2]==3 && v[3]==4 && v[4]==5, "中间插入顺序正确");

    // 头部插入
    v.emplace(v.begin(), 0);
    check(v[0] == 0 && v[1] == 1, "头部插入");

    // 尾部插入（pos==finish，等价 push_back）
    int* tail = v.emplace(v.end(), 99);
    check(*tail == 99 && v.back() == 99, "尾部插入");

    // 触发扩容的插入仍正确
    vector<int> g;
    for (int i = 0; i < 8; ++i) { g.push_back(i); }   // 填满到默认 capacity
    std::size_t before = g.size();
    g.emplace(g.begin() + 1, -1);
    check(g.size() == before + 1 && g[1] == -1 && g[2] == 1, "扩容时中间插入正确");

    // 越界抛异常
    bool threw = false;
    vector<int> e{1, 2};
    try { e.emplace(e.end() + 5, 0); } catch (const std::out_of_range&) { threw = true; }
    check(threw, "emplace 越界抛 out_of_range");
}

// ============ 测试 12：pop_back（含空容器）============
void test_pop_back() {
    std::cout << "[Test 12] pop_back\n";
    vector<int> v{1, 2, 3};
    v.pop_back();
    check(v.size() == 2 && v.back() == 2, "pop_back 后 size=2");

    v.pop_back();
    v.pop_back();
    check(v.empty(), "全部弹出后为空");

    // 空容器 pop_back 不崩溃（实现保护了 size>0）
    v.pop_back();
    check(v.empty(), "空容器 pop_back 安全 no-op");
}

// ============ 测试 13：resize（扩大/缩小）============
void test_resize() {
    std::cout << "[Test 13] resize\n";
    vector<int> v{1, 2, 3};
    v.resize(5, 7);
    check(v.size() == 5 && v[3] == 7 && v[4] == 7, "扩大用填充值补齐");

    v.resize(2);
    check(v.size() == 2 && v[0] == 1 && v[1] == 2, "缩小截断尾部");

    v.resize(0);
    check(v.empty(), "resize(0) 清空");
}

// ============ 测试 14：clear ============
void test_clear() {
    std::cout << "[Test 14] clear\n";
    vector<int> v{1, 2, 3, 4};
    std::size_t cap = v.capacity();
    v.clear();
    check(v.size() == 0,        "clear 后 size=0");
    check(v.capacity() == cap,  "clear 不改变 capacity");
    v.push_back(42);
    check(v[0] == 42,           "clear 后可继续使用");
}

// ============ 测试 15：reserve ============
void test_reserve() {
    std::cout << "[Test 15] reserve\n";
    vector<int> v{1, 2, 3};
    v.reserve(100);
    check(v.capacity() >= 100, "reserve(100) 后 capacity>=100");
    check(v.size() == 3,       "reserve 不改变 size");
    check(v[0]==1 && v[2]==3,  "reserve 后元素保留");

    std::size_t cap = v.capacity();
    v.reserve(10);
    check(v.capacity() == cap, "reserve 小值不缩减");
}

// ============ 测试 16：迭代器（正向/const/range-for）============
void test_iterators() {
    std::cout << "[Test 16] 迭代器\n";
    vector<int> v{1, 2, 3, 4, 5};

    int sum = 0;
    for (auto it = v.begin(); it != v.end(); ++it) { sum += *it; }
    check(sum == 15, "begin/end 遍历求和=15");

    sum = 0;
    for (int x : v) { sum += x; }
    check(sum == 15, "range-for 求和=15");

    const vector<int> cv{10, 20, 30};
    int csum = 0;
    for (auto it = cv.cbegin(); it != cv.cend(); ++it) { csum += *it; }
    check(csum == 60, "cbegin/cend 遍历求和=60");
}

// ============ 测试 17：反向迭代器 ============
void test_reverse_iterators() {
    std::cout << "[Test 17] 反向迭代器\n";
    vector<int> v{1, 2, 3};
    std::string s;
    for (auto it = v.rbegin(); it != v.rend(); ++it) { s += std::to_string(*it); }
    check(s == "321", "rbegin/rend 逆序遍历=321");

    const vector<int> cv{4, 5, 6};
    std::string cs;
    for (auto it = cv.crbegin(); it != cv.crend(); ++it) { cs += std::to_string(*it); }
    check(cs == "654", "crbegin/crend 逆序遍历=654");
}

// ============ 测试 18：相等比较 operator== / != ============
void test_equality() {
    std::cout << "[Test 18] 相等比较\n";
    vector<int> a{1, 2, 3};
    vector<int> b{1, 2, 3};
    vector<int> c{1, 2, 4};
    vector<int> d{1, 2};
    check(a == b,  "内容相同 -> ==");
    check(a != c,  "内容不同 -> !=");
    check(a != d,  "长度不同 -> !=");
    check(!(a != b), "!= 取反正确");
}

// ============ 测试 19：swap（成员 + 非成员 ADL）============
void test_swap() {
    std::cout << "[Test 19] swap\n";
    vector<int> a{1, 2, 3};
    vector<int> b{9, 8};
    a.swap(b);
    check(a.size() == 2 && a[0] == 9, "成员 swap 后 a 变 b");
    check(b.size() == 3 && b[0] == 1, "成员 swap 后 b 变 a");

    using std::swap;
    swap(a, b);                 // 经 ADL 找到 KaKaRot::swap，O(1) 指针交换
    check(a.size() == 3 && a[0] == 1, "非成员 swap(ADL) 还原");
}

// ============ 测试 20：vector<string>（含析构的类型）============
void test_vector_string() {
    std::cout << "[Test 20] vector<string>\n";
    vector<std::string> v;
    v.push_back("hello");
    v.push_back("world");
    v.emplace_back("!");
    check(v.size() == 3 && v[0] == "hello" && v[2] == "!", "string 元素正确");

    vector<std::string> v2(v);
    v.push_back("extra");
    check(v2.size() == 3, "拷贝后改源不影响副本");
}

// ============ 测试 21：无泄漏/无重复析构（计数类型）============
void test_no_leak() {
    std::cout << "[Test 21] 实例计数（泄漏/重复析构检测）\n";
    Counted::reset();
    {
        vector<Counted> v;
        for (int i = 0; i < 50; ++i) { v.push_back(Counted(i)); }  // 触发多次扩容
        check(Counted::alive == 50, "存活实例=50");

        vector<Counted> copy(v);            // 拷贝
        check(Counted::alive == 100, "拷贝后存活=100");

        v.resize(10);                       // 缩小，析构 40 个
        check(Counted::alive == 60, "resize 缩小后存活=60");

        v.clear();                          // 析构剩余 10 个
        check(Counted::alive == 50, "clear 后仅剩副本的 50");
    }
    check(Counted::alive == 0, "全部析构，无泄漏/无重复析构");
}

// ============ 测试 22：扩容用移动而非拷贝（move_if_noexcept）============
void test_move_on_grow() {
    std::cout << "[Test 22] 扩容使用移动\n";
    Counted::reset();
    vector<Counted> v;
    v.reserve(2);
    v.push_back(Counted(1));
    v.push_back(Counted(2));
    int copies_before = Counted::copies;
    v.push_back(Counted(3));                // size==cap，触发扩容搬移
    // Counted 的移动构造 noexcept，故 reserve 应移动而非拷贝旧元素
    check(Counted::copies == copies_before, "扩容时移动旧元素，未发生拷贝");
    check(v.size() == 3 && v[0].v == 1 && v[2].v == 3, "扩容后内容正确");
}

// ============ 测试 23：拷贝赋值的强异常保证 ============
void test_strong_exception_guarantee() {
    std::cout << "[Test 23] 拷贝赋值强异常保证\n";
    vector<Bomb> dst;
    for (int i = 0; i < 3; ++i) { dst.push_back(Bomb(i)); }   // dst = {0,1,2}

    vector<Bomb> src;
    Bomb::reset(0);
    for (int i = 0; i < 4; ++i) { src.push_back(Bomb(100 + i)); }

    // 令拷贝 src（copy-and-swap 构造 tmp）时在第 2 次拷贝抛异常
    Bomb::reset(2);
    bool threw = false;
    try {
        dst = src;              // tmp(src) 拷贝途中抛异常
    } catch (const std::runtime_error&) {
        threw = true;
    }
    check(threw, "拷贝赋值途中确实抛出异常");
    // 强保证：dst 完全不受影响
    check(dst.size() == 3 && dst[0].v == 0 && dst[2].v == 2, "异常后 dst 保持原样（强异常保证）");
    Bomb::reset(0);             // 复位，避免影响后续析构
}

int main()
{
    std::cout << "===== KaKaRot::vector 测试开始 =====\n";

    test_default_ctor();
    test_count_ctor();
    test_init_list_ctor();
    test_copy_ctor();
    test_copy_assign();
    test_move_ctor();
    test_move_assign();
    test_element_access();
    test_push_back();
    test_emplace_back();
    test_emplace();
    test_pop_back();
    test_resize();
    test_clear();
    test_reserve();
    test_iterators();
    test_reverse_iterators();
    test_equality();
    test_swap();
    test_vector_string();
    test_no_leak();
    test_move_on_grow();
    test_strong_exception_guarantee();

    std::cout << "\n===== 测试结束 =====\n";
    std::cout << "PASS: " << g_pass << "   FAIL: " << g_fail << "\n";
    return g_fail == 0 ? 0 : 1;
}
