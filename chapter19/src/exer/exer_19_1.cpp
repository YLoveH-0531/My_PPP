/**
 * @file exer_19_1.cpp
 * @author KaKaRot
 * @brief exer 19.1 — add 函数模板测试
 * @version 0.1
 * @date 2026-06-09
 *
 * @copyright Copyright (c) 2026
 *
 * 编译运行（在 chapter19 目录下）：
 *   g++ -std=c++17 -g -fsanitize=address,undefined \
 *       -I include/exer src/exer/exer_19_1.cpp -o /tmp/exer_19_1 && /tmp/exer_19_1
 */

#include "exer_19_1.h"
#include <iostream>
#include <string>
#include <cmath>

// ---- 极简测试框架 ----
static int g_pass = 0, g_fail = 0;
void check(bool cond, const std::string& msg) {
    if (cond) { ++g_pass; }
    else       { ++g_fail; std::cout << "  [FAIL] " << msg << "\n"; }
}
// 浮点比较（允许小误差）
bool near(double a, double b, double eps = 1e-9) {
    return std::abs(a - b) < eps;
}

// ============ 测试 1：add(v1, v2) — 把 v2 元素覆盖进 v1 ============
void test_add_overwrite() {
    std::cout << "[Test 1] add(v1, v2) — v2 覆盖 v1\n";

    // 等长
    {
        std::vector<int> v1 = {1, 2, 3};
        std::vector<int> v2 = {10, 20, 30};
        add(v1, v2);
        check(v1[0]==10 && v1[1]==20 && v1[2]==30, "等长：v1 被 v2 完全覆盖");
    }

    // v1 比 v2 长：只覆盖前 min(size) 个，多余的保留
    {
        std::vector<int> v1 = {1, 2, 3, 4, 5};
        std::vector<int> v2 = {10, 20};
        add(v1, v2);
        check(v1[0]==10 && v1[1]==20, "v1 更长：前两个被覆盖");
        check(v1[2]==3  && v1[3]==4 && v1[4]==5, "v1 更长：后三个不变");
    }

    // v2 比 v1 长：只覆盖 v1 全部（v2 多余的不管）
    {
        std::vector<int> v1 = {1, 2};
        std::vector<int> v2 = {10, 20, 30, 40};
        add(v1, v2);
        check(v1[0]==10 && v1[1]==20, "v2 更长：v1 全部被覆盖");
        check(v1.size() == 2, "v2 更长：v1 大小不变");
    }

    // 空 vector
    {
        std::vector<int> v1 = {1, 2, 3};
        std::vector<int> v2;
        add(v1, v2);
        check(v1[0]==1 && v1[1]==2 && v1[2]==3, "v2 为空：v1 不变");
    }

    // double 类型
    {
        std::vector<double> v1 = {1.1, 2.2, 3.3};
        std::vector<double> v2 = {9.9, 8.8, 7.7};
        add(v1, v2);
        check(near(v1[0],9.9) && near(v1[1],8.8) && near(v1[2],7.7),
              "double 类型覆盖正确");
    }
}

// ============ 测试 inner_product(vt, vu) — 点积（dot product）============
void test_add_dot_product() {
    std::cout << "[Test 2] inner_product(vt, vu) — 点积\n";

    // int · int
    {
        std::vector<int> a = {1, 2, 3};
        std::vector<int> b = {4, 5, 6};
        int result = inner_product<int>(a, b);   // 1*4 + 2*5 + 3*6 = 4+10+18 = 32
        check(result == 32, "int 点积 = 32，实际: " + std::to_string(result));
    }

    // double · double
    {
        std::vector<double> a = {1.0, 2.0, 3.0};
        std::vector<double> b = {0.5, 1.5, 2.5};
        double result = inner_product<double>(a, b);  // 0.5+3.0+7.5 = 11.0
        check(near(result, 11.0), "double 点积 = 11.0，实际: " + std::to_string(result));
    }

    // 混合类型：int · double → T=double
    {
        std::vector<double> a = {1.0, 2.0, 3.0};
        std::vector<int>    b = {2,   3,   4  };
        double result = inner_product<double, int>(a, b);  // 2.0+6.0+12.0 = 20.0
        check(near(result, 20.0), "混合类型点积 = 20.0，实际: " + std::to_string(result));
    }

    // v 长度不等：只算 min(size) 个
    {
        std::vector<int> a = {1, 2, 3, 4, 5};
        std::vector<int> b = {10, 10};
        int result = inner_product<int>(a, b);   // 1*10 + 2*10 = 30（只算前两个）
        check(result == 30, "长度不等只算前 min 个，= 30，实际: " + std::to_string(result));
    }

    // 空 vector：结果应为 0
    {
        std::vector<int> a;
        std::vector<int> b = {1, 2, 3};
        int result = inner_product<int>(a, b);
        check(result == 0, "空 vector 点积 = 0，实际: " + std::to_string(result));
    }

    // 单元素
    {
        std::vector<int> a = {7};
        std::vector<int> b = {6};
        int result = inner_product<int>(a, b);
        check(result == 42, "单元素点积 = 42，实际: " + std::to_string(result));
    }
}

int main()
{
    std::cout << "===== exer_19_1 测试开始 =====\n";

    test_add_overwrite();
    test_add_dot_product();

    std::cout << "===== 测试结束 =====\n";
    std::cout << "PASS: " << g_pass << "   FAIL: " << g_fail << "\n";
    return g_fail == 0 ? 0 : 1;
}
