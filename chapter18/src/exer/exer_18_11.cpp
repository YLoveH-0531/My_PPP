/**
 * @file exer_18_11.cpp
 * @author KaKaRot
 * @brief Skip list 测试
 * @version 0.1
 * @date 2026-06-04
 *
 * @copyright Copyright (c) 2026
 *
 * 由于 Skip_list 只暴露 insert / at（无遍历接口），
 * 测试策略 = 插入 + 查询的可观察行为，并用 std::set 做对照交叉验证。
 *
 * 编译运行（在 chapter18 目录下）：
 *   g++ -std=c++17 -I include/exer \
 *       src/exer/exer_18_11.cpp src/exer/Skip_List.cpp -o test_skiplist
 *   ./test_skiplist
 */

#include "Skip_List.h"

#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <random>

// ---- 极简测试框架 ----
static int g_pass = 0;
static int g_fail = 0;

void check(bool cond, const std::string& msg) {
    if (cond) {
        ++g_pass;
        // 通过的不打印，免得刷屏；想看全部就解开下一行
        // std::cout << "  [PASS] " << msg << "\n";
    } else {
        ++g_fail;
        std::cout << "  [FAIL] " << msg << "\n";
    }
}

// ============ 测试 1：基本插入与查找 ============
void test_basic() {
    std::cout << "[Test 1] 基本插入与查找\n";
    Skip_list sl;

    std::vector<int> values = {30, 10, 50, 20, 40, 25, 5, 60};
    for (int v : values) {
        sl.insert(v);
    }

    // 所有插入的值都应能查到，且 data 正确
    for (int v : values) {
        node* p = sl.at(v);
        check(p != nullptr, "应能查到 " + std::to_string(v));
        if (p) {
            check(p->data == v, "查到的 data 应等于 " + std::to_string(v));
        }
    }

    // 不存在的值应返回 nullptr
    std::vector<int> absent = {1, 11, 99, 35, 1000, -5};
    for (int v : absent) {
        check(sl.at(v) == nullptr, std::to_string(v) + " 不存在，应返回 nullptr");
    }
}

// ============ 测试 2：重复插入 ============
void test_duplicate() {
    std::cout << "[Test 2] 重复插入\n";
    Skip_list sl;

    node* first  = sl.insert(42);
    node* second = sl.insert(42);   // 重复，应返回已存在的同一节点，不新建

    check(first != nullptr, "首次插入 42 应成功");
    check(second != nullptr, "重复插入 42 应返回非空");
    check(first == second, "重复插入应返回同一个节点（未新建）");

    node* found = sl.at(42);
    check(found == first, "at(42) 应返回那唯一的节点");
}

// ============ 测试 3：空表查询 ============
void test_empty() {
    std::cout << "[Test 3] 空表查询\n";
    Skip_list sl;
    check(sl.at(123) == nullptr, "空表 at(123) 应返回 nullptr");
    check(sl.at(0)   == nullptr, "空表 at(0) 应返回 nullptr");
}

// ============ 测试 4：随机压力测试（对照 std::set）============
void test_stress() {
    std::cout << "[Test 4] 随机压力测试（对照 std::set）\n";
    Skip_list sl;
    std::set<int> ref;   // 标准答案

    std::mt19937 rng(12345);                       // 固定 seed，结果可复现
    std::uniform_int_distribution<int> dist(0, 5000);

    const int N = 2000;
    for (int i = 0; i < N; ++i) {
        int v = dist(rng);
        sl.insert(v);
        ref.insert(v);
    }

    // 对一大批随机值，跳表的"查到没"应与 set 完全一致
    int mismatches = 0;
    for (int i = 0; i < 10000; ++i) {
        int v = dist(rng);
        bool in_skip = (sl.at(v) != nullptr);
        bool in_set  = (ref.count(v) != 0);
        if (in_skip != in_set) {
            ++mismatches;
            if (mismatches <= 5) {
                std::cout << "  [MISMATCH] value=" << v
                          << " skip=" << in_skip << " set=" << in_set << "\n";
            }
        }
    }
    check(mismatches == 0, "跳表与 std::set 成员判断完全一致（共 10000 次查询）");

    // 已插入的每个值都应查得到
    int missing = 0;
    for (int v : ref) {
        if (sl.at(v) == nullptr) ++missing;
    }
    check(missing == 0, "ref 中所有值都应能在跳表中查到");
}

// ============ 测试 5：删除 erase ============
void test_erase() {
    std::cout << "[Test 5] 删除 erase\n";
    Skip_list sl;

    std::vector<int> values = {30, 10, 50, 20, 40, 25, 5, 60, 15, 45};
    for (int v : values) sl.insert(v);

    // 删除一半：被删的应查不到，返回值应非空
    std::vector<int> to_erase = {10, 20, 40, 60, 15};
    for (int v : to_erase) {
        node* r = sl.erase(v);
        check(r != nullptr, "erase 已存在的 " + std::to_string(v) + " 应返回非空");
        check(sl.at(v) == nullptr, std::to_string(v) + " 删除后应查不到");
    }

    // 没删的应该还在
    std::vector<int> remain = {30, 50, 25, 5, 45};
    for (int v : remain) {
        check(sl.at(v) != nullptr, "未删除的 " + std::to_string(v) + " 应仍在");
    }

    // 删除不存在 / 重复删除 → 返回 nullptr
    check(sl.erase(999) == nullptr, "erase 不存在的 999 应返回 nullptr");
    check(sl.erase(10)  == nullptr, "重复删除已删的 10 应返回 nullptr");

    // 删除后重新插入，应能查到
    sl.insert(20);
    check(sl.at(20) != nullptr, "删除后重新插入 20，应能查到");

    // 全部删光 → 空表
    for (int v : {30, 50, 25, 5, 45, 20}) sl.erase(v);
    for (int v : values) {
        check(sl.at(v) == nullptr, "全部删除后 " + std::to_string(v) + " 应查不到");
    }
}

// ============ 测试 6：erase 随机压力（对照 std::set）============
void test_erase_stress() {
    std::cout << "[Test 6] erase 随机压力（对照 std::set）\n";
    Skip_list sl;
    std::set<int> ref;

    std::mt19937 rng(98765);                       // 固定 seed，可复现
    std::uniform_int_distribution<int> dist(0, 2000);

    // 先插入一批
    for (int i = 0; i < 1500; ++i) {
        int v = dist(rng);
        sl.insert(v);
        ref.insert(v);
    }

    // 随机删除一批：erase 返回是否非空，应与"删之前是否存在"一致
    for (int i = 0; i < 1500; ++i) {
        int v = dist(rng);
        bool in_set = (ref.count(v) != 0);
        node* r = sl.erase(v);
        check((r != nullptr) == in_set,
              "erase(" + std::to_string(v) + ") 的返回应与是否存在一致");
        ref.erase(v);
    }

    // 删完后，成员判断应仍与 set 完全一致
    int mism = 0;
    for (int i = 0; i < 5000; ++i) {
        int v = dist(rng);
        if ((sl.at(v) != nullptr) != (ref.count(v) != 0)) ++mism;
    }
    check(mism == 0, "插入+随机删除后，跳表与 set 成员判断仍完全一致");
}

// ============ 测试 7：print 可视化（请人工核对）============
void test_print() {
    std::cout << "[Test 7] print 可视化（人工核对，不计入 PASS/FAIL）\n";
    Skip_list sl;
    for (int v : {30, 10, 50, 20, 40}) sl.insert(v);
    std::cout << "  期望：最底层应【升序】列出全部 5 个值：10 20 30 40 50\n";
    std::cout << "        更高层是稀疏子集（少数几个，同样升序）。\n";
    std::cout << "  ---- 实际输出 ----\n";
    sl.print();
    std::cout << "  ------------------\n";
}

int main() {
    std::cout << "===== Skip List 测试开始 =====\n";

    test_basic();
    test_duplicate();
    test_empty();
    test_stress();
    test_erase();          // 修好 erase 的越界后应全过
    test_erase_stress();
    test_print();          // 修好 print 的 p=head 后，最底层应升序列出全部

    std::cout << "===== 测试结束 =====\n";
    std::cout << "PASS: " << g_pass << "   FAIL: " << g_fail << "\n";

    return g_fail == 0 ? 0 : 1;
}
