/**
 * @file exer_20_8_10.cpp
 * @author KaKaRot
 * @brief
 * @version 0.1
 * @date 2026-06-29
 *
 * @copyright Copyright (c) 2026
 */

#include "exer_20_8_10.h"
#include "Document.h"
#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

// ── 工具：把字符串灌入 Document ────────────────────────────────
static Document make_doc(const std::string& text)
{
    std::istringstream ss(text);
    Document d;
    ss >> d;
    return d;
}

// ── 工具：把 Document 全文读回 std::string ─────────────────────
static std::string doc_to_string(Document& d)
{
    std::string result;
    for (auto it = d.begin(); it != d.end(); ++it)
        result += *it;
    return result;
}

// ══════════════════════════════════════════════════════════════
//  find_txt
// ══════════════════════════════════════════════════════════════
static void test_find_txt()
{
    std::cout << "── find_txt ──\n";

    // 1. 找到文档中间的字符串
    {
        Document d = make_doc("hello world\n");
        auto it = find_txt(d.begin(), d.end(), "world");
        assert(it != d.end());
        assert(*it == 'w');
        std::cout << "  [PASS] 找到中间字符串 \"world\"\n";
    }

    // 2. 找到文档开头的字符串
    {
        Document d = make_doc("hello world\n");
        auto it = find_txt(d.begin(), d.end(), "hello");
        assert(it != d.end());
        assert(*it == 'h');
        std::cout << "  [PASS] 找到开头字符串 \"hello\"\n";
    }

    // 3. 找到文档末尾的字符串（最后一个完整词）
    {
        Document d = make_doc("foo bar baz\n");
        auto it = find_txt(d.begin(), d.end(), "baz");
        assert(it != d.end());
        assert(*it == 'b');
        std::cout << "  [PASS] 找到末尾字符串 \"baz\"\n";
    }

    // 4. 字符串不存在 → 返回 end
    {
        Document d = make_doc("hello world\n");
        auto it = find_txt(d.begin(), d.end(), "xyz");
        assert(it == d.end());
        std::cout << "  [PASS] 不存在的字符串返回 end\n";
    }

    // 5. 空字符串 → 规定返回 end
    {
        Document d = make_doc("hello\n");
        auto it = find_txt(d.begin(), d.end(), "");
        assert(it == d.end());
        std::cout << "  [PASS] 空字符串返回 end\n";
    }

    // 6. 多行文档，跨行不匹配（匹配目标在第二行）
    {
        Document d = make_doc("line one\nline two\n");
        auto it = find_txt(d.begin(), d.end(), "two");
        assert(it != d.end());
        assert(*it == 't');
        std::cout << "  [PASS] 多行文档，找到第二行的字符串 \"two\"\n";
    }

    // 7. 单字符查找
    {
        Document d = make_doc("abc\n");
        auto it = find_txt(d.begin(), d.end(), "b");
        assert(it != d.end());
        assert(*it == 'b');
        std::cout << "  [PASS] 单字符查找 'b'\n";
    }
}

// ══════════════════════════════════════════════════════════════
//  find_and_replace
// ══════════════════════════════════════════════════════════════
static void test_find_and_replace()
{
    std::cout << "── find_and_replace ──\n";

    // 1. 等长替换
    {
        Document d = make_doc("hello world\n");
        find_and_replace(d.begin(), d.end(), "world", "earth");
        std::string result = doc_to_string(d);
        assert(result == "hello earth\n");
        std::cout << "  [PASS] 等长替换 world→earth\n";
    }

    // 2. 替换为更长的字符串
    {
        Document d = make_doc("hi there\n");
        find_and_replace(d.begin(), d.end(), "hi", "hello");
        std::string result = doc_to_string(d);
        assert(result == "hello there\n");
        std::cout << "  [PASS] 替换为更长字符串 hi→hello\n";
    }

    // 3. 替换为更短的字符串
    {
        Document d = make_doc("hello there\n");
        find_and_replace(d.begin(), d.end(), "hello", "hi");
        std::string result = doc_to_string(d);
        assert(result == "hi there\n");
        std::cout << "  [PASS] 替换为更短字符串 hello→hi\n";
    }

    // 4. 替换为空字符串（删除）
    {
        Document d = make_doc("foo bar\n");
        find_and_replace(d.begin(), d.end(), "foo ", "");
        std::string result = doc_to_string(d);
        assert(result == "bar\n");
        std::cout << "  [PASS] 替换为空字符串（删除）foo →\"\"\n";
    }

    // 5. 查找字符串不存在 → 文档不变，返回 end
    {
        Document d = make_doc("hello world\n");
        auto it = find_and_replace(d.begin(), d.end(), "xyz", "abc");
        assert(it == d.end());
        std::string result = doc_to_string(d);
        assert(result == "hello world\n");
        std::cout << "  [PASS] 找不到目标字符串，文档不变，返回 end\n";
    }

    // 6. 多行文档中替换第二行内容
    {
        Document d = make_doc("line one\nline two\n");
        find_and_replace(d.begin(), d.end(), "two", "2");
        std::string result = doc_to_string(d);
        assert(result == "line one\nline 2\n");
        std::cout << "  [PASS] 多行文档替换第二行 two→2\n";
    }
}

// ══════════════════════════════════════════════════════════════
//  count_char（非空白字符数）
// ══════════════════════════════════════════════════════════════
static void test_count_char()
{
    std::cout << "── count_char ──\n";

    // 1. 普通文本：只统计非空白
    {
        Document d = make_doc("hello world\n");
        // "hello world\n" → 10 letters + 0 （space 和 \n 不计）= 10
        std::size_t n = count_char(d.begin(), d.end());
        assert(n == 10);
        std::cout << "  [PASS] \"hello world\\n\" → 10 个非空白字符\n";
    }

    // 2. 全空格 → 0
    {
        Document d = make_doc("   \n");
        std::size_t n = count_char(d.begin(), d.end());
        assert(n == 0);
        std::cout << "  [PASS] 全空白字符 → 0\n";
    }

    // 3. 无空白 → 全部计入
    {
        Document d = make_doc("abc\n");
        // "abc\n" → 'a','b','c' = 3，\n 不计
        std::size_t n = count_char(d.begin(), d.end());
        assert(n == 3);
        std::cout << "  [PASS] 无空白 \"abc\\n\" → 3\n";
    }

    // 4. 包含数字和标点（非空白都算）
    {
        Document d = make_doc("a1!b2@\n");
        // a,1,!,b,2,@ = 6 个非空白
        std::size_t n = count_char(d.begin(), d.end());
        assert(n == 6);
        std::cout << "  [PASS] 含数字标点 \"a1!b2@\\n\" → 6\n";
    }

    // 5. 多行文档
    {
        Document d = make_doc("ab\ncd\n");
        // 'a','b','\n' → 2；'c','d','\n' → 2；共 4
        std::size_t n = count_char(d.begin(), d.end());
        assert(n == 4);
        std::cout << "  [PASS] 多行 \"ab\\ncd\\n\" → 4 个非空白字符\n";
    }
}

// ══════════════════════════════════════════════════════════════
//  count_words_with_space（空白字符数，含空格/tab/换行）
// ══════════════════════════════════════════════════════════════
static void test_count_words_with_space()
{
    std::cout << "── count_words_with_space ──\n";

    // 1. 普通句子
    {
        Document d = make_doc("hello world\n");
        // 空白：1个空格 + 1个\n = 2
        std::size_t n = count_words_with_space(d.begin(), d.end());
        assert(n == 2);
        std::cout << "  [PASS] \"hello world\\n\" → 2 个空白字符\n";
    }

    // 2. 无空白
    {
        Document d = make_doc("abc\n");
        // 只有末尾 \n = 1
        std::size_t n = count_words_with_space(d.begin(), d.end());
        assert(n == 1);
        std::cout << "  [PASS] \"abc\\n\" → 1（只含换行）\n";
    }

    // 3. 多个连续空格各自独立计数
    {
        Document d = make_doc("a   b\n");
        // 3个空格 + 1个\n = 4
        std::size_t n = count_words_with_space(d.begin(), d.end());
        assert(n == 4);
        std::cout << "  [PASS] \"a   b\\n\" → 4 个空白字符\n";
    }

    // 4. 多行文档：每行末尾\n也计入
    {
        Document d = make_doc("ab cd\nef gh\n");
        // 行1：1空格+\n=2；行2：1空格+\n=2 → 共4
        std::size_t n = count_words_with_space(d.begin(), d.end());
        assert(n == 4);
        std::cout << "  [PASS] 两行文档 → 4 个空白字符\n";
    }

    // 5. 全空格
    {
        Document d = make_doc("   \n");
        // 3个空格 + 1个\n = 4
        std::size_t n = count_words_with_space(d.begin(), d.end());
        assert(n == 4);
        std::cout << "  [PASS] \"   \\n\" → 4\n";
    }
}

// ══════════════════════════════════════════════════════════════
//  count_words_with_character（字母字符数，isalpha）
// ══════════════════════════════════════════════════════════════
static void test_count_words_with_character()
{
    std::cout << "── count_words_with_character ──\n";

    // 1. 只有字母
    {
        Document d = make_doc("hello\n");
        std::size_t n = count_words_with_character(d.begin(), d.end());
        assert(n == 5);
        std::cout << "  [PASS] \"hello\\n\" → 5 个字母\n";
    }

    // 2. 数字和标点不计入
    {
        Document d = make_doc("a1b2c!\n");
        // a, b, c = 3
        std::size_t n = count_words_with_character(d.begin(), d.end());
        assert(n == 3);
        std::cout << "  [PASS] \"a1b2c!\\n\" → 3（数字/标点不计）\n";
    }

    // 3. 全是数字 → 0
    {
        Document d = make_doc("12345\n");
        std::size_t n = count_words_with_character(d.begin(), d.end());
        assert(n == 0);
        std::cout << "  [PASS] \"12345\\n\" → 0\n";
    }

    // 4. 多行文档
    {
        Document d = make_doc("hi\nbye\n");
        // h,i = 2；b,y,e = 3 → 共5
        std::size_t n = count_words_with_character(d.begin(), d.end());
        assert(n == 5);
        std::cout << "  [PASS] 多行 \"hi\\nbye\\n\" → 5 个字母\n";
    }

    // 5. 含空格和换行不影响计数
    {
        Document d = make_doc("hello world\n");
        // h,e,l,l,o,w,o,r,l,d = 10
        std::size_t n = count_words_with_character(d.begin(), d.end());
        assert(n == 10);
        std::cout << "  [PASS] \"hello world\\n\" → 10 个字母\n";
    }
}

// ══════════════════════════════════════════════════════════════
//  main
// ══════════════════════════════════════════════════════════════
int main()
{
    test_find_txt();
    std::cout << '\n';
    test_find_and_replace();
    std::cout << '\n';
    test_count_char();
    std::cout << '\n';
    test_count_words_with_space();
    std::cout << '\n';
    test_count_words_with_character();
    std::cout << "\n所有测试通过！\n";
    return 0;
}
