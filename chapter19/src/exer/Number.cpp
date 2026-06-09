/**
 * @file Number.cpp
 * @author KaKaRot
 * @brief Number<T> 测试
 * @version 0.1
 * @date 2026-06-09
 *
 * @copyright Copyright (c) 2026
 *
 * 编译运行（在 chapter19 目录下）：
 *   g++ -std=c++17 -g -fsanitize=address,undefined \
 *       -I include/exer src/exer/Number.cpp -o /tmp/number_test && /tmp/number_test
 *
 * 已知 bug（需修复后测试全过）：
 *   1. operator=(const Number&) 漏了 return *this → UB
 *   2. Number(T&& t) : val(t) 应为 val(std::move(t))
 */

#include "Number.h"
#include "exer_19_1.h"  // 包含 exercises 07 的测试
#include <iostream>
#include <string>
#include <stdexcept>
#include <cmath>

// ---- 极简测试框架 ----
static int g_pass = 0, g_fail = 0;
void check(bool cond, const std::string& msg) {
    if (cond) { ++g_pass; }
    else       { ++g_fail; std::cout << "  [FAIL] " << msg << "\n"; }
}
bool near(double a, double b, double eps = 1e-9) {
    return std::abs(a - b) < eps;
}

// 取出 Number 内部值的辅助：用算术运算反推
// 因为 val 是 private，通过 n + Number(0) 不可行，改用 n - Number(0) 也不行
// 最简单：用 operator+ 配合默认构造（Number() = 0）来间接验证
// 实际测试里我们通过运算结果来验证正确性，不直接读 val

// ============ 测试 1：构造 ============
void test_ctor() {
    std::cout << "[Test 1] 构造\n";

    Number<int> a;              // 默认构造，val = 0
    Number<int> b(42);          // 左值带参
    Number<int> c(b);           // 拷贝构造
    Number<int> d(std::move(Number<int>(99)));  // 移动构造

    // 通过加法间接验证值（a=0, b=42）
    Number<int> sum = b + a;    // 42 + 0 = 42
    Number<int> expected(42);
    check(sum == expected, "默认构造=0，带参构造=42，加法验证");

    Number<int> sum2 = c + a;   // 拷贝构造后 c=42，42+0=42
    check(sum2 == expected, "拷贝构造后值正确");
}

// ============ 测试 2：拷贝赋值（含 bug 验证）============
void test_copy_assign() {
    std::cout << "[Test 2] 拷贝赋值\n";

    Number<int> a(10);
    Number<int> b(20);
    b = a;                       // 修完 bug 后应正常工作
    check(b == Number<int>(10), "拷贝赋值后值正确");

    b = b;                       // 自赋值不崩溃
    check(b == Number<int>(10), "自赋值后值不变");
}

// ============ 测试 3：移动赋值 ============
void test_move_assign() {
    std::cout << "[Test 3] 移动赋值\n";

    Number<int> a(77);
    Number<int> b(0);
    b = std::move(a);
    check(b == Number<int>(77), "移动赋值后值正确");
}

// ============ 测试 4：operator+ ============
void test_add() {
    std::cout << "[Test 4] operator+\n";

    check(Number<int>(3)    + Number<int>(4)    == Number<int>(7),    "int 3+4=7");
    check(Number<int>(-5)   + Number<int>(5)    == Number<int>(0),    "int -5+5=0");
    check(Number<double>(1.5) + Number<double>(2.5) == Number<double>(4.0), "double 1.5+2.5=4.0");
}

// ============ 测试 5：operator- ============
void test_sub() {
    std::cout << "[Test 5] operator-\n";

    check(Number<int>(10)  - Number<int>(3)  == Number<int>(7),  "int 10-3=7");
    check(Number<int>(0)   - Number<int>(5)  == Number<int>(-5), "int 0-5=-5");
    check(Number<int>(5)   - Number<int>(5)  == Number<int>(0),  "int 5-5=0");
}

// ============ 测试 6：operator* ============
void test_mul() {
    std::cout << "[Test 6] operator*\n";

    check(Number<int>(3)  * Number<int>(4)  == Number<int>(12), "int 3*4=12");
    check(Number<int>(0)  * Number<int>(99) == Number<int>(0),  "int 0*99=0");
    check(Number<int>(-2) * Number<int>(3)  == Number<int>(-6), "int -2*3=-6");
}

// ============ 测试 7：operator/ ============
void test_div() {
    std::cout << "[Test 7] operator/\n";

    check(Number<int>(10) / Number<int>(2)  == Number<int>(5),  "int 10/2=5");
    check(Number<int>(7)  / Number<int>(2)  == Number<int>(3),  "int 7/2=3（截断）");
    check(Number<int>(-6) / Number<int>(2)  == Number<int>(-3), "int -6/2=-3");

    // 除以 0 应抛异常
    bool threw = false;
    try {
        Number<int>(5) / Number<int>(0);
    } catch (const std::runtime_error&) {
        threw = true;
    }
    check(threw, "除以 0 抛 runtime_error");
}

// ============ 测试 8：operator% ============
void test_mod() {
    std::cout << "[Test 8] operator%\n";

    check(Number<int>(10) % Number<int>(3) == Number<int>(1), "int 10%3=1");
    check(Number<int>(9)  % Number<int>(3) == Number<int>(0), "int 9%3=0");
    check(Number<int>(7)  % Number<int>(4) == Number<int>(3), "int 7%4=3");

    // 模 0 应抛异常
    bool threw = false;
    try {
        Number<int>(5) % Number<int>(0);
    } catch (const std::runtime_error&) {
        threw = true;
    }
    check(threw, "模 0 抛 runtime_error");
}

// ============ 测试 9：链式运算 ============
void test_chain() {
    std::cout << "[Test 9] 链式运算\n";

    // (2 + 3) * 4 = 20
    Number<int> r1 = (Number<int>(2) + Number<int>(3)) * Number<int>(4);
    check(r1 == Number<int>(20), "(2+3)*4=20");

    // 10 - 3 * 2 = 4（注意：运算符优先级由调用顺序决定，不是数学优先级）
    // 这里写成 (10-3)*2=14
    Number<int> r2 = (Number<int>(10) - Number<int>(3)) * Number<int>(2);
    check(r2 == Number<int>(14), "(10-3)*2=14");
}

// ============ 测试 10：double 类型运算 ============
void test_double() {
    std::cout << "[Test 10] double 类型\n";

    Number<double> a(10.0);
    Number<double> b(3.0);

    Number<double> div_result = a / b;
    // 10.0 / 3.0 ≈ 3.333...，不等于整数3
    check(!(div_result == Number<double>(3.0)), "double 10/3 != 3.0（不截断）");

    Number<double> mul_result = Number<double>(1.5) * Number<double>(4.0);
    check(mul_result == Number<double>(6.0), "double 1.5*4=6.0");
}

// ============ 测试 11：Number<T> 配合 inner_product ============
void test_exercises() {
    std::cout << "[Test 11] Number<T> + inner_product\n";

    // inner_product 要求 T 支持 += 和 *
    // Number<double> 都支持，所以可以直接用

    // {10, 3} · {3, 10} = 10*3 + 3*10 = 30 + 30 = 60
    std::vector<Number<double>> v1{Number<double>(10.0), Number<double>(3.0)};
    std::vector<Number<double>> v2{Number<double>(3.0),  Number<double>(10.0)};
    Number<double> result = inner_product<Number<double>>(v1, v2);
    check(result == Number<double>(60.0), "Number<double> 点积 {10,3}·{3,10} = 60");

    // {1, 2, 3} · {4, 5, 6} = 4+10+18 = 32
    std::vector<Number<int>> a{Number<int>(1), Number<int>(2), Number<int>(3)};
    std::vector<Number<int>> b{Number<int>(4), Number<int>(5), Number<int>(6)};
    Number<int> r2 = inner_product<Number<int>>(a, b);
    check(r2 == Number<int>(32), "Number<int> 点积 {1,2,3}·{4,5,6} = 32");

    // 空 vector → 结果为 Number 默认值（0）
    std::vector<Number<int>> empty;
    Number<int> r3 = inner_product<Number<int>>(empty, b);
    check(r3 == Number<int>(0), "空 vector 点积 = 0");
}

int main()
{
    std::cout << "===== Number<T> 测试开始 =====\n";

    test_ctor();
    test_copy_assign();
    test_move_assign();
    test_add();
    test_sub();
    test_mul();
    test_div();
    test_mod();
    test_chain();
    test_double();
    test_exercises();
    std::cout << "\n===== 测试结束 =====\n";
    std::cout << "PASS: " << g_pass << "   FAIL: " << g_fail << "\n";
    return g_fail == 0 ? 0 : 1;
}
