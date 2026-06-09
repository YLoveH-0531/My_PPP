/**
 * @file drill_19_1.cpp
 * @author KaKaRot
 * @brief drill 19.1 — S<T> + vector<T> IO operator 测试
 * @version 0.1
 * @date 2026-06-08
 *
 * @copyright Copyright (c) 2026
 *
 * 编译运行（在 chapter19 目录下）：
 *   g++ -std=c++17 -g -fsanitize=address,undefined \
 *       -I include/drill src/drill/drill_19_1.cpp -o /tmp/drill_19_1 && /tmp/drill_19_1
 */

#include "drill_19_1.h"
#include <sstream>
#include <string>
#include <cassert>

// ---- 极简测试框架 ----
static int g_pass = 0, g_fail = 0;
void check(bool cond, const std::string& msg) {
    if (cond) { ++g_pass; }
    else       { ++g_fail; std::cout << "  [FAIL] " << msg << "\n"; }
}

// ============ 测试 1：S<T> 基本操作 ============
void test_S_basic() {
    std::cout << "[Test 1] S<T> 基本操作\n";

    S<int>         si(42);
    S<double>      sd(3.14);
    S<char>        sc('Z');
    S<std::string> ss("hello");

    check(si.get() == 42,        "S<int> get");
    check(sd.get() == 3.14,      "S<double> get");
    check(sc.get() == 'Z',       "S<char> get");
    check(ss.get() == "hello",   "S<string> get");
}

// ============ 测试 2：S<T> 拷贝构造 / 拷贝赋值 ============
void test_S_copy() {
    std::cout << "[Test 2] S<T> 拷贝\n";

    S<int> a(10);
    S<int> b(a);                   // 拷贝构造
    check(b.get() == 10, "拷贝构造 S<int>");

    S<int> c(99);
    c = a;                         // 拷贝赋值
    check(c.get() == 10, "拷贝赋值 S<int>");

    // 修改原对象不影响副本（值语义）
    a.get() = 777;
    check(b.get() == 10, "拷贝构造后修改原对象不影响副本");
    check(c.get() == 10, "拷贝赋值后修改原对象不影响副本");
}

// ============ 测试 3：S<T> 移动构造 / 移动赋值 ============
void test_S_move() {
    std::cout << "[Test 3] S<T> 移动\n";

    S<std::string> a("world");
    S<std::string> b(std::move(a));   // 移动构造
    check(b.get() == "world", "移动构造后新对象值正确");

    S<std::string> c("tmp");
    c = S<std::string>("moved");      // 移动赋值
    check(c.get() == "moved", "移动赋值后值正确");
}

// ============ 测试 4：vector<T> operator<< ============
void test_vector_output() {
    std::cout << "[Test 4] vector<T> operator<<\n";

    std::ostringstream oss;

    std::vector<int> v1 = {1, 2, 3};
    oss << v1;
    check(oss.str() == "{1, 2, 3}", "输出 {1, 2, 3}，实际: " + oss.str());

    oss.str("");
    std::vector<int> empty;
    oss << empty;
    check(oss.str() == "{}", "空 vector 输出 {}，实际: " + oss.str());

    oss.str("");
    std::vector<int> single = {42};
    oss << single;
    check(oss.str() == "{42}", "单元素输出 {42}，实际: " + oss.str());
}

// ============ 测试 5：vector<T> operator>> ============
void test_vector_input() {
    std::cout << "[Test 5] vector<T> operator>>\n";

    // 正常输入
    {
        std::istringstream iss("{ 10, 20, 30 }");
        std::vector<int> v;
        iss >> v;
        check(iss.good() || iss.eof(), "正常输入流状态");
        check(v.size() == 3,  "正常输入元素个数=3");
        check(v[0]==10 && v[1]==20 && v[2]==30, "正常输入元素值正确");
    }

    // 空 vector
    {
        std::istringstream iss("{ }");
        std::vector<int> v = {99};   // 原本有值，读完应变空
        iss >> v;
        check(!iss.fail(), "空 vector 流状态正常");
        check(v.empty(), "空 vector 读完后 size=0");
    }

    // 单元素
    {
        std::istringstream iss("{ 7 }");
        std::vector<int> v;
        iss >> v;
        check(!iss.fail() && v.size()==1 && v[0]==7, "单元素读取正确");
    }

    // 格式错误：缺少 {
    {
        std::istringstream iss("1, 2, 3}");
        std::vector<int> v = {9, 9};
        iss >> v;
        check(iss.fail(), "缺 { 应设 failbit");
        check(v.size()==2, "格式错误时原 vector 不被修改");
    }

    // 格式错误：分隔符不是逗号
    {
        std::istringstream iss("{ 1; 2; 3 }");
        std::vector<int> v;
        iss >> v;
        check(iss.fail(), "非法分隔符应设 failbit");
    }

    // 格式错误：缺少 }
    {
        std::istringstream iss("{ 1, 2, 3");
        std::vector<int> v;
        iss >> v;
        check(iss.fail(), "缺 } 应设 failbit");
    }
}

// ============ 测试 6：S<vector<int>> + read_val ============
void test_S_vector() {
    std::cout << "[Test 6] S<vector<int>> + read_val\n";

    std::vector<int> init = {1, 2, 3};
    S<std::vector<int>> sv(init);

    // get() 正确
    check(sv.get().size() == 3, "S<vector<int>> get() size=3");
    check(sv.get()[0]==1 && sv.get()[2]==3, "S<vector<int>> get() 值正确");

    // operator<< 通过 S 的 get() 使用
    std::ostringstream oss;
    oss << sv.get();
    check(oss.str() == "{1, 2, 3}", "S<vector<int>> 输出正确: " + oss.str());

    // read_val 用 >> 读入（这正是 bonus 题要求的）
    std::istringstream iss("{ 10, 20, 30, 40 }");
    sv.read_val(iss);
    check(!iss.fail(), "read_val 流状态正常");
    check(sv.get().size() == 4, "read_val 后 size=4");
    check(sv.get()[3] == 40,    "read_val 后最后一个元素=40");
}

// ============ 测试 7：连续读取 ============
void test_consecutive_reads() {
    std::cout << "[Test 7] 连续读取\n";
    std::istringstream iss("{ 1, 2 } { 3, 4, 5 }");
    std::vector<int> a, b;
    iss >> a >> b;
    check(!iss.fail(),          "连续读取流状态正常");
    check(a.size()==2 && b.size()==3, "连续读取各自 size 正确");
    check(a[0]==1 && b[2]==5,   "连续读取值正确");
}

int main()
{
    std::cout << "===== drill_19_1 测试开始 =====\n";

    test_S_basic();
    test_S_copy();
    test_S_move();
    test_vector_output();
    test_vector_input();
    test_S_vector();
    test_consecutive_reads();

    std::cout << "===== 测试结束 =====\n";
    std::cout << "PASS: " << g_pass << "   FAIL: " << g_fail << "\n";
    return g_fail == 0 ? 0 : 1;
}
