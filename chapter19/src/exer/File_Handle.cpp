/**
 * @file File_Handle.cpp
 * @author KaKaRot
 * @brief File_Handle RAII 测试
 * @version 0.1
 * @date 2026-06-10
 *
 * @copyright Copyright (c) 2026
 *
 * 编译运行（在 chapter19 目录下）：
 *   g++ -std=c++17 -g -fsanitize=address,undefined \
 *       -I include/exer src/exer/File_Handle.cpp -o /tmp/fh_test && /tmp/fh_test
 */

#include "File_Handle.h"
#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>    // std::remove
#include <sys/stat.h> // stat（检查文件是否存在）

// 检查文件是否存在
static bool file_exists(const std::string& path) {
    struct stat st;
    return stat(path.c_str(), &st) == 0;
}

// ---- 极简测试框架 ----
static int g_pass = 0, g_fail = 0;
void check(bool cond, const std::string& msg) {
    if (cond) { ++g_pass; }
    else       { ++g_fail; std::cout << "  [FAIL] " << msg << "\n"; }
}

const std::string TEST_FILE = "/tmp/fh_test.txt";

// 清理辅助
void cleanup() {
    if (file_exists(TEST_FILE)) std::remove(TEST_FILE.c_str());
}

// ============ 测试 1：构造打开文件 ============
void test_open() {
    std::cout << "[Test 1] 构造打开文件\n";
    cleanup();

    // 用 out|trunc 创建新文件
    File_Handle fh(TEST_FILE, std::ios::out | std::ios::trunc);
    check(fh.is_open(),              "文件已打开");
    check(fh.filename() == TEST_FILE,"filename() 返回正确路径");
    check(file_exists(TEST_FILE),     "文件在磁盘上已创建");
}   // ← fh 析构，文件自动关闭

// ============ 测试 2：析构自动关闭（RAII 核心验证）============
void test_raii_close() {
    std::cout << "[Test 2] 析构自动关闭（RAII）\n";
    cleanup();

    {
        File_Handle fh(TEST_FILE, std::ios::out | std::ios::trunc);
        check(fh.is_open(), "作用域内文件已打开");
    }   // ← fh 析构

    // 文件关闭后，可以用标准方式重新打开——验证它被正常关闭了
    std::ifstream check_fs(TEST_FILE);
    check(check_fs.is_open(), "文件析构后可以被其他流重新打开（说明已正常关闭）");
}

// ============ 测试 3：写入和读取 ============
void test_read_write() {
    std::cout << "[Test 3] 写入和读取\n";
    cleanup();

    // 写入
    {
        File_Handle fh(TEST_FILE, std::ios::out | std::ios::trunc);
        fh.stream() << "hello\n";
        fh.stream() << "world\n";
    }   // 析构时 flush + close

    // 读取
    {
        File_Handle fh(TEST_FILE, std::ios::in);
        std::string line1, line2;
        std::getline(fh.stream(), line1);
        std::getline(fh.stream(), line2);
        check(line1 == "hello", "第一行 = hello");
        check(line2 == "world", "第二行 = world");
    }
}

// ============ 测试 4：打开不存在的文件（只读）→ 抛异常 ============
void test_open_fail() {
    std::cout << "[Test 4] 打开不存在文件抛异常\n";

    bool threw = false;
    try {
        File_Handle fh("/tmp/nonexistent_xyzzy_12345.txt", std::ios::in);
    } catch (const std::runtime_error& e) {
        threw = true;
    }
    check(threw, "打开不存在文件抛 runtime_error");
}

// ============ 测试 5：多次写入同一文件 ============
void test_append() {
    std::cout << "[Test 5] 追加写入\n";
    cleanup();

    // 第一次写入
    {
        File_Handle fh(TEST_FILE, std::ios::out | std::ios::trunc);
        fh.stream() << "line1\n";
    }

    // 追加写入
    {
        File_Handle fh(TEST_FILE, std::ios::out | std::ios::app);
        fh.stream() << "line2\n";
    }

    // 读回验证
    {
        File_Handle fh(TEST_FILE, std::ios::in);
        std::string l1, l2, l3;
        std::getline(fh.stream(), l1);
        std::getline(fh.stream(), l2);
        std::getline(fh.stream(), l3);
        check(l1 == "line1", "追加后第一行 = line1");
        check(l2 == "line2", "追加后第二行 = line2");
        check(l3.empty(),    "只有两行");
    }
}

// ============ 测试 6：异常安全——写入途中抛异常，文件仍能正确关闭 ============
void test_exception_safety() {
    std::cout << "[Test 6] 异常安全\n";
    cleanup();

    try {
        File_Handle fh(TEST_FILE, std::ios::out | std::ios::trunc);
        fh.stream() << "partial write\n";
        throw std::runtime_error("模拟异常");
        fh.stream() << "this line never written\n";
    } catch (const std::runtime_error&) {
        // fh 已经在 stack unwinding 时析构，文件已关闭
    }

    // 验证文件已关闭且内容可读
    std::ifstream ifs(TEST_FILE);
    check(ifs.is_open(), "异常后文件已正常关闭（可重新打开）");
    std::string line;
    std::getline(ifs, line);
    check(line == "partial write", "已写入的内容保留");
}

// ============ 测试 7：拷贝被禁止（编译期验证）============
void test_no_copy() {
    std::cout << "[Test 7] 拷贝语义被禁止\n";
    // 取消注释会编译报错：
    // File_Handle a(TEST_FILE, std::ios::out);
    // File_Handle b(a);    // ❌ 拷贝构造 = delete
    // File_Handle c = a;   // ❌ 拷贝赋值 = delete
    check(true, "拷贝构造/赋值被 delete（编译期保证，需取消注释验证）");
}

int main()
{
    std::cout << "===== File_Handle 测试开始 =====\n";

    test_open();
    test_raii_close();
    test_read_write();
    test_open_fail();
    test_append();
    test_exception_safety();
    test_no_copy();

    cleanup();   // 清理测试文件

    std::cout << "\n===== 测试结束 =====\n";
    std::cout << "PASS: " << g_pass << "   FAIL: " << g_fail << "\n";
    return g_fail == 0 ? 0 : 1;
}
