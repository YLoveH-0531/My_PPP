/**
 * @file Regex_error_test.cpp
 * @author KaKaRot
 * @brief  Test cases for Regex_error()
 * @version 0.1
 * @date 2026-09-17
 *
 * @copyright Copyright (c) 2026
 */

#include <cassert>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>

// Regex_error.cpp 没有对应头文件，此处手动前向声明
void Regex_error();

// 把 std::cin/std::cout 临时重定向到给定的流，析构时自动恢复，
// 这样即使 Regex_error() 抛异常也能保证测试之间互不影响
struct StreamRedirect {
    StreamRedirect(std::istream& in, std::ostream& out)
        : old_cin(std::cin.rdbuf(in.rdbuf())), old_cout(std::cout.rdbuf(out.rdbuf())) {}
    ~StreamRedirect() {
        std::cin.rdbuf(old_cin);
        std::cout.rdbuf(old_cout);
    }
    std::streambuf* old_cin;
    std::streambuf* old_cout;
};

#define TEST(name) void test_##name()
#define RUN(name)  do { test_##name(); std::cout << "[PASS] " #name "\n"; } while(0)

TEST(matches_and_no_match) {
    std::istringstream input("\\d+\nabc123def\nno digits\n42 and 7\n");
    std::ostringstream output;
    {
        StreamRedirect redirect(input, output);
        Regex_error();
    }
    // cin >> expre 读完 pattern 后不会吃掉行尾的 '\n'，
    // 紧接着的第一次 getline 只会读到这个残留的空行，lineno 从 1 开始记的其实是它
    std::string expected =
        "Input regex expression...\n"
        "Does not match!\n"
        "line: 2abc123def\n"
        "\tmatches[ 0]: 123\n"
        "Does not match!\n"
        "line: 442 and 7\n"
        "\tmatches[ 0]: 42\n";
    assert(output.str() == expected);
}

TEST(capture_group_submatches) {
    std::istringstream input("(\\d+)-(\\d+)\n12-34\n");
    std::ostringstream output;
    {
        StreamRedirect redirect(input, output);
        Regex_error();
    }
    std::string expected =
        "Input regex expression...\n"
        "Does not match!\n"
        "line: 212-34\n"
        "\tmatches[ 0]: 12-34\n"
        "\tmatches[ 1]: 12\n"
        "\tmatches[ 2]: 34\n";
    assert(output.str() == expected);
}

TEST(invalid_pattern_exception_escapes) {
    // Regex_error() 内部写的是 catch (std::bad_exception)，
    // 但非法正则实际抛出的是 std::regex_error —— 二者都直接派生自 std::exception，互不相关，
    // 所以这个 catch 永远接不住，异常会从函数里逃逸给调用者（这里由测试自己兜底）
    std::istringstream input("(unbalanced\n");
    std::ostringstream output;
    bool threw = false;
    {
        StreamRedirect redirect(input, output);
        try {
            Regex_error();
        } catch (const std::regex_error&) {
            threw = true;
        }
    }
    assert(threw);
    assert(output.str() == "Input regex expression...\n");
}

int main() {
    RUN(matches_and_no_match);
    RUN(capture_group_submatches);
    RUN(invalid_pattern_exception_escapes);

    std::cout << "\nAll tests passed.\n";
    return 0;
}
