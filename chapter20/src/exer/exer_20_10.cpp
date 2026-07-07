/**
 * @file exer_20_10.cpp
 * @author KaKaRot
 * @brief
 * @version 0.1
 * @date 2026-07-07
 *
 * @copyright Copyright (c) 2026
 */

#include <cassert>
#include <cstddef>
#include <iostream>
#include <string>

// 改进前：数分隔符个数。问题：
//   "hello world"  → 1（应为 2，漏了第一个词）
//   " hello world" → 2（应为 2，但对于"hello" alone 返回0）
//   "hello  world" → 2（应为 2，碰巧对，但"   "返回3应为0）
template<typename Iter>
std::size_t count_words_old(Iter first, Iter last, char separator)
{
    std::size_t count = 0;
    for (auto it = first; it != last; ++it)
        if (*it == separator)
            ++count;
    return count;
}

// 改进后：状态机，"进入单词"时才计数
//   正确处理：首尾分隔符、连续分隔符、空范围、全分隔符
template<typename Iter>
std::size_t count_words(Iter first, Iter last, char separator)
{
    std::size_t count = 0;
    bool in_word = false;
    for (auto it = first; it != last; ++it) {
        if (*it != separator) {
            if (!in_word) {
                ++count;   // 分隔符 → 非分隔符：进入新词
                in_word = true;
            }
        } else {
            in_word = false;
        }
    }
    return count;
}

// ── 测试辅助 ────────────────────────────────────────────────
static void check(const std::string& label,
                  std::size_t got, std::size_t expected)
{
    if (got == expected) {
        std::cout << "  [PASS] " << label << "\n";
    } else {
        std::cout << "  [FAIL] " << label
                  << "  got=" << got << "  expected=" << expected << "\n";
        assert(false);
    }
}

int main()
{
    // ── 基本正确性 ──────────────────────────────────────────
    std::cout << "── 基本场景 ──\n";
    {
        std::string s1 = "hello world";
        std::string s2 = "hello";
        std::string s3 = "";
        std::string s4 = "one two three";
        check("两个单词 \"hello world\"",          count_words(s1.begin(), s1.end(), ' '), 2);
        check("单个单词 \"hello\"",                count_words(s2.begin(), s2.end(), ' '), 1);
        check("空字符串",                          count_words(s3.begin(), s3.end(), ' '), 0);
        check("三个单词 \"one two three\"",        count_words(s4.begin(), s4.end(), ' '), 3);
    }

    // ── 首尾分隔符 ──────────────────────────────────────────
    std::cout << "── 首尾分隔符 ──\n";
    {
        std::string leading  = " hello world";
        std::string trailing = "hello world ";
        std::string both     = " hello world ";
        check("首部有空格 \" hello world\"",       count_words(leading.begin(),  leading.end(),  ' '), 2);
        check("尾部有空格 \"hello world \"",       count_words(trailing.begin(), trailing.end(), ' '), 2);
        check("首尾都有空格 \" hello world \"",    count_words(both.begin(),     both.end(),     ' '), 2);
    }

    // ── 连续分隔符 ──────────────────────────────────────────
    std::cout << "── 连续分隔符 ──\n";
    {
        std::string s1 = "hello  world";      // 双空格
        std::string s2 = "a   b   c";         // 多空格
        check("连续双空格 \"hello  world\"",       count_words(s1.begin(), s1.end(), ' '), 2);
        check("多空格分隔 \"a   b   c\"",          count_words(s2.begin(), s2.end(), ' '), 3);
    }

    // ── 全是分隔符 ──────────────────────────────────────────
    std::cout << "── 全是分隔符 ──\n";
    {
        std::string s1 = " ";
        std::string s2 = "   ";
        check("单个空格",                          count_words(s1.begin(), s1.end(), ' '), 0);
        check("三个空格",                          count_words(s2.begin(), s2.end(), ' '), 0);
    }

    // ── 自定义分隔符 ────────────────────────────────────────
    std::cout << "── 自定义分隔符 ──\n";
    {
        std::string csv  = "one,two,three";
        std::string pipe = "a|b|c|d";
        check("逗号分隔 \"one,two,three\"",        count_words(csv.begin(),  csv.end(),  ','), 3);
        check("竖线分隔 \"a|b|c|d\"",              count_words(pipe.begin(), pipe.end(), '|'), 4);
    }

    // ── 对比旧实现的错误 ────────────────────────────────────
    std::cout << "── 旧实现 vs 新实现对比 ──\n";
    {
        std::string s1 = "hello world";   // 旧→1，新→2
        std::string s2 = "hello";         // 旧→0，新→1
        std::string s3 = "   ";           // 旧→3，新→0
        std::string s4 = " hi there ";   // 旧→3，新→2

        std::cout << "  [旧] \"hello world\"  → " << count_words_old(s1.begin(), s1.end(), ' ')
                  << "  (正确应为 2)\n";
        std::cout << "  [新] \"hello world\"  → " << count_words(s1.begin(), s1.end(), ' ') << "\n";

        std::cout << "  [旧] \"hello\"        → " << count_words_old(s2.begin(), s2.end(), ' ')
                  << "  (正确应为 1)\n";
        std::cout << "  [新] \"hello\"        → " << count_words(s2.begin(), s2.end(), ' ') << "\n";

        std::cout << "  [旧] \"   \"          → " << count_words_old(s3.begin(), s3.end(), ' ')
                  << "  (正确应为 0)\n";
        std::cout << "  [新] \"   \"          → " << count_words(s3.begin(), s3.end(), ' ') << "\n";

        std::cout << "  [旧] \" hi there \"  → " << count_words_old(s4.begin(), s4.end(), ' ')
                  << "  (正确应为 2)\n";
        std::cout << "  [新] \" hi there \"  → " << count_words(s4.begin(), s4.end(), ' ') << "\n";
    }

    std::cout << "\n所有测试通过！\n";
    return 0;
}
